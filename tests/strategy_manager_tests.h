#ifndef BACKTESTER_CPP_STRATEGY_MANAGER_TESTS_H
#define BACKTESTER_CPP_STRATEGY_MANAGER_TESTS_H

#include <gtest/gtest.h>

#include "StrategyManager.h"

class StrategyManagerTest : public ::testing::Test {
protected:
    QPointer<StrategyManager> manager;

    void SetUp() override {
        manager = new StrategyManager();
    }

    void TearDown() override {
        delete manager;
        manager = nullptr;
    }
};

#endif