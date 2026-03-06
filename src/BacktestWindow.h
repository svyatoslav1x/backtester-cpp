#ifndef BACKTESTER_CPP_BACKTESTWINDOW_H
#define BACKTESTER_CPP_BACKTESTWINDOW_H

#include <QWidget>

class ChartWidget;

class BacktestWindow : public QWidget {
    Q_OBJECT

public:
    explicit BacktestWindow(QWidget *parent = nullptr);
    ~BacktestWindow();

    void add_data_point(double x, double y);
    void add_ma_point(double x, double short_ma, double long_ma);
    void add_equity_point(double x, double y);

private:
    ChartWidget* equity_chart;
    ChartWidget* price_chart;

    double current_price;
    double current_equity;
};


#endif