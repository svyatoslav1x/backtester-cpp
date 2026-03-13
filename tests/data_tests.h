#ifndef BACKTESTER_CPP_DATA_TESTS_H
#define BACKTESTER_CPP_DATA_TESTS_H

#include <gtest/gtest.h>
#include "data.h"

class FakeDataHandler : public DataHandler {
public:

    std::vector<std::string> symbols = {"AAPL", "MSFT"};

    std::vector<Bar> get_latest_bars(const std::string&, size_t N) const override {
        std::vector<Bar> bars;
        int i = 10;

        for (int i = 0; i < 10; ++i) {
        bars.emplace_back(Bar{
            std::chrono::system_clock::now(),
            100.0,
            105.0,
            95.0,
            102.0,
            1000.0});
        }
        return bars;
    }

    void update_bars() override {}

    bool if_continue_backtest() const override {
        return true;
    }

    std::vector<std::string> get_symbols() const override {
        return symbols;
    }
};

class DataHandlerTest : public ::testing::Test {
protected:

    std::unique_ptr<DataHandler> data;

    void SetUp() override {
        data = std::make_unique<FakeDataHandler>();
    }

    void TearDown() override {
        data.reset();
    }
};

#endif