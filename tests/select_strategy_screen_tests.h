#ifndef BACKTESTER_CPP_SELECT_STRATEGY_SCREEN_TESTS_H
#define BACKTESTER_CPP_SELECT_STRATEGY_SCREEN_TESTS_H

#include <gtest/gtest.h>

#include "select_strategy_screen.h"

class SelectStrategyScreenTest : public ::testing::Test {
protected:
    QPointer<SelectStrategyScreen> screen;

    void SetUp() override {
        screen = new SelectStrategyScreen();
    }

    void TearDown() override {
        delete screen;
        screen = nullptr;
    }
};


#endif //BACKTESTER_CPP_SELECT_STRATEGY_SCREEN_TESTS_H