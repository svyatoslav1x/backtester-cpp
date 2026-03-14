#ifndef BACKTESTER_CPP_STRATEGY_MANAGER_TESTS_H
#define BACKTESTER_CPP_STRATEGY_MANAGER_TESTS_H

#include <gtest/gtest.h>

#include "StrategyManager.h"

class StrategyManagerTest : public ::testing::Test {
protected:
    std::unique_ptr<StrategyManager> manager;

    void SetUp() override {
        manager = std::make_unique<StrategyManager>();
    }
};

#endif
