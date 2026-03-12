#include "chart_widget_tests.h"

TEST_F(ChartWidgetTest, ChartInitialization) {
    ASSERT_NE(chart, nullptr);
    EXPECT_TRUE(chart->getIsPriceChart());
    EXPECT_NE(chart->getMainSeries(), nullptr);
    EXPECT_NE(chart->getShortMaSeries(), nullptr);
    EXPECT_NE(chart->getLongMaSeries(), nullptr);
}

TEST_F(ChartWidgetTest, AddDataPointUpdatesMetrics) {
    ASSERT_NE(chart, nullptr);
    chart->add_data_point(1.0, 100.0);
    chart->add_data_point(2.0, 105.0);

    EXPECT_EQ(chart->getMainSeries()->count(), 2);
    EXPECT_NE(chart->getMetricsLabel()->text(), "Metrics: N/A");
    EXPECT_TRUE(chart->getMetricsLabel()->text().contains("$105.00"));
}

TEST_F(ChartWidgetTest, ClearChartResetsData) {
    ASSERT_NE(chart, nullptr);
    chart->add_data_point(1.0, 100.0);
    chart->clearChart();

    EXPECT_EQ(chart->getMainSeries()->count(), 0);
    EXPECT_EQ(chart->getMetricsLabel()->text(), "Metrics: N/A");
}