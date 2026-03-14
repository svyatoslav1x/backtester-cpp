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
	double cost = IB_MIN_COMMISSION;
	if (quantity <= IB_TIER_THRESHOLD) {
		cost = std::max(IB_MIN_COMMISSION, IB_RATE_TIER_1 * quantity);
	} else {
		cost = std::max(IB_MIN_COMMISSION, IB_RATE_TIER_2 * quantity);
	}

	if (fill_cost > 0.0) {
		cost = std::min(cost, IB_MAX_PCT_CAP * quantity * fill_cost);
	}
	return cost;
}
