#include "../../include/backtester.h"

Backtester::Backtester(std::shared_ptr<std::queue<std::unique_ptr<Event>>> events_queue,
					   std::unique_ptr<DataHandler> dh,
					   std::unique_ptr<Portfolio> port,
					   std::unique_ptr<ExecutionHandler> exec) :
	events(events_queue), data_handler(std::move(dh)), portfolio(std::move(port)),
	execution_handler(std::move(exec)) {}

void Backtester::set_strategy(std::unique_ptr<Strategy> new_strategy) { strategy = std::move(new_strategy); }

bool Backtester::step() {
	// check if the strategy was injected
	if (!strategy) {
		throw std::runtime_error("Error: Strategy not set in Backtester before calling step().");
	}

	// check if we have reached the end of the CSV data
	if (!data_handler->if_continue_backtest()) {
		return false;
	}

	// triggers system heartbeat (if there is data in csv files, processes new line
	// , adds new bar to the latest_symbol_data and creates new MarketEvent)
	data_handler->update_bars();

	// process the Event Queue until it is completely empty for this specific timestamp
	// this ensures cascading events (Market -> Signal -> Order -> Fill) all happen in the same "tick"
	while (!events->empty()) {
		// pop the latest event and take ownership of the memory
		std::unique_ptr<Event> event = std::move(events->front());
		events->pop();

		if (event) {
			switch (event->type()) {
			case EventType::MARKET:
				// calculate indicators/signals first, then update portfolio holdings
				strategy->calculate_signals(*event);
				portfolio->update_timeindex(*event);
				break;

			case EventType::SIGNAL:
				// convert signal advice into concrete orders
				portfolio->update_signal(*event);
				break;

			case EventType::ORDER:
				// send order to the simulated exchange
				execution_handler->execute_order(*event);
				break;

			case EventType::FILL:
				// process receipt of filled order (updates cash, positions, and commission)
				portfolio->update_fill(*event);
				break;
			}
		}
	}

	return true;
}
