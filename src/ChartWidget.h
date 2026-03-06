#ifndef BACKTESTER_CPP_CHARTWIDGET_H
#define BACKTESTER_CPP_CHARTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>

class InteractiveChartView : public QChartView {
    Q_OBJECT

public:
    explicit InteractiveChartView(QChart* chart, QWidget* parent = nullptr);

};

class ChartWidget : public QWidget {
    Q_OBJECT

private:
    InteractiveChartView* chart_view;
    QChart* chart;
    QLineSeries* main_series;
    QLineSeries* short_ma_series;
    QLineSeries* long_ma_series;
    QValueAxis* axis_x;
    QValueAxis* axis_y;
    QLabel* tooltip_label;
    QLabel* metrics_label;

    bool is_price_chart;
    int data_point_counter;

public:
    explicit ChartWidget(const QString& title, bool price_chart = false, QWidget* parent = nullptr);
    ~ChartWidget();
};

#endif