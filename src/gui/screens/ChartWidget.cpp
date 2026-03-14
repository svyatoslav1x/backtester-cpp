#include "ChartWidget.h"
#include <QTimer>
#include <QToolTip>
#include <QVBoxLayout>

InteractiveChartView::InteractiveChartView(QChart* chart, QWidget* parent) : QChartView(chart, parent) {
	setRubberBand(QChartView::RectangleRubberBand); // enables rectangle zooming
}

// zooms in and out
void InteractiveChartView::wheelEvent(QWheelEvent* event) {
	qreal factor = event->angleDelta().y() > 0 ? 0.97 : 1.03;
	chart()->zoom(factor);
	QChartView::wheelEvent(event);
}

// tracks the mouse cursor as it moves across the chart
void InteractiveChartView::mouseMoveEvent(QMouseEvent* event) {
	QPointF chartPoint = chart()->mapToValue(event->pos());
	emit pointHovered(chartPoint);
	QChartView::mouseMoveEvent(event);
}

ChartWidget::ChartWidget(const QString& title, bool price_chart, QWidget* parent) :
	QWidget(parent), is_price_chart(price_chart), data_point_counter(0) {
	chart = new QChart();
	chart->setTitle(title);
	chart->setTitleFont(QFont("Arial", 14, QFont::Bold));
	chart->setAnimationOptions(QChart::NoAnimation); // disabled for performance
	chart->setTheme(QChart::ChartThemeLight);

	// main line (Price or Equity)
	main_series = new QLineSeries();
	main_series->setName(price_chart ? "Price" : "Equity");
	main_series->setPen(QPen(QColor("#1E40AF"), 2));
	chart->addSeries(main_series);

	// configure MAs and signal markers only if it is the Price Chart
	if (is_price_chart) {
		// short MA
		short_ma_series = new QLineSeries();
		short_ma_series->setName("Short MA");
		short_ma_series->setPen(QPen(QColor("#2563EB"), 2));
		chart->addSeries(short_ma_series);

		// long MA
		long_ma_series = new QLineSeries();
		long_ma_series->setName("Long MA");
		long_ma_series->setPen(QPen(QColor("#94A3B8"), 2));
		chart->addSeries(long_ma_series);

		// green triangle (buy)
		buy_markers = new QScatterSeries();
		buy_markers->setName("Buy (Long)");
		buy_markers->setColor(QColor("#16A34A"));
		buy_markers->setMarkerSize(15);
		buy_markers->setMarkerShape(QScatterSeries::MarkerShapeTriangle);
		buy_markers->setBorderColor(QColor("#166534"));
		chart->addSeries(buy_markers);

		// red triangle (sell)
		sell_markers = new QScatterSeries();
		sell_markers->setName("Sell (Exit)");
		sell_markers->setColor(QColor("#DC2626"));
		sell_markers->setMarkerSize(15);
		sell_markers->setMarkerShape(QScatterSeries::MarkerShapeTriangle);
		sell_markers->setBorderColor(QColor("#991B1B"));
		chart->addSeries(sell_markers);
	} else {
		short_ma_series = nullptr;
		long_ma_series = nullptr;
		buy_markers = nullptr;
		sell_markers = nullptr;
	}

	// axes setup
	// time (x)
	axis_x = new QValueAxis();
	axis_x->setTitleText("Time");
	axis_x->setLabelFormat("%d");
	axis_x->setTitleFont(QFont("Arial", 10, QFont::Bold));

	// price (y)
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

		buy_markers->attachAxis(axis_x);
		buy_markers->attachAxis(axis_y);
		sell_markers->attachAxis(axis_x);
		sell_markers->attachAxis(axis_y);
	}

	// move the legend to the bottom
	chart->legend()->setAlignment(Qt::AlignBottom);
	chart->legend()->setFont(QFont("Arial", 9));

	chart_view = new InteractiveChartView(chart);
	chart_view->setRenderHint(QPainter::Antialiasing); // smooth out lines (through adding sub-pixels)

	connect(chart_view, &InteractiveChartView::pointHovered, this, &ChartWidget::on_point_hovered);
	connect(chart_view, &InteractiveChartView::mouseLeft, this, &ChartWidget::on_mouse_left);

	// floating box that follows the mouse
	tooltip_label = new QLabel(this);
	tooltip_label->setStyleSheet(
		"QLabel { background-color: rgba(17, 24, 39, 230); color: white; " "border: 1px solid #475569; border-radius: 5px; padding: 5px; " "font-size: 10pt; }");
	tooltip_label->hide();
	tooltip_label->setAttribute(Qt::WA_TransparentForMouseEvents); // let mouse clicks pass through the label

	// metrics bar
	metrics_label = new QLabel(this);
	metrics_label->setStyleSheet(
		"QLabel { background-color: #F8FAFC; color: #111827; " "border: 2px solid #CBD5E1; border-radius: 8px; padding: 10px; " "font-size: 9pt; font-weight: bold; }");
	metrics_label->setText("Metrics: N/A");

	layout = new QVBoxLayout(this);
	layout->addWidget(metrics_label);
	layout->addWidget(chart_view);
	layout->setContentsMargins(5, 5, 5, 5);
	setLayout(layout);
}

