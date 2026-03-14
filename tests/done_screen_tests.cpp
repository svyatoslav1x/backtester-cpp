#include "done_screen_tests.h"

#include <QSignalSpy>
#include <QTest>

TEST_F(DoneScreenTest, SetResultsUpdatesText) {
    ASSERT_NE(screen, nullptr);

    const QString stats = "chemical by post malone is a great song";

    screen->setResults(stats);

    EXPECT_EQ(screen->resultsText(), stats);
}

TEST_F(DoneScreenTest, HomeButtonEmitsStartScreenSwitch) {
    ASSERT_NE(screen, nullptr);
    ASSERT_NE(screen->homeButton(), nullptr);

    QSignalSpy spy(screen.get(), &DoneScreen::StartScreenSwitch);
    ASSERT_TRUE(spy.isValid());

    QTest::mouseClick(screen->homeButton(), Qt::LeftButton);

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(DoneScreenTest, ChartsButtonEmitsBacktestScreenSwitch) {
    ASSERT_NE(screen, nullptr);
    ASSERT_NE(screen->chartsButton(), nullptr);

    QSignalSpy spy(screen.get(), &DoneScreen::BacktestScreenSwitch);
    ASSERT_TRUE(spy.isValid());

    QTest::mouseClick(screen->chartsButton(), Qt::LeftButton);

    EXPECT_EQ(spy.count(), 1);
}