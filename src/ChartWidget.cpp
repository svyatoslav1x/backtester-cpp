#include "ChartWidget.h"
#include <QVBoxLayout>

ChartWidget::ChartWidget(const QString& title, bool price_chart, QWidget* parent)
    : QWidget(parent), is_price_chart(price_chart), data_point_counter(0) {

    chart = new QChart();
    chart->setTitle(title);
    chart->setTitleFont(QFont("Arial", 14, QFont::Bold));
    chart->setAnimationOptions(QChart::NoAnimation);
    chart->setTheme(QChart::ChartThemeDark);

    main_series = new QLineSeries();
    main_series->setName(price_chart ? "Price" : "Equity");
    main_series->setPen(QPen(QColor(0, 188, 212), 2));
    chart->addSeries(main_series);

    if (is_price_chart) {
        short_ma_series = new QLineSeries();
        short_ma_series->setName("Short MA");
        short_ma_series->setPen(QPen(QColor(33, 150, 243), 2));
        chart->addSeries(short_ma_series);

        long_ma_series = new QLineSeries();
        long_ma_series->setName("Long MA");
        long_ma_series->setPen(QPen(QColor(244, 67, 54), 2));
        chart->addSeries(long_ma_series);
    } else {
        short_ma_series = nullptr;
        long_ma_series = nullptr;
    }

    axis_x = new QValueAxis();
    axis_x->setTitleText("Time");
    axis_x->setLabelFormat("%d");
    axis_x->setTitleFont(QFont("Arial", 10, QFont::Bold));

    axis_y = new QValueAxis();
    axis_y->setTitleText(price_chart ? "Price" : "Portfolio Value");
    axis_y->setLabelFormat("%.2f");
    axis_y->setTitleFont(QFont("Arial", 10, QFont::Bold));

    chart->addAxis(axis_x, Qt::AlignBottom);
    chart->addAxis(axis_y, Qt::AlignLeft);

    main_series->attachAxis(axis_x);
    main_series->attachAxis(axis_y);

    if (is_price_chart) {
        short_ma_series->attachAxis(axis_x);
        short_ma_series->attachAxis(axis_y);
        long_ma_series->attachAxis(axis_x);
        long_ma_series->attachAxis(axis_y);
    }

    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setFont(QFont("Arial", 9));

    chart_view = new InteractiveChartView(chart);
    chart_view->setRenderHint(QPainter::Antialiasing);

    tooltip_label = new QLabel(this);
    tooltip_label->setStyleSheet(
        "QLabel { background-color: rgba(0, 0, 0, 180); color: white; "
        "border: 1px solid white; border-radius: 5px; padding: 5px; "
        "font-size: 10pt; }"
    );
    tooltip_label->hide();
    tooltip_label->setAttribute(Qt::WA_TransparentForMouseEvents);

    metrics_label = new QLabel(this);
    metrics_label->setStyleSheet(
        "QLabel { background-color: rgba(0, 0, 0, 0); "
        "border: 2px solid #2196F3; border-radius: 8px; padding: 10px; "
        "font-size: 9pt; font-weight: bold; }"
    );
    metrics_label->setText("Metrics: N/A");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(metrics_label);
    layout->addWidget(chart_view);
    layout->setContentsMargins(5, 5, 5, 5);
    setLayout(layout);
}

InteractiveChartView::InteractiveChartView(QChart* chart, QWidget* parent)
    : QChartView(chart, parent) {
    setRubberBand(QChartView::RectangleRubberBand);
}

ChartWidget::~ChartWidget() {}