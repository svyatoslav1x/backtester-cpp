#include "../../include/execution.h"
#include <chrono>

SimulatedExecutionHandler::SimulatedExecutionHandler(std::queue<std::unique_ptr<Event>>& events_) :
	events(events_) {}

void SimulatedExecutionHandler::execute_order(const Event& event) {
	if (event.type() != EventType::ORDER) {
		return;
	}

	const auto& order_event = static_cast<const OrderEvent&>(event);

	// In a simulated environment, we assume the order is filled immediately.
	// The fill_cost is set to 0.0 here because the NaivePortfolio
	// calculates the fill cost based on the latest market price
	// when it receives the FillEvent.

	auto fill_time = std::chrono::system_clock::now();

	auto fill_event = std::make_unique<FillEvent>(fill_time, order_event.get_symbol(), "SIM_EXCHANGE",
												  order_event.get_quantity(), order_event.get_direction(),
												  0.0, // fill_cost (to be estimated by Portfolio)
												  -1.0 // pass -1 to trigger IB commission calculation
	);

	events.push(std::move(fill_event));
}