ChartWidget::~ChartWidget() {}

// adds data point, adjusts zoom, recalculates math (update_metrics)
void ChartWidget::add_data_point(double x, double y) {
	x_data.push_back(x);
	y_data.push_back(y);

	main_series->append(x, y);

	if (x_data.size() > 1) {
		auto_scale(); // ensures the axes expand to keep the line visible
	}

	update_metrics();
}

// adds ma point (only if chart is price chart)
void ChartWidget::add_ma_point(double x, double short_ma, double long_ma) {
	if (is_price_chart && short_ma > 0 && long_ma > 0) {
		short_ma_series->append(x, short_ma);
		long_ma_series->append(x, long_ma);
		short_ma_data.push_back(short_ma);
		long_ma_data.push_back(long_ma);
	}
}

// dynamically adjusts axis boundaries to ensure all lines stay inside the visible area
void ChartWidget::auto_scale() {
	if (x_data.empty() || y_data.empty())
		return;

	// find min and max of current dataset
	double x_min = *std::min_element(x_data.begin(), x_data.end());
	double x_max = *std::max_element(x_data.begin(), x_data.end());
	double y_min = *std::min_element(y_data.begin(), y_data.end());
	double y_max = *std::max_element(y_data.begin(), y_data.end());

	// calculate margins so line doesn't hit edge
	double x_padding = (x_max - x_min) * 0.05;
	double y_padding = (y_max - y_min) * 0.1;

	if (x_padding == 0)
		x_padding = 1;
	if (y_padding == 0)
		y_padding = 1;

	axis_x->setRange(x_min - x_padding, x_max + x_padding);
	axis_y->setRange(y_min - y_padding, y_max + y_padding);
}

// computes statistics for the top banner
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

	// Volatility calculation
	double variance = 0.0;
	for (double val : y_data) {
		variance += (val - avg_value) * (val - avg_value);
	}
	double volatility = std::sqrt(variance / y_data.size());

	// Total % Change
	double change = 0.0;
	if (y_data.size() > 1 && y_data.front() != 0) {
		change = ((current_value - y_data.front()) / y_data.front()) * 100.0;
	}

	// assign text to label
	QString metrics_text;
	if (is_price_chart) {
		metrics_text = QString("Current: $%1 | Min: $%2 | Max: $%3 | Avg: $%4 | Vol: $%5 | Change: %6%")
						   .arg(current_value, 0, 'f', 2)
						   .arg(min_value, 0, 'f', 2)
						   .arg(max_value, 0, 'f', 2)
						   .arg(avg_value, 0, 'f', 2)
						   .arg(volatility, 0, 'f', 2)
						   .arg(change, 0, 'f', 2);
	} else {
		metrics_text = QString("Portfolio: $%1 | Min: $%2 | Max: $%3 | Avg: $%4 | ROI: %5%")
						   .arg(current_value, 0, 'f', 2)
						   .arg(min_value, 0, 'f', 2)
						   .arg(max_value, 0, 'f', 2)
						   .arg(avg_value, 0, 'f', 2)
						   .arg(change, 0, 'f', 2);
	}

	metrics_label->setText(metrics_text);
}

