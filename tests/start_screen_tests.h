#ifndef BACKTESTER_CPP_START_SCREEN_TESTS_H
#define BACKTESTER_CPP_START_SCREEN_TESTS_H


#include <gtest/gtest.h>
#include <QPointer>

#include "start_screen.h"

class StartScreenTest : public ::testing::Test {
protected:
    QPointer<StartScreen> screen;

    void SetUp() override {
        screen = new StartScreen();
    }

    void TearDown() override {
        delete screen;
        screen = nullptr;
    }
};

#endif //BACKTESTER_CPP_START_SCREEN_TESTS_H
