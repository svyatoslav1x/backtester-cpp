#include "mainwindow_tests.h"

TEST_F(MainWindowTest, CreatesMainScreens) {
    ASSERT_NE(window, nullptr);

    EXPECT_NE(window->startScreen(), nullptr);
    EXPECT_NE(window->doneScreen(), nullptr);
    EXPECT_NE(window->selectStrategyScreen(), nullptr);
    EXPECT_NE(window->createStrategyScreen(), nullptr);
    EXPECT_NE(window->stackedWidget(), nullptr);
}

TEST_F(MainWindowTest, StartsOnStartScreen) {
    ASSERT_NE(window, nullptr);
    ASSERT_NE(window->stackedWidget(), nullptr);
    ASSERT_NE(window->startScreen(), nullptr);

    EXPECT_EQ(window->stackedWidget()->currentWidget(), window->startScreen());
}