// get the continuous mouse coordinates to the closest logged data point
QString ChartWidget::format_point_info(const QPointF& point) {
	int x_idx = static_cast<int>(std::round(point.x()));

	if (x_idx < 0 || x_data.empty()) {
		return QString("");
	}

	// iterate through data to find point nearest to cursor
	size_t closest_idx = 0;
	double min_dist = std::abs(x_data[0] - point.x());
	for (size_t i = 1; i < x_data.size(); ++i) {
		double dist = std::abs(x_data[i] - point.x());
		if (dist < min_dist) {
			min_dist = dist;
			closest_idx = i;
		}
	}

	// don't show tooltip if cursor is too far
	if (min_dist > 1.0) {
		return QString("");
	}

	// info for user to read (depends on if there are MAs or not)
	QString info;
	if (is_price_chart && closest_idx < short_ma_data.size()) {
		info = QString("Time: %1\nPrice: $%2\nShort MA: $%3\nLong MA: $%4")
				   .arg(static_cast<int>(x_data[closest_idx]))
				   .arg(y_data[closest_idx], 0, 'f', 2)
				   .arg(short_ma_data[closest_idx], 0, 'f', 2)
				   .arg(long_ma_data[closest_idx], 0, 'f', 2);
	} else if (closest_idx < y_data.size()) {
		info = QString("Time: %1\nValue: $%2")
				   .arg(static_cast<int>(x_data[closest_idx]))
				   .arg(y_data[closest_idx], 0, 'f', 2);
	}

	return info;
}

// moves and shows the tooltip box dynamically as cursor moves
void ChartWidget::on_point_hovered(const QPointF& point) {
	QString info = format_point_info(point);

	if (!info.isEmpty()) {
		tooltip_label->setText(info);
		tooltip_label->adjustSize(); // the label shrinks or grows, depending on the size of info
		QPoint globalPos = QCursor::pos();
		QPoint localPos = mapFromGlobal(globalPos); // look for the point inside the chart
		tooltip_label->move(localPos.x() + 10, localPos.y() + 10); // push the tooltip down and to the right.
		tooltip_label->show();
		tooltip_label->raise(); // ensure it renders on top
	} else {
		tooltip_label->hide();
	}
}

// hides tooltip when mouse stops touching chart
void ChartWidget::on_mouse_left() {
	tooltip_label->hide();
}

// adds triangles (buy/sell)
void ChartWidget::add_signal_marker(double x, double y, bool is_buy) {
	if (is_price_chart) {
		if (is_buy) {
			buy_markers->append(x, y);
		} else {
			sell_markers->append(x, y);
		}
	}
}

// completely clears current chart state (used when returning to config screen)
void ChartWidget::clearChart() {
	// clear all the plotted series
	main_series->clear();
	if (is_price_chart) {
		short_ma_series->clear();
		long_ma_series->clear();
		buy_markers->clear();
		sell_markers->clear();
	}

	// clear the internal data vectors
	x_data.clear();
	y_data.clear();
	short_ma_data.clear();
	long_ma_data.clear();

	// reset counters and UI elements
	data_point_counter = 0;
	metrics_label->setText("Metrics: N/A");

	// revert axis scaling to defaults
	axis_x->setRange(0, 100);
	axis_y->setRange(0, 100);
}

// ensure tooltip disappears when mouse physically exits the widget
void InteractiveChartView::leaveEvent(QEvent* event) {
	emit mouseLeft();
	QChartView::leaveEvent(event);
}