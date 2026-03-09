#ifndef BACKTESTER_CPP_BACKTESTWINDOW_H
#define BACKTESTER_CPP_BACKTESTWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class ChartWidget;

class BacktestWindow : public QWidget {
    Q_OBJECT

public:
    explicit BacktestWindow(QWidget *parent = nullptr);

    ~BacktestWindow();

    void add_data_point(double x, double y);

    void add_ma_point(double x, double short_ma, double long_ma);

    void add_equity_point(double x, double y);

    void set_simulation_finished(const QString &stats);

    void add_signal_marker(double x, double y, bool is_buy);

signals:
    void pauseToggled(bool paused);

private slots:
    void on_pause_clicked();

private:
    QWidget *charts_view;

    ChartWidget *equity_chart;
    ChartWidget *price_chart;

    QPushButton *pause_button;
    QPushButton *show_results_button;
    QPushButton *back_to_charts_button;
    QLabel *results_label;

    double current_price;
    double current_equity;

    bool is_paused = false;
};


#endif
