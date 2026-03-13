#include "stop_loss_tests.h"


class FakeDataHandler : public DataHandler {
public:
    std::vector<std::string> symbols = {"AAPL"};
    std::vector<Bar> bars;

    std::vector<std::string> get_symbols() const override {
        return symbols;
    }
    void update_bars() override {};
    bool if_continue_backtest() const override {return true;};
    virtual std::vector<Bar> get_latest_bars(const std::string& symbol, std::size_t N = 1) const override {
        if (bars.size() < N)
            return bars;
        return {bars.end() - N, bars.end()};
    }

    void push_price(double price) {
        Bar b{};
        b.close = price;
        b.datetime = std::chrono::system_clock::now();
        bars.push_back(b);
    }
};

class FakePortfolio : public Portfolio {
public:
    double cash = 1000;
    int position = 0;

    double get_cash() const override {
        return cash;
    }

    int get_position(const std::string&) const override {
        return position;
    }
    virtual void update_timeindex(const Event& event) override {};
	virtual void update_signal(const Event& event) override {};
	virtual void update_fill(const Event& event) override {};
};