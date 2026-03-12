#ifndef BACKTESTER_CPP_BACKTESTWINDOW_H
#define BACKTESTER_CPP_BACKTESTWINDOW_H

#include <QLabel>
#include <QPointer>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>

class ChartWidget;

class BacktestWindow : public QWidget {
	Q_OBJECT
public:
	explicit BacktestWindow(QWidget* parent = nullptr);
	~BacktestWindow();

	void add_data_point(double x, double y);
	void add_ma_point(double x, double short_ma, double long_ma);
	void add_equity_point(double x, double y);

	void add_signal_marker(double x, double y, bool is_buy);
	void resetUI();
	void set_simulation_finished();

	void update_progress(int percentage);

signals:
	void pauseToggled(bool paused);
	void toDoneScreen();
	void toSelectStrategyScreen();

private slots:
	void on_pause_clicked();
private:
	QPointer<QWidget> charts_view;

	QPointer<ChartWidget> equity_chart;
	QPointer<ChartWidget> price_chart;

	QPointer<QPushButton> pause_button;
	QPointer<QPushButton> back_button;
	QPointer<QPushButton> show_results_button;

	QPointer<QVBoxLayout> main_layout;
	QPointer<QVBoxLayout> charts_layout;
	QPointer<QHBoxLayout> bottom_buttons_layout;
	QPointer<QSplitter> splitter;
	QPointer<QLabel> progress_label;

	bool is_paused = false;
};

#endif
