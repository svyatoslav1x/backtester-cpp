#include "backtest_window_tests.h"
#include <QSignalSpy>
#include <QTest>

TEST_F(BacktestWindowTest, InitialStateIsCorrect) {
    ASSERT_NE(window, nullptr);

    EXPECT_EQ(window->pauseButton()->text(), "Pause");
    EXPECT_FALSE(window->showResultsButton()->isEnabled());
    EXPECT_EQ(window->progressLabel()->text(), "0%");
}

TEST_F(BacktestWindowTest, PauseButtonTogglesStateAndEmitsSignal) {
    ASSERT_NE(window, nullptr);

    QSignalSpy spy(window, &BacktestWindow::pauseToggled);
    ASSERT_TRUE(spy.isValid());

    QTest::mouseClick(window->pauseButton(), Qt::LeftButton);
    EXPECT_EQ(window->pauseButton()->text(), "Resume");
    EXPECT_EQ(spy.count(), 1);
    EXPECT_TRUE(spy.takeFirst().at(0).toBool());

    QTest::mouseClick(window->pauseButton(), Qt::LeftButton);
    EXPECT_EQ(window->pauseButton()->text(), "Pause");
}

TEST_F(BacktestWindowTest, BackButtonEmitsToSelectStrategyScreen) {
    ASSERT_NE(window, nullptr);

    QSignalSpy spy(window, &BacktestWindow::toSelectStrategyScreen);
    ASSERT_TRUE(spy.isValid());

    QTest::mouseClick(window->backButton(), Qt::LeftButton);
    EXPECT_EQ(spy.count(), 1);
}

TEST_F(BacktestWindowTest, ResetUIRestoresDefaults) {
    ASSERT_NE(window, nullptr);

    window->triggerPauseClicked();
    window->update_progress(50);

    window->resetUI();

    EXPECT_EQ(window->pauseButton()->text(), "Pause");
    EXPECT_FALSE(window->showResultsButton()->isEnabled());
    EXPECT_EQ(window->progressLabel()->text(), "0%");
}