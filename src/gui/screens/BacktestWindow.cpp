#include "BacktestWindow.h"
#include "ChartWidget.h"
#include <QHBoxLayout>
#include <QSplitter>

BacktestWindow::BacktestWindow(QWidget *parent)
    : QWidget(parent) {
    QVBoxLayout *main_layout = new QVBoxLayout(this);

    charts_view = new QWidget();
    QVBoxLayout *charts_layout = new QVBoxLayout(charts_view);

    QSplitter *splitter = new QSplitter(Qt::Vertical);
    equity_chart = new ChartWidget("Portfolio Value", false);
    price_chart = new ChartWidget("Asset Price", true);

    splitter->addWidget(equity_chart);
    splitter->addWidget(price_chart);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    QHBoxLayout *bottom_buttons_layout = new QHBoxLayout();

    pause_button = new QPushButton("Pause");
    pause_button->setMinimumSize(100, 40);
    pause_button->setStyleSheet("font-weight: bold; background-color: #333; color: white;");

    bottom_buttons_layout->addStretch();

    show_results_button = new QPushButton("Show Results");
    show_results_button->setMinimumSize(120, 30);
    show_results_button->setEnabled(false);

    bottom_buttons_layout->addWidget(pause_button);
    bottom_buttons_layout->addWidget(show_results_button);
    bottom_buttons_layout->addStretch();

    charts_layout->addWidget(splitter);
    charts_layout->addLayout(bottom_buttons_layout);

    main_layout->addWidget(charts_view);
}

BacktestWindow::~BacktestWindow() {
}

void BacktestWindow::add_data_point(double x, double y) {
    price_chart->add_data_point(x, y);
}

void BacktestWindow::add_ma_point(double x, double short_ma, double long_ma) {
    price_chart->add_ma_point(x, short_ma, long_ma);
}

void BacktestWindow::add_equity_point(double x, double y) {
    equity_chart->add_data_point(x, y);
}

void BacktestWindow::on_pause_clicked() {
    is_paused = !is_paused;

    if (is_paused) {
        pause_button->setText("Resume");
        pause_button->setStyleSheet("font-weight: bold; background-color: #2e7d32; color: white;");
    } else {
        pause_button->setText("Pause");
        pause_button->setStyleSheet("font-weight: bold; background-color: #333; color: white;");
    }

    emit pauseToggled(is_paused);
}

void BacktestWindow::set_simulation_finished(const QString &stats) {
    results_label->setText(stats);

    show_results_button->setEnabled(true);

    show_results_button->setStyleSheet(
        "background-color: #4CAF50; color: white; font-weight: bold; border-radius: 5px;"
    );
}

void BacktestWindow::add_signal_marker(double x, double y, bool is_buy) {
    price_chart->add_signal_marker(x, y, is_buy);
}
