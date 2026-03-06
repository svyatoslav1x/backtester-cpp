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

void ChartWidget::add_data_point(double x, double y) {
    x_data.push_back(x);
    y_data.push_back(y);

    main_series->append(x, y);

    if (x_data.size() > 1) {
        auto_scale();
    }

    update_metrics();
}

void ChartWidget::auto_scale() {
    if (x_data.empty() || y_data.empty()) return;

    double x_min = *std::min_element(x_data.begin(), x_data.end());
    double x_max = *std::max_element(x_data.begin(), x_data.end());
    double y_min = *std::min_element(y_data.begin(), y_data.end());
    double y_max = *std::max_element(y_data.begin(), y_data.end());

    double x_padding = (x_max - x_min) * 0.05;
    double y_padding = (y_max - y_min) * 0.1;

    if (x_padding == 0) x_padding = 1;
    if (y_padding == 0) y_padding = 1;

    axis_x->setRange(x_min - x_padding, x_max + x_padding);
    axis_y->setRange(y_min - y_padding, y_max + y_padding);
}

void ChartWidget::update_metrics() {
    if (y_data.empty()) {
        metrics_label->setText("Metrics: No data");
        return;
    }

    double current_value = y_data.back();
    double min_value = *std::min_element(y_data.begin(), y_data.end());
    double max_value = *std::max_element(y_data.begin(), y_data.end());
    double avg_value = 0.0;
    for (double val : y_data) {
        avg_value += val;
    }
    avg_value /= y_data.size();

    double variance = 0.0;
    for (double val : y_data) {
        variance += (val - avg_value) * (val - avg_value);
    }
    double volatility = std::sqrt(variance / y_data.size());

    double change = 0.0;
    if (y_data.size() > 1 && y_data.front() != 0) {
        change = ((current_value - y_data.front()) / y_data.front()) * 100.0;
    }

    QString metrics_text;
    if (is_price_chart) {
        metrics_text = QString(
            "Current: $%1 | Min: $%2 | Max: $%3 | Avg: $%4 | Vol: $%5 | Change: %6%"
        ).arg(current_value, 0, 'f', 2)
         .arg(min_value, 0, 'f', 2)
         .arg(max_value, 0, 'f', 2)
         .arg(avg_value, 0, 'f', 2)
         .arg(volatility, 0, 'f', 2)
         .arg(change, 0, 'f', 2);
    } else {
        metrics_text = QString(
            "Portfolio: $%1 | Min: $%2 | Max: $%3 | Avg: $%4 | ROI: %5%"
        ).arg(current_value, 0, 'f', 2)
         .arg(min_value, 0, 'f', 2)
         .arg(max_value, 0, 'f', 2)
         .arg(avg_value, 0, 'f', 2)
         .arg(change, 0, 'f', 2);
    }

    metrics_label->setText(metrics_text);
}