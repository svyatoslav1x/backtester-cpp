#ifndef BACKTESTER_CPP_START_SCREEN_TESTS_H
#define BACKTESTER_CPP_START_SCREEN_TESTS_H


#include <gtest/gtest.h>
#include <QPointer>

#include "start_screen.h"

class StartScreenTest : public ::testing::Test {
protected:
    std::unique_ptr<StartScreen> screen;

    void SetUp() override {
        screen = std::make_unique<StartScreen>();
    }
};

#endif //BACKTESTER_CPP_START_SCREEN_TESTS_H
