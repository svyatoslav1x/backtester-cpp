#include <chrono>
#include <gtest/gtest.h>
#include <memory>
#include <queue>

#include "../include/event.h"
#include "../include/execution.h"

class ExecutionTest : public ::testing::Test {
protected:
	std::queue<std::unique_ptr<Event>> events;
	std::unique_ptr<SimulatedExecutionHandler> broker;

	void SetUp() override { broker = std::make_unique<SimulatedExecutionHandler>(events); }
};

// Check that we get a valid FillEvent from an OrderEvent
TEST_F(ExecutionTest, OrderGeneratesFill) {
	OrderEvent order("AAPL", "MKT", 100, "BUY");

	broker->execute_order(order);

	ASSERT_EQ(events.size(), 1);

	auto event = std::move(events.front());
	events.pop();

	ASSERT_EQ(event->type(), EventType::FILL);

	auto* fill = static_cast<FillEvent*>(event.get());
	EXPECT_EQ(fill->get_symbol(), "AAPL");
	EXPECT_EQ(fill->get_quantity(), 100);
	EXPECT_EQ(fill->get_direction(), "BUY");
}

// Check that executing an INCORRECT event type does nothing
TEST_F(ExecutionTest, IgnoresNonOrderEvents) {
	MarketEvent market_event;

	broker->execute_order(market_event);

	EXPECT_EQ(events.size(), 0);
}

// Check commission calculation
TEST_F(ExecutionTest, CommissionCalculation) {
	OrderEvent order("AAPL", "MKT", 100, "BUY");
	broker->execute_order(order);

	auto event = std::move(events.front());
	events.pop();

	auto* fill = static_cast<FillEvent*>(event.get());

	// based on calculate_ib_commission():
	// 100 shares * $0.013/share = $1.30
	// minimum commission is $1.30.
	EXPECT_DOUBLE_EQ(fill->get_commission(), 1.30);
}

// Check large order commission calculation
TEST_F(ExecutionTest, LargeCommissionCalculation) {
	OrderEvent order("AAPL", "MKT", 1000, "SELL");
	broker->execute_order(order);

	auto event = std::move(events.front());
	events.pop();

	auto* fill = static_cast<FillEvent*>(event.get());

	// based on calculate_ib_commission():
	// quantity > 500, so it uses $0.008/share.
	// 1000 shares * $0.008 = $8.00
	EXPECT_DOUBLE_EQ(fill->get_commission(), 8.00);
}
