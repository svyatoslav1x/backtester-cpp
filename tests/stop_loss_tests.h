#ifndef BACKTESTER_CPP_MACD_TESTS_H
#define BACKTESTER_CPP_MACD_TESTS_H

#include <gtest/gtest.h>
#include <queue>
#include <memory>

#include "stop_loss.h"

class FakeDataHandler;
class FakePortfolio;

class MACDStrategyTest : public ::testing::Test {
protected:
    std::unique_ptr<FakeDataHandler> data;
    std::unique_ptr<FakePortfolio> portfolio;
    std::queue<std::unique_ptr<Event>> events;

    std::unique_ptr<StopLossStrategy> strategy;

    void SetUp() override;
    void TearDown() override;
};

#endif