#ifndef BACKTESTER_CPP_SELECT_STRATEGY_SCREEN_TESTS_H
#define BACKTESTER_CPP_SELECT_STRATEGY_SCREEN_TESTS_H

#include <gtest/gtest.h>

#include "select_strategy_screen.h"

class SelectStrategyScreenTest : public ::testing::Test {
protected:
    std::unique_ptr<SelectStrategyScreen> screen;

    void SetUp() override {
        screen = std::make_unique<SelectStrategyScreen>();
    }
};


#endif //BACKTESTER_CPP_SELECT_STRATEGY_SCREEN_TESTS_H