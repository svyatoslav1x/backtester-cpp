#ifndef BACKTESTER_CPP_CREATE_STRATEGY_SCREEN_TESTS_H
#define BACKTESTER_CPP_CREATE_STRATEGY_SCREEN_TESTS_H

#include <gtest/gtest.h>

#include "create_strategy_screen.h"

class CreateStrategyScreenTest : public ::testing::Test {
protected:
    std::unique_ptr<CreateStrategyScreen> screen;

    void SetUp() override {
        screen = std::make_unique<CreateStrategyScreen>();
    }
};


#endif //BACKTESTER_CPP_CREATE_STRATEGY_SCREEN_TESTS_H
