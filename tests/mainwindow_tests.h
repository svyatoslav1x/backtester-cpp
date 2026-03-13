#ifndef BACKTESTER_CPP_MAINWINDOW_TESTS_H
#define BACKTESTER_CPP_MAINWINDOW_TESTS_H
#include <gtest/gtest.h>
#include <QPointer>

#include "MainWindow.h"

class MainWindowTest : public ::testing::Test {
protected:
    std::unique_ptr<MainWindow> window;

    void SetUp() override {
        window = std::make_unique<MainWindow>();
    }

};

#endif //BACKTESTER_CPP_MAINWINDOW_TESTS_H
