#include "select_strategy_screen_tests.h"

#include <QSignalSpy>
#include <QTest>

TEST_F(SelectStrategyScreenTest, SetStrategiesPopulatesComboBox) {
    ASSERT_NE(screen, nullptr);
    ASSERT_NE(screen->strategyCombo(), nullptr);

    const QList<QPair<QString, int> > strategies = {
        {"Moving Average", 10},
        {"Stop Loss", 20}
    };

    screen->setStrategies(strategies);

    EXPECT_EQ(screen->strategyCombo()->count(), 2);
    EXPECT_EQ(screen->strategyCombo()->itemText(0), "Moving Average");
    EXPECT_EQ(screen->strategyCombo()->itemData(0).toInt(), 10);
    EXPECT_EQ(screen->strategyCombo()->itemText(1), "Stop Loss");
    EXPECT_EQ(screen->strategyCombo()->itemData(1).toInt(), 20);
}

TEST_F(SelectStrategyScreenTest, SelectedStrategyMethodsReturnCurrentSelection) {
    ASSERT_NE(screen, nullptr);
    ASSERT_NE(screen->strategyCombo(), nullptr);

    screen->setStrategies({
        {"Moving Average", 10},
        {"Stop Loss", 20}
    });

    screen->strategyCombo()->setCurrentIndex(1);

    ASSERT_TRUE(screen->selectedStrategyId().has_value());
    EXPECT_EQ(screen->selectedStrategyId(), 20);
}

TEST_F(SelectStrategyScreenTest, EmptyListShowsFallback) {
    ASSERT_NE(screen, nullptr);
    ASSERT_NE(screen->strategyCombo(), nullptr);

    screen->setStrategies({});

    EXPECT_EQ(screen->strategyCombo()->count(), 1);
    EXPECT_EQ(screen->strategyCombo()->itemText(0), "No strategies available");
    EXPECT_FALSE(screen->selectedStrategyId().has_value());
}

TEST_F(SelectStrategyScreenTest, BackButtonEmitsStartScreenSwitch) {
    ASSERT_NE(screen, nullptr);
    ASSERT_NE(screen->backButton(), nullptr);

    QSignalSpy spy(screen.get(), &SelectStrategyScreen::StartScreenSwitch);
    ASSERT_TRUE(spy.isValid());

    QTest::mouseClick(screen->backButton(), Qt::LeftButton);

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(SelectStrategyScreenTest, StartBacktestButtonEmitsSignal) {
    ASSERT_NE(screen, nullptr);
    ASSERT_NE(screen->startBacktestButton(), nullptr);

    QSignalSpy spy(screen.get(), &SelectStrategyScreen::StartBacktestSwitch);
    ASSERT_TRUE(spy.isValid());

    QTest::mouseClick(screen->startBacktestButton(), Qt::LeftButton);

    EXPECT_EQ(spy.count(), 1);
}
