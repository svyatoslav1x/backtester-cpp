#ifndef BACKTESTER_CPP_BACKTEST_WINDOW_TESTS_H
#define BACKTESTER_CPP_BACKTEST_WINDOW_TESTS_H

#include <gtest/gtest.h>

#include "BacktestWindow.h"

class BacktestWindowTest : public ::testing::Test {
protected:
    std::unique_ptr<BacktestWindow> window;

    void SetUp() override {
        window = std::make_unique<BacktestWindow>();
    }
};

#endif