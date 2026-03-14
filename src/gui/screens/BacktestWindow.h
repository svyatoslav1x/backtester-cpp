#ifndef BACKTESTER_CPP_BACKTESTWINDOW_H
#define BACKTESTER_CPP_BACKTESTWINDOW_H

#include <QLabel>
#include <QPointer>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>
#include <memory>

class ChartWidget;

// BacktestWindow is the central UI view displaying the simulation
// it displays the two charts (Asset Price and Portfolio Value) and buttons (back, pause, show results)
class BacktestWindow : public QWidget {
	Q_OBJECT
public:
	explicit BacktestWindow(QWidget* parent = nullptr);
	~BacktestWindow();

	// pass signals from SimulationEngine to the charts
	void add_data_point(double x, double y);
	void add_ma_point(double x, double short_ma, double long_ma);
	void add_equity_point(double x, double y);
	void add_signal_marker(double x, double y, bool is_buy);

	// UI state management
	void resetUI();
	void set_simulation_finished();
	void update_progress(int percentage);

	// for tests
	QPointer<QPushButton> pauseButton() const { return pause_button; }
	QPointer<QPushButton> backButton() const { return back_button; }
	QPointer<QPushButton> showResultsButton() const { return show_results_button; }
	QPointer<QLabel> progressLabel() const { return progress_label; }
	void triggerPauseClicked() { on_pause_clicked(); }

signals:
	void pauseToggled(bool paused); // to let SimulationEngine know whether to stop/start the timer
	void toDoneScreen();
	void toSelectStrategyScreen();

private slots:
	void on_pause_clicked();
private:
	QPointer<QWidget> charts_view;

	// the two chart instances (managed via unique_ptr since they own their respective graphics)
	std::unique_ptr<ChartWidget> equity_chart;
	std::unique_ptr<ChartWidget> price_chart;

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
