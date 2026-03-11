#ifndef BACKTESTER_CPP_MAINWINDOW_TESTS_H
#define BACKTESTER_CPP_MAINWINDOW_TESTS_H
#include <gtest/gtest.h>
#include <QPointer>

#include "MainWindow.h"

class MainWindowTest : public ::testing::Test {
protected:
    QPointer<MainWindow> window;

    void SetUp() override {
        window = new MainWindow();
    }

    void TearDown() override {
        delete window;
        window = nullptr;
    }
};

#endif //BACKTESTER_CPP_MAINWINDOW_TESTS_H
