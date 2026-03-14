#ifndef BACKTESTER_CPP_CHARTWIDGET_H
#define BACKTESTER_CPP_CHARTWIDGET_H

#include <QLabel>
#include <QPointer>
#include <QVBoxLayout>
#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>

// QChartView subclass to enable mouse hovering and mouse wheel zooming
class InteractiveChartView : public QChartView {
	Q_OBJECT
public:
	explicit InteractiveChartView(QChart* chart, QWidget* parent = nullptr);
protected:
	// override default mouse/wheel events to emit custom signals
	void wheelEvent(QWheelEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void leaveEvent(QEvent* event) override;

signals:
	void pointHovered(const QPointF& point);
	void mouseLeft();
};

// ChartWidget acts as a wrapper for QChart, managing all series (lines/scatters),
// auto-scaling axes, and dynamically generated metrics/tooltips.
class ChartWidget : public QWidget {
	Q_OBJECT
private:
	QPointer<InteractiveChartView> chart_view;
	QPointer<QChart> chart;
	QPointer<QLineSeries> main_series;
	QPointer<QLineSeries> short_ma_series;
	QPointer<QLineSeries> long_ma_series;
	QPointer<QValueAxis> axis_x;
	QPointer<QValueAxis> axis_y;
	QPointer<QLabel> tooltip_label; // hover pop-up box
	QPointer<QLabel> metrics_label; // Box tracking volatility, min/max, etc.
	QPointer<QScatterSeries> buy_markers; // green triangles (buy)
	QPointer<QScatterSeries> sell_markers; // red triangles (sell)
	QPointer<QVBoxLayout> layout;

	std::vector<double> x_data;
	std::vector<double> y_data;
	std::vector<double> short_ma_data;
	std::vector<double> long_ma_data;

	bool is_price_chart; // Asset Price or Portfolio Value
	int data_point_counter;

	// calculates and updates the metrics_label based on y_data
	void update_metrics();

	// locates the nearest point to the user's cursor
	QString format_point_info(const QPointF& point);

private slots:
	void on_point_hovered(const QPointF& point);
	void on_mouse_left();
public:
	explicit ChartWidget(const QString& title, bool price_chart = false, QWidget* parent = nullptr);

	~ChartWidget();

	void add_data_point(double x, double y);
	void add_ma_point(double x, double short_ma, double long_ma);
	void auto_scale();
	void add_signal_marker(double x, double y, bool is_buy);
	void clearChart();

	// for tests
	bool getIsPriceChart() const { return is_price_chart; }
	QPointer<QLineSeries> getMainSeries() const { return main_series; }
	QPointer<QLineSeries> getShortMaSeries() const { return short_ma_series; }
	QPointer<QLineSeries> getLongMaSeries() const { return long_ma_series; }
	QPointer<QScatterSeries> getBuyMarkers() const { return buy_markers; }
	QPointer<QLabel> getMetricsLabel() const { return metrics_label; }
};

#endif
