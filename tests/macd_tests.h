#pragma once

#include <gtest/gtest.h>

#include "macd.h"
#include "data.h"

#include <queue>
#include <memory>

// Fake DataHandler
class FakeDataHandler : public DataHandler {
    std::unordered_map<std::string,std::vector<Bar>> data;
public:

    FakeDataHandler() {
        //std::cout << "[DEBUG] Initializing FakeDataHandler" << std::endl;
        data["AAPL"] = {};
        data["MSFT"] = {};
    }

    std::vector<std::string> get_symbols() const override {
        return {"AAPL","MSFT"};
    }

    std::vector<Bar> get_latest_bars(const std::string& symbol, std::size_t N = 1) const override
    {
        //std::cout<< "[DEBUG] Getting latest " << N << " bars for symbol " << symbol << std::endl;
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
        /*for (auto bar : data.get_latest_bars("AAPL", 10)) {
        std::cout << "[DEBUG] Initial bar for AAPL: " << bar.close << " at " << std::chrono::system_clock::to_time_t(bar.datetime) << std::endl;
    }*/
        //std::cout << "[DEBUG] Pushing price " << price << " for symbol " << symbol << std::endl;
        Bar b{};
        b.close = price;
        b.datetime = std::chrono::system_clock::now();
        b.open = price;
        b.high = price;
        b.low = price;
        b.volume = 100;
        //std::cout << "[DEBUG] Bar created with close price " << b.close << " and datetime " << std::chrono::system_clock::to_time_t(b.datetime) << std::endl;

        data[symbol].push_back(b);
        //std::cout << "[DEBUG] Bar pushed. Total bars for " << symbol << ": " << data[symbol].size() << std::endl;
    }

    void update_bars() override {}

    bool if_continue_backtest() const override {
        return true;
    }
};

//Fake Portfolio
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

    void update_timeindex(const Event&) override {}
    void update_signal(const Event&) override {}
    void update_fill(const Event&) override {}
};


//Test Fixture
class MACDTest : public ::testing::Test {
protected:

    FakeDataHandler data;
    FakePortfolio portfolio;

    std::queue<std::unique_ptr<Event>> events;

    std::unique_ptr<MovingAveragesLongStrategy> strategy;

    MarketEvent market_event;

    void SetUp() override {
        strategy = std::make_unique<MovingAveragesLongStrategy>(
            data,
            events,
            portfolio,
            3,
            5
        );
    }

    void TearDown() override {
        while (!events.empty())
            events.pop();
    }
};