#ifndef BACKTESTER_CPP_DONE_SCREEN_TESTS_H
#define BACKTESTER_CPP_DONE_SCREEN_TESTS_H

#include <gtest/gtest.h>

#include "done_screen.h"

class DoneScreenTest : public ::testing::Test {
protected:
    std::unique_ptr<DoneScreen> screen;

    void SetUp() override {
        screen = std::make_unique<DoneScreen>();
    }
};


#endif //BACKTESTER_CPP_DONE_SCREEN_TESTS_H
