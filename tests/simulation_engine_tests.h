#ifndef BACKTESTER_CPP_SIMULATION_ENGINE_TESTS_H
#define BACKTESTER_CPP_SIMULATION_ENGINE_TESTS_H

#include <gtest/gtest.h>

#include "SimulationEngine.h"
#include <QPointer>

class SimulationEngineTest : public ::testing::Test {
protected:
    std::unique_ptr<SimulationEngine> engine;

    void SetUp() override {
        engine = std::make_unique<SimulationEngine>();
    }
};

#endif