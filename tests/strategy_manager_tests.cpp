#include "strategy_manager_tests.h"

#include <QSignalSpy>
#include <QTest>

TEST_F(StrategyManagerTest, BackButtonEmitsSignal) {
    ASSERT_NE(manager->backButton(), nullptr);
    QSignalSpy spy(manager, &StrategyManager::startScreenSwitch);

    QTest::mouseClick(manager->backButton(), Qt::LeftButton);

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(StrategyManagerTest, ListWidgetIsInitialized) {
    ASSERT_NE(manager->strategyList(), nullptr);
    EXPECT_EQ(manager->strategyList()->count(), 0);
}