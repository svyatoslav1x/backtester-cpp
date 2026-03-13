#include "simulation_engine_tests.h"
#include <QSignalSpy>

TEST_F(SimulationEngineTest, InitialStatePreventsSimulation) {
    ASSERT_NE(engine, nullptr);

    EXPECT_FALSE(engine->isFinished());
    
    QSignalSpy progressSpy(engine, &SimulationEngine::progressUpdated);
    
    engine->startSimulation();

    EXPECT_EQ(progressSpy.count(), 0);
}

TEST_F(SimulationEngineTest, SetPausedTogglesTimer) {
    ASSERT_NE(engine, nullptr);

    engine->setPaused(true);
    EXPECT_FALSE(engine->isTimerActive());
}

TEST_F(SimulationEngineTest, StopTerminatesEngine) {
    ASSERT_NE(engine, nullptr);
    
    engine->stop();
    EXPECT_TRUE(engine->isFinished());
    EXPECT_FALSE(engine->isTimerActive());
}