#include "BacktestWindow.h"
#include "ChartWidget.h"

#include "style.h"

#include <QHBoxLayout>
#include <QSplitter>

BacktestWindow::BacktestWindow(QWidget* parent) : QWidget(parent) {
	main_layout = new QVBoxLayout(this);

	charts_view = new QWidget();
	charts_layout = new QVBoxLayout(charts_view);

	// QSplitter allows the user to drag the border between the two charts to resize them
	splitter = new QSplitter(Qt::Vertical);
	equity_chart = std::make_unique<ChartWidget>("Portfolio Value", false);
	price_chart = std::make_unique<ChartWidget>("Asset Price", true);

	splitter->addWidget(equity_chart.get());
	splitter->addWidget(price_chart.get());

	// initially the stretch ratio is 1:1
	splitter->setStretchFactor(0, 1);
	splitter->setStretchFactor(1, 1);

	bottom_buttons_layout = new QHBoxLayout();

	// back button setup
	back_button = new QPushButton("Back");
	back_button->setMinimumSize(100, 40);
	setSecondaryButtonStyle(back_button, 11, true);
	connect(back_button, &QPushButton::clicked, this, [this] { emit toSelectStrategyScreen(); });

	// pause button setup
	pause_button = new QPushButton("Pause");
	pause_button->setMinimumSize(100, 40);
	pause_button->setStyleSheet("font-weight: bold; background-color: #333; color: white;");
	connect(pause_button, &QPushButton::clicked, this, &BacktestWindow::on_pause_clicked);

	// progress label
	progress_label = new QLabel("0%");
	progress_label->setStyleSheet("font-size: 14pt; font-weight: bold; color: #475569;");

	// results button (disabled until simulation is over)
	show_results_button = new QPushButton("Show Results");
	show_results_button->setEnabled(false);
	show_results_button->setMinimumSize(100, 40);
	setButtonStyle(show_results_button, colors[0], colors[1], 11, true);
	connect(show_results_button, &QPushButton::clicked, this, [this] { emit toDoneScreen(); });

	// assemble bottom layout
	bottom_buttons_layout->addWidget(back_button);
	bottom_buttons_layout->addWidget(pause_button);
	bottom_buttons_layout->addWidget(progress_label);
	bottom_buttons_layout->addStretch();
	bottom_buttons_layout->addWidget(show_results_button);

	charts_layout->addWidget(splitter);
	charts_layout->addLayout(bottom_buttons_layout);
	main_layout->addWidget(charts_view);
}

BacktestWindow::~BacktestWindow() {}

// pass price point to the price chart
void BacktestWindow::add_data_point(double x, double y) { price_chart->add_data_point(x, y); }

// pass MA points to the price chart
void BacktestWindow::add_ma_point(double x, double short_ma, double long_ma) { price_chart->add_ma_point(x, short_ma, long_ma); }

// pass equity point updates to the equity chart
void BacktestWindow::add_equity_point(double x, double y) { equity_chart->add_data_point(x, y); }

// handles the logic for toggling the pause state and updating the button's look
void BacktestWindow::on_pause_clicked() {
	is_paused = !is_paused;

	if (is_paused) {
		pause_button->setText("Resume");
		pause_button->setStyleSheet("font-weight: bold; background-color: #2e7d32; color: white;");
	} else {
		pause_button->setText("Pause");
		pause_button->setStyleSheet("font-weight: bold; background-color: #333; color: white;");
	}

	emit pauseToggled(is_paused); // notify SimulationEngine
}

// place triangle on the price chart
void BacktestWindow::add_signal_marker(double x, double y, bool is_buy) {
	price_chart->add_signal_marker(x, y, is_buy);
}

// resets visual state for a fresh backtest
void BacktestWindow::resetUI() {
	is_paused = false;
	pause_button->setText("Pause");
	// restore the default style from the constructor
	pause_button->setStyleSheet("font-weight: bold; background-color: #333; color: white;");

	// disable the results button until the new simulation is finished
	show_results_button->setEnabled(false);

	if (progress_label) {
		progress_label->setText("0%");
	}

	// tell child charts to clear themselves
	if (equity_chart) {
		equity_chart->clearChart();
	}
	if (price_chart) {
		price_chart->clearChart();
	}
}

// activated when SimulationEngine finishes the simulation
void BacktestWindow::set_simulation_finished() {
	show_results_button->setEnabled(true);
}

// updates the percentage
void BacktestWindow::update_progress(int percentage) {
	if (progress_label) {
		progress_label->setText(QString("Simulation is done by %1%").arg(percentage));
	}
}