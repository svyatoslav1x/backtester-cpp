#pragma once

#include <gtest/gtest.h>

#include "stop_loss.h"
#include "data.h"

#include <queue>
#include <memory>

// Fake DataHandler
class StopLoss_FakeDataHandler : public DataHandler {
    std::unordered_map<std::string,std::vector<Bar>> data;
public:

    StopLoss_FakeDataHandler() {
        data["AAPL"] = {};
        data["MSFT"] = {};
    }

    std::vector<std::string> get_symbols() const override {
        return {"AAPL","MSFT"};
    }

    std::vector<Bar> get_latest_bars(const std::string& symbol, std::size_t N = 1) const override
    {
        auto it = data.find(symbol);

        if(it == data.end())
            return {};

        const auto& bars = it->second;

        if(bars.empty())
            return {};

        if(bars.size() <= N)
            return bars;

        return std::vector<Bar>(bars.end()-N,bars.end());
    }

    void push_price(const std::string& symbol,double price)
    {
        Bar b{};
        b.close = price;
        b.datetime = std::chrono::system_clock::now();
        b.open = price;
        b.high = price;
        b.low = price;
        b.volume = 100;

        data[symbol].push_back(b);
    }

    void update_bars() override {}

    bool if_continue_backtest() const override {
        return true;
    }
};

//Fake Portfolio
class StopLoss_FakePortfolio : public Portfolio {
public:
    double cash = 1000;
    int position = 0;

    double get_cash() const override {
        return cash;
    }

    int get_position(const std::string&) const override {
        return position;
    }

    void update_timeindex(const Event&) override {}
    void update_signal(const Event&) override {}
    void update_fill(const Event&) override {}
};


//Test Fixture
class LossTest : public ::testing::Test {
protected:

    StopLoss_FakeDataHandler data;
    StopLoss_FakePortfolio portfolio;

    std::queue<std::unique_ptr<Event>> events;

    std::unique_ptr<StopLossStrategy> strategy;

    MarketEvent market_event;

    void SetUp() override {
        strategy = std::make_unique<StopLossStrategy>(
            data,
            events,
            portfolio,
            0.9
        );
    }

    void TearDown() override {
        while (!events.empty())
            events.pop();
    }
};