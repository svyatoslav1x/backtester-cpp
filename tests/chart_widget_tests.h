#ifndef BACKTESTER_CPP_CHART_WIDGET_TESTS_H
#define BACKTESTER_CPP_CHART_WIDGET_TESTS_H

#include <gtest/gtest.h>

#include "ChartWidget.h"

class ChartWidgetTest : public ::testing::Test {
protected:
    QPointer<ChartWidget> chart;

    void SetUp() override {
        chart = new ChartWidget("Asset Price", true);
    }

    void TearDown() override {
        delete chart;
        chart = nullptr;
    }
};

#endif