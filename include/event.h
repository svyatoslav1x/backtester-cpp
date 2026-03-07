#pragma once

#include <chrono>
#include <string>

enum class EventType {
	MARKET,
	SIGNAL,
	ORDER,
	FILL
};

class Event {
	// Event is base class providing an interface for all subsequent
	// (inherited) events, that will trigger further events in the
	// trading infrastructure.
public:
	virtual ~Event() = default;
	virtual EventType type() const = 0;
	virtual std::string get_symbol() const = 0;
	virtual int get_quantity() const = 0;
	virtual std::string get_direction() const = 0;
};

class MarketEvent : public Event {
	// Is provided by DataHandler to trigger the Strategy object generating new trading signals.
	// Triggered when the outer while loop begins a new "heartbeat"
	// and DataHandler object receives a new update of market data for any symbols which are
	// currently being tracked
public:
	EventType type() const override { return EventType::MARKET; }
	std::string get_symbol() const override { return ""; }
	int get_quantity() const override { return 0; }
	std::string get_direction() const override { return ""; }
};

class SignalEvent : public Event {
	// Is utilised by the Portfolio object as advice for how to trade
	// The Strategy object utilises market data to create new SignalEvents
	std::string symbol; // the ticker symbol
	std::chrono::system_clock::time_point datetime; // when it was generated
	std::string signal_type; // LONG or SHORT
	int quantity;
public:
	SignalEvent(std::string symbol,
				std::chrono::system_clock::time_point datetime,
				std::string signal_type,
				int quantity);
	EventType type() const override { return EventType::SIGNAL; }
	std::string get_symbol() const override { return symbol; }
	int get_quantity() const override { return quantity; }
	std::string get_direction() const override { return signal_type; }
};

class OrderEvent : public Event {
	// When a Portfolio object receives SignalEvents it assesses them in the wider context of the
	// portfolio, in terms of risk and position sizing. This ultimately leads to OrderEvents that
	// will be sent to an ExecutionHandler has a print_order() method, used to output the
	// information to the console if necessary
	std::string symbol;
	std::string order_type; // 'MKT' or 'LMT' for Market or Limit
	int quantity;
	std::string direction; // 'BUY' or 'SELL' for long or short
public:
	OrderEvent(std::string symbol, std::string order_type, int quantity, std::string direction);
	EventType type() const override { return EventType::ORDER; }
	std::string get_symbol() const override { return symbol; }
	int get_quantity() const override { return quantity; }
	std::string get_direction() const override { return direction; }
	void print_order() const;
};

class FillEvent : public Event {
	// When an ExecutionHandler receives an OrderEvent it must transact the order.
	// Once an order has been transacted it generates a FillEvent,
	// which describes the cost of purchase or sale as well as the transaction costs, such as fees
	// or slippage.
	// TODO here is an opportunity to make a slippage
	std::chrono::system_clock::time_point timeindex; // the bar-resolution when the order was filled.
	std::string symbol;
	std::string exchange; // the exchange where the order was filled.
	int quantity; // the quantity of SHARES transacted
	std::string direction; // 'BUY' or 'SELL' for long or short
	double fill_cost; // the cost of the fill in dollars
	double commission; // commission for trading on IB (Interactive Brokers exchange)
public:
	FillEvent(std::chrono::system_clock::time_point time,
			  std::string s,
			  std::string ex,
			  int q,
			  std::string d,
			  double fill_cost,
			  double com = -1.0);
	double calculate_ib_commission() const;
	EventType type() const override { return EventType::FILL; }
	std::string get_symbol() const override { return symbol; }
	int get_quantity() const override { return quantity; }
	std::string get_direction() const override { return direction; }
	double get_fill_cost() const { return fill_cost; }
	double get_commission() const { return commission; }
};
