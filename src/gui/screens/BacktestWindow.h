#ifndef BACKTESTER_CPP_BACKTESTWINDOW_H
#define BACKTESTER_CPP_BACKTESTWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include <QPointer>

class ChartWidget;

class BacktestWindow : public QWidget {
    Q_OBJECT

public:
    explicit BacktestWindow(QWidget *parent = nullptr);
    ~BacktestWindow();

    void add_data_point(double x, double y);
    void add_ma_point(double x, double short_ma, double long_ma);
    void add_equity_point(double x, double y);

    void set_simulation_finished(const QString& stats);
    void add_signal_marker(double x, double y, bool is_buy);

signals:
    void pauseToggled(bool paused);

private slots:
    void on_pause_clicked();
    void on_show_results_clicked();
    void on_back_to_charts_clicked();

private:
    void create_done_screen();

    QPointer<QStackedWidget> stacked_widget;
    QPointer<QWidget> charts_view;
    QPointer<QWidget> done_screen;

    QPointer<ChartWidget> equity_chart;
    QPointer<ChartWidget> price_chart;

    QPointer<QPushButton> pause_button;
    QPointer<QPushButton> show_results_button;
    QPointer<QPushButton> back_to_charts_button;
    QPointer<QLabel> results_label;

    double current_price;
    double current_equity;

    bool is_paused = false;
};

#endif