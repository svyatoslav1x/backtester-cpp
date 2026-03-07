#include "BacktestWindow.h"
#include "ChartWidget.h"
#include <QHBoxLayout>
#include <QSplitter>

BacktestWindow::BacktestWindow(QWidget *parent)
    : QWidget(parent) {
    QVBoxLayout* main_layout = new QVBoxLayout(this);
    stacked_widget = new QStackedWidget(this);

    charts_view = new QWidget();
    QVBoxLayout* charts_layout = new QVBoxLayout(charts_view);

    QSplitter* splitter = new QSplitter(Qt::Vertical);
    equity_chart = new ChartWidget("Portfolio Value", false);
    price_chart = new ChartWidget("Asset Price", true);

    splitter->addWidget(equity_chart);
    splitter->addWidget(price_chart);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    QHBoxLayout* bottom_buttons_layout = new QHBoxLayout();

    pause_button = new QPushButton("Pause");
    pause_button->setMinimumSize(100, 40);
    pause_button->setStyleSheet("font-weight: bold; background-color: #333; color: white;");
    connect(pause_button, &QPushButton::clicked, this, &BacktestWindow::on_pause_clicked);

    show_results_button = new QPushButton("Show Results");
    show_results_button->setMinimumSize(120, 30);
    show_results_button->setEnabled(false);
    connect(show_results_button, &QPushButton::clicked, this, &BacktestWindow::on_show_results_clicked);

    bottom_buttons_layout->addWidget(pause_button);
    bottom_buttons_layout->addWidget(show_results_button);
    bottom_buttons_layout->addStretch();

    charts_layout->addWidget(splitter);
    charts_layout->addLayout(bottom_buttons_layout);

    create_done_screen();

    stacked_widget->addWidget(charts_view);
    stacked_widget->addWidget(done_screen);
    main_layout->addWidget(stacked_widget);
}

BacktestWindow::~BacktestWindow() {}

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

void BacktestWindow::create_done_screen() {
    done_screen = new QWidget();
    done_screen->setStyleSheet(
        "QWidget { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #1a1a2e, stop:1 #16213e); }"
    );

    QVBoxLayout* layout = new QVBoxLayout(done_screen);
    layout->setContentsMargins(60, 60, 60, 60);
    layout->setAlignment(Qt::AlignCenter);

    QLabel* title = new QLabel("<h1 style='color: #4CAF50;'>Backtest Complete!</h1>");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("background: transparent; font-size: 28pt; font-weight: bold;");
    layout->addWidget(title);

    layout->addSpacing(30);

    results_label = new QLabel();
    results_label->setStyleSheet(
        "background-color: #0f3460; color: #eee; font-size: 14pt; "
        "border: 2px solid #4CAF50; border-radius: 10px; padding: 30px;"
    );
    results_label->setAlignment(Qt::AlignCenter);
    layout->addWidget(results_label);

    layout->addSpacing(30);

    QHBoxLayout* button_layout = new QHBoxLayout();
    button_layout->setSpacing(20);

    back_to_charts_button = new QPushButton("Back to Charts");
    back_to_charts_button->setMinimumSize(200, 50);
    back_to_charts_button->setStyleSheet(
        "QPushButton { background: #F57C00; "
        "color: white; border: none; border-radius: 10px; font-size: 14pt; font-weight: bold; }"
        "QPushButton:hover { background: #FF9800; }"
        "QPushButton:pressed { background: #E65100; }"
    );
    connect(back_to_charts_button, &QPushButton::clicked, this, &BacktestWindow::on_back_to_charts_clicked);
    button_layout->addWidget(back_to_charts_button);

    layout->addLayout(button_layout);

    layout->addStretch();
}

void BacktestWindow::on_show_results_clicked() {
    stacked_widget->setCurrentIndex(1);
}

void BacktestWindow::on_back_to_charts_clicked() {
    stacked_widget->setCurrentIndex(0);
}

void BacktestWindow::set_simulation_finished(const QString& stats) {
    results_label->setText(stats);

    show_results_button->setEnabled(true);

    show_results_button->setStyleSheet(
        "background-color: #4CAF50; color: white; font-weight: bold; border-radius: 5px;"
    );
}

void BacktestWindow::add_signal_marker(double x, double y, bool is_buy) {
    price_chart->add_signal_marker(x, y, is_buy);
}
