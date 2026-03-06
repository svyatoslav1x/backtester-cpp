#include "BacktestWindow.h"
#include "ChartWidget.h"
#include <QHBoxLayout>
#include <QSplitter>

BacktestWindow::BacktestWindow(QWidget *parent)
    : QWidget(parent) {
    QVBoxLayout* main_layout = new QVBoxLayout(this);
    QSplitter* splitter = new QSplitter(Qt::Vertical);

    equity_chart = new ChartWidget("Portfolio Value", false);
    price_chart = new ChartWidget("Asset Price", true);

    splitter->addWidget(equity_chart);
    splitter->addWidget(price_chart);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);
    main_layout->addWidget(splitter, 3);
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

BacktestWindow::~BacktestWindow() {}
