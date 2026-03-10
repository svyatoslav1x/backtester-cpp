#pragma once

#include <memory>
#include <queue>
#include <vector>

#include "data.h"
#include "event.h"
#include "execution.h"
#include "portfolio.h"
#include "strategy.h"

class Backtester {
private:
	std::shared_ptr<std::queue<std::unique_ptr<Event>>> events;

	std::unique_ptr<DataHandler> data_handler;
	std::unique_ptr<Portfolio> portfolio;
	std::unique_ptr<ExecutionHandler> execution_handler;
	std::unique_ptr<Strategy> strategy;
public:
	Backtester(std::shared_ptr<std::queue<std::unique_ptr<Event>>> events_queue,
			   std::unique_ptr<DataHandler> dh,
			   std::unique_ptr<Portfolio> port,
			   std::unique_ptr<ExecutionHandler> exec);

	// the core loop
	void run();

	bool step(); // Processes one bar. Returns false if no data left.

	// set the strategy (can be changed between runs)
	void set_strategy(std::unique_ptr<Strategy> new_strategy);

	// getters to allow access to core modules
	DataHandler& get_data_handler() { return *data_handler; }
	Portfolio& get_portfolio() { return *portfolio; }
	std::queue<std::unique_ptr<Event>>& get_events() { return *events; }
	Strategy* get_strategy() { return strategy.get(); } // To extract indicator values
};
