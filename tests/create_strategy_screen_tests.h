#ifndef BACKTESTER_CPP_CREATE_STRATEGY_SCREEN_TESTS_H
#define BACKTESTER_CPP_CREATE_STRATEGY_SCREEN_TESTS_H

#include <gtest/gtest.h>

#include "create_strategy_screen.h"

class CreateStrategyScreenTest : public ::testing::Test {
protected:
    QPointer<CreateStrategyScreen> screen;

    void SetUp() override {
        screen = new CreateStrategyScreen();
    }

    void TearDown() override {
        delete screen;
        screen = nullptr;
    }
};


#endif //BACKTESTER_CPP_CREATE_STRATEGY_SCREEN_TESTS_H
