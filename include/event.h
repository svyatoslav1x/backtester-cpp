#include <chrono>
#include <iostream>
#include <string>


enum class EventType {
    MARKET,
    SIGNAL,
    ORDER,
    FILL
};

class Event { //base class from wich methods are inherited
public:
    virtual ~Event() = default;
    virtual EventType type() const = 0; // return the market event
    virtual std::string get_symbol() = 0;
    virtual int get_quantity() = 0;
    virtual std::string get_direction() = 0;
};

class MarketEvent : public Event { // is used to trigger the Strategy object generating new trading signals
    //triggered when the outer while loop begins a new "heartbeat"
    // and DataHandler object receives a new update of market data for any symbols which are currently being tracked
public:
    EventType type() const override {
        return EventType::MARKET;
    }
    std::string get_symbol() override = 0;
    int get_quantity() override = 0;
    std::string get_direction() override = 0;
};

class SignalEvent : public Event { // are utilised by the Portfolio object as advice for how to trade
    //The Strategy object utilises market data to create new SignalEvents
    std::string symbol; // the ticker symbol
    std::string datatime; //when it was generated
    std::string signal_type; // LONG or SHORT
    int quantity; // how many want to bue/sell
public:
    SignalEvent (std::string symbol, std::string datatime, std::string signal_type, int quantity):
    symbol(symbol), datatime(datatime), signal_type(signal_type), quantity(quantity){}
    EventType type() const override {
        return EventType::SIGNAL;
    }
    std::string get_symbol() override {return symbol;}
    int get_quantity() override {return quantity;}
    std::string get_direction() override = 0;
};

class OrderEvent : public Event {
    //has a print_order() method, used to output the information to the console if necessary
    //When a Portfolio object receives SignalEvents it assesses them in the wider context of the portfolio,
    //in terms of risk and position sizing. This ultimately leads to OrderEvents that will be sent to an ExecutionHandler
    std::string symbol; // The instrument to trade.
    std::string order_type; //'MKT' or 'LMT' for Market or Limit.
    int quantity; //Non-negative integer for quantity.
    std::string direction; //'BUY' or 'SELL' for long or short.
public:
    OrderEvent(std::string symbol, std::string order_type, int quantity, std::string direction) :
    symbol(symbol), order_type(order_type), quantity(quantity), direction(direction)
    {}
    EventType type() const override {
        return EventType::ORDER;
    }
    std::string get_symbol() override {return symbol;}
    int get_quantity() override {return quantity;}
    std::string get_direction() override {return direction;}
    void print_order() {
        std::cout << "Order: Symbol=%s, Type=%s, Quantity=%s, Direction=%s" % (self.symbol, self.order_type, self.quantity, self.direction);
    }
};

class FillEvent : public Event {
    // When an ExecutionHandler receives an OrderEvent it must transact the order.
    // Once an order has been transacted it generates a FillEvent,
    // which describes the cost of purchase or sale as well as the transaction costs, such as fees or slippage.
    // TODO here is an opportunity to make a slippage
    std::string timeindex; //The bar-resolution when the order was filled.
    std::string symbol; //The instrument which was filled.
    std::string exchange; //The exchange where the order was filled.
    int quantity ; //The filled quantity.
    std::string direction; //The direction of fill ('BUY' or 'SELL')
    int fill_cost; //The holdings value in ???? dollars ?????????
    int commission; //An optional commission sent from IB.
public:
    //TODO add commission(com) to constructor
    FillEvent(std::string time, std::string s, std::string ex, int q, std::string d, int full):
    timeindex(time), symbol(std::move(s)), exchange(std::move(ex)), quantity(q), direction(std::move(d)), fill_cost(full)
    {}
    //TODO define the way of calculating commission
    int calculate_ib_commission(int commission_) { // to calculate commission

    }
    std::string get_symbol() override {return symbol;}
    int get_quantity() override {return quantity;}
    std::string get_direction() override {return direction;}
    EventType type() const override {
        return EventType::FILL;
    }
};
