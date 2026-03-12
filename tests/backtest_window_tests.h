#ifndef BACKTESTER_CPP_BACKTEST_WINDOW_TESTS_H
#define BACKTESTER_CPP_BACKTEST_WINDOW_TESTS_H

#include <gtest/gtest.h>

#include "BacktestWindow.h"

class BacktestWindowTest : public ::testing::Test {
protected:
    QPointer<BacktestWindow> window;

    void SetUp() override {
        window = new BacktestWindow();
    }

    void TearDown() override {
        delete window;
        window = nullptr;
    }
};

#endif