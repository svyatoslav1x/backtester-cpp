#include <chrono>
#include <gtest/gtest.h>
#include <memory>
#include <queue>
#include <vector>

#include "../include/data.h"
#include "../include/event.h"
#include "../include/portfolio.h"

class FakeDataHandler : public DataHandler {
public:
	std::vector<std::string> symbols = {"AAPL"};
	double current_price = 150.0;

	std::vector<std::string> get_symbols() const override { return symbols; }

	std::vector<Bar> get_latest_bars(const std::string& symbol, std::size_t N = 1) const override {
		Bar b;
		b.datetime = std::chrono::system_clock::now();
		b.close = current_price;
		return {b};
	}

	void update_bars() override {}
	bool if_continue_backtest() const override { return true; }
};

class PortfolioTest : public ::testing::Test {
protected:
	std::queue<std::unique_ptr<Event>> events;
	FakeDataHandler fake_data;
	std::unique_ptr<NaivePortfolio> portfolio;

	void SetUp() override {
		portfolio = std::make_unique<NaivePortfolio>(fake_data, events, "TestStrategy", 100000.0);
	}
};

// Check correct initial state
TEST_F(PortfolioTest, InitialStateIsCorrect) {
	EXPECT_DOUBLE_EQ(portfolio->get_cash(), 100000.0);
	EXPECT_EQ(portfolio->get_position("AAPL"), 0);
}

// Check that we correctly get an OrderEvent from a SignalEvent
TEST_F(PortfolioTest, SignalGeneratesOrder) {
	SignalEvent signal("AAPL", std::chrono::system_clock::now(), "LONG", 50);

	portfolio->update_signal(signal);

	ASSERT_EQ(events.size(), 1);

	auto event = std::move(events.front());
	events.pop();

	ASSERT_EQ(event->type(), EventType::ORDER);

	auto* order = static_cast<OrderEvent*>(event.get());
	EXPECT_EQ(order->get_symbol(), "AAPL");
	EXPECT_EQ(order->get_direction(), "BUY");
	EXPECT_EQ(order->get_quantity(), 50);
}

// Check the math when a BUY (Long) order is filled
TEST_F(PortfolioTest, FillEventUpdatesHoldingsForBuy) {
	// bought 10 shares of AAPL at 150 USD each, commission was 1.3 USD
	FillEvent fill(std::chrono::system_clock::now(), "AAPL", "Test", 10, "BUY", 150.0, 1.30);

	portfolio->update_fill(fill);

	// should be: 100,000 - (10 * 150) - 1.30 = 98498.70
	EXPECT_DOUBLE_EQ(portfolio->get_cash(), 98498.70);

	// should now own exactly 10 shares
	EXPECT_EQ(portfolio->get_position("AAPL"), 10);
}

// Check the math when a SELL (Short) order is filled
TEST_F(PortfolioTest, FillEventUpdatesHoldingsForShortSell) {
	// sold 20 shares of AAPL at 150 USD each, commission was 2 USD
	FillEvent fill(std::chrono::system_clock::now(), "AAPL", "Test", 20, "SELL", 150.0, 2.00);

	portfolio->update_fill(fill);

	// Cash increases from the short sale, minus commission
	// should be: 100,000 + (20 * 150) - 2.00 = 102998.00
	EXPECT_DOUBLE_EQ(portfolio->get_cash(), 102998.00);

	// position should be negative (representing a short)
	EXPECT_EQ(portfolio->get_position("AAPL"), -20);
}

// Check the signal logic based on current position
TEST_F(PortfolioTest, ExitSignalClosesPosition) {
	FillEvent fill_buy(std::chrono::system_clock::now(), "AAPL", "Test", 30, "BUY", 150.0, 1.30);
	portfolio->update_fill(fill_buy);

	SignalEvent exit_signal("AAPL", std::chrono::system_clock::now(), "EXIT", 30);
	portfolio->update_signal(exit_signal);

	// verify it created a SELL order to close the 30 shares
	ASSERT_EQ(events.size(), 1);
	auto* order = static_cast<OrderEvent*>(events.front().get());

	EXPECT_EQ(order->get_direction(), "SELL");
	EXPECT_EQ(order->get_quantity(), 30);
}
