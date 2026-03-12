#include "../../include/event.h"
#include <algorithm>
#include <cstdio>

SignalEvent::SignalEvent(std::string symbol,
	std::chrono::system_clock::time_point datetime,
	std::string signal_type,
	int quantity) :
	symbol(symbol), datetime(datetime), signal_type(signal_type), quantity(quantity) {}

OrderEvent::OrderEvent(std::string symbol, std::string order_type, int quantity, std::string direction) :
	symbol(symbol), order_type(order_type), quantity(quantity), direction(direction) {}

void OrderEvent::print_order() const {
	std::printf("Order: Symbol=%s, Type=%s, Quantity=%d, Direction=%s\n", symbol.c_str(), order_type.c_str(),
				quantity, direction.c_str());
}

FillEvent::FillEvent(std::chrono::system_clock::time_point time,
	std::string s,
	std::string ex,
	int q,
	std::string d,
	double fill_cost,
	double com) :
	timeindex(time), symbol(std::move(s)), exchange(std::move(ex)), quantity(q), direction(std::move(d)),
	fill_cost(fill_cost), commission(com) {
	if (this->commission < 0.0) {
		this->commission = calculate_ib_commission();
	}
}

double FillEvent::calculate_ib_commission() const {
	double full_cost = 1.3;
	if (quantity <= 500) {
		full_cost = std::max(1.3, 0.013 * quantity);
	} else {
		full_cost = std::max(1.3, 0.008 * quantity);
	}

	full_cost = std::min(full_cost, (0.5 / 100.0) * quantity * fill_cost);
	return full_cost;
}
