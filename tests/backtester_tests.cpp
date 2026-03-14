#include <gtest/gtest.h>
#include <map>
#include <memory>
#include <queue>

#include "../include/backtester.h"
#include "../include/data.h"
#include "../include/event.h"
#include "../include/execution.h"
#include "../include/portfolio.h"
#include "../include/strategy.h"

class MockDataHandler : public DataHandler {
	std::queue<std::unique_ptr<Event>>& events;
	bool continue_bt = true;
public:
	explicit MockDataHandler(std::queue<std::unique_ptr<Event>>& ev) : events(ev) {}

	std::vector<Bar> get_latest_bars(const std::string& symbol, std::size_t N = 1) const override {
		return {};
	}
	std::vector<std::string> get_symbols() const override { return {"MOCK"}; }

	// simulates a heartbeat by pushing a MarketEvent
	void update_bars() override { events.push(std::make_unique<MarketEvent>()); }

	bool if_continue_backtest() const override { return continue_bt; }
	void set_continue(bool val) { continue_bt = val; }
};

class MockStrategy : public Strategy {
	std::queue<std::unique_ptr<Event>>& events;
public:
	int market_events_handled = 0;

	explicit MockStrategy(std::queue<std::unique_ptr<Event>>& ev) : events(ev) {}

	// when it receives a MARKET event, it generates a SIGNAL event
	void calculate_signals(const Event& event) override {
		if (event.type() == EventType::MARKET) {
			market_events_handled++;
			events.push(std::make_unique<SignalEvent>("MOCK", std::chrono::system_clock::now(), "LONG", 10));
		}
	}

	std::map<std::string, double> get_indicators() const override { return {}; }
	void plot() override {}
};

class MockPortfolio : public Portfolio {
	std::queue<std::unique_ptr<Event>>& events;
public:
	int timeindex_updates = 0;
	int signals_handled = 0;
	int fills_handled = 0;

	explicit MockPortfolio(std::queue<std::unique_ptr<Event>>& ev) : events(ev) {}

	void update_timeindex(const Event& event) override { timeindex_updates++; }

	// when it receives a SIGNAL, it generates an ORDER
	void update_signal(const Event& event) override {
		signals_handled++;
		events.push(std::make_unique<OrderEvent>("MOCK", "MKT", 10, "BUY"));
	}

	// wWhen it receives a FILL, it just counts it
	void update_fill(const Event& event) override { fills_handled++; }

	double get_cash() const override { return 0.0; }
	int get_position(const std::string& symbol) const override { return 0; }
};

class MockExecutionHandler : public ExecutionHandler {
	std::queue<std::unique_ptr<Event>>& events;
public:
	int orders_handled = 0;

	explicit MockExecutionHandler(std::queue<std::unique_ptr<Event>>& ev) : events(ev) {}

	// when it receives an ORDER, it generates a FILL
	void execute_order(const Event& event) override {
		orders_handled++;
		events.push(std::make_unique<FillEvent>(std::chrono::system_clock::now(), "MOCK", "ARCA", 10, "BUY",
												100.0, 1.0));
	}
};

class BacktesterTest : public ::testing::Test {
protected:
	std::shared_ptr<std::queue<std::unique_ptr<Event>>> events;
	std::unique_ptr<Backtester> engine;

	// raw pointers to check counters after ownership is moved to Backtester
	MockDataHandler* mock_data;
	MockPortfolio* mock_port;
	MockExecutionHandler* mock_exec;
	MockStrategy* mock_strat;

	void SetUp() override {
		events = std::make_shared<std::queue<std::unique_ptr<Event>>>();

		auto dh = std::make_unique<MockDataHandler>(*events);
		auto port = std::make_unique<MockPortfolio>(*events);
		auto exec = std::make_unique<MockExecutionHandler>(*events);

		// save raw pointers for assertion checking later
		mock_data = dh.get();
		mock_port = port.get();
		mock_exec = exec.get();

		engine = std::make_unique<Backtester>(events, std::move(dh), std::move(port), std::move(exec));
	}

	void AttachStrategy() {
		auto strat = std::make_unique<MockStrategy>(*events);
		mock_strat = strat.get();
		engine->set_strategy(std::move(strat));
	}
};

// If we forget to add a strategy, it should throw an error.
TEST_F(BacktesterTest, ThrowsIfNoStrategySet) {
	// we intentionally do NOT call AttachStrategy()
	EXPECT_THROW(engine->step(), std::runtime_error);
}

// If the CSV is out of data, step() should cleanly return false.
TEST_F(BacktesterTest, ReturnsFalseWhenDataExhausted) {
	AttachStrategy();
	mock_data->set_continue(false);

	bool result = engine->step();
	EXPECT_FALSE(result);
	EXPECT_EQ(events->size(), 0);
}

// Check that the entire event loop cascade works flawlessly.
TEST_F(BacktesterTest, FullEventCascadeRouting) {
	AttachStrategy();

	bool result = engine->step();

	// should return true (data was processed)
	EXPECT_TRUE(result);

	// the event queue must be completely empty (drained) at the end of the tick
	EXPECT_EQ(events->size(), 0);

	//    MarketEvent -> Strategy
	EXPECT_EQ(mock_strat->market_events_handled, 1);

	//    MarketEvent -> Portfolio (Update History)
	EXPECT_EQ(mock_port->timeindex_updates, 1);

	//    SignalEvent -> Portfolio
	EXPECT_EQ(mock_port->signals_handled, 1);

	//    OrderEvent -> ExecutionHandler
	EXPECT_EQ(mock_exec->orders_handled, 1);

	//    FillEvent -> Portfolio
	EXPECT_EQ(mock_port->fills_handled, 1);
}
