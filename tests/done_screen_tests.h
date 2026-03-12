#ifndef BACKTESTER_CPP_DONE_SCREEN_TESTS_H
#define BACKTESTER_CPP_DONE_SCREEN_TESTS_H

#include <gtest/gtest.h>

#include "done_screen.h"

class DoneScreenTest : public ::testing::Test {
protected:
    QPointer<DoneScreen> screen;

    void SetUp() override {
        screen = new DoneScreen();
    }

    void TearDown() override {
        delete screen;
        screen = nullptr;
    }
};


#endif //BACKTESTER_CPP_DONE_SCREEN_TESTS_H