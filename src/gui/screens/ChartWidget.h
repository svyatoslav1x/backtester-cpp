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
	QPointer<InteractiveChartView> chart_view;
	QPointer<QChart> chart;
	QPointer<QLineSeries> main_series;
	QPointer<QLineSeries> short_ma_series;
	QPointer<QLineSeries> long_ma_series;
	QPointer<QValueAxis> axis_x;
	QPointer<QValueAxis> axis_y;
	QPointer<QLabel> tooltip_label;
	QPointer<QLabel> metrics_label;
	QPointer<QScatterSeries> buy_markers;
	QPointer<QScatterSeries> sell_markers;
	QPointer<QVBoxLayout> layout;

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
	void clearChart();
};

#endif
