#ifndef BACKTESTER_CPP_CHART_WIDGET_TESTS_H
#define BACKTESTER_CPP_CHART_WIDGET_TESTS_H

#include <gtest/gtest.h>

#include "ChartWidget.h"

class ChartWidgetTest : public ::testing::Test {
protected:
    std::unique_ptr<ChartWidget> chart;

    void SetUp() override {
        chart = std::make_unique<ChartWidget>("Asset Price", true);
    }
};

#endif