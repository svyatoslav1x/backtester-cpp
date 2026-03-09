#ifndef BACKTESTER_CPP_CHARTWIDGET_H
#define BACKTESTER_CPP_CHARTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>
#include <QtCharts/QScatterSeries>

class InteractiveChartView : public QChartView {
    Q_OBJECT

public:
    explicit InteractiveChartView(QChart* chart, QWidget* parent = nullptr);

protected:
    void wheelEvent(QWheelEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

signals:
    void pointHovered(const QPointF& point);
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
    QScatterSeries* buy_markers;
    QScatterSeries* sell_markers;

    std::vector<double> x_data;
    std::vector<double> y_data;
    std::vector<double> short_ma_data;
    std::vector<double> long_ma_data;

    bool is_price_chart;
    int data_point_counter;

    void update_metrics();
    QString format_point_info(const QPointF& point);

private slots:
    void on_point_hovered(const QPointF& point);

public:
    explicit ChartWidget(const QString& title, bool price_chart = false, QWidget* parent = nullptr);
    ~ChartWidget();

    void add_data_point(double x, double y);
    void add_ma_point(double x, double short_ma, double long_ma);
    void auto_scale();
    void add_signal_marker(double x, double y, bool is_buy);
};

#endif