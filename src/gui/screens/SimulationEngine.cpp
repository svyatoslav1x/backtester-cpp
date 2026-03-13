#include "SimulationEngine.h"

SimulationEngine::SimulationEngine(QObject* parent) : QObject(parent), time_step(0), last_position(0) {
	// connect the QTimer's timeout to the step function
	connect(&timer, &QTimer::timeout, this, &SimulationEngine::simulateStep);
}

void SimulationEngine::startSimulation() {
	if (!backtester || finished)
		return;

	// pre-fill the first 50 data points instantly so the chart isn't empty on load
	for (int i = 0; i < 50; ++i) {
		simulateStep();
	}

	// start periodic updates (e.g., 50ms per tick for smooth visualization)
	timer.start(50);
}

void SimulationEngine::setup(std::unique_ptr<Backtester> bt, const std::string& symbol) {
	backtester = std::move(bt);
	active_symbol = symbol;
	time_step = 0;
	last_position = 0;
	finished = false;
	if (symbol == "AAPL") total_steps = 755;
	else if (symbol == "OMXS30") total_steps = 4813;
	else if (symbol == "SNP") total_steps = 4528;
}

void SimulationEngine::stop() {
	timer.stop();
	finished = true;
}

void SimulationEngine::simulateStep() {
	if (!backtester || finished) {
		return;
	}

	if (!backtester->step()) {
		timer.stop();
		finished = true;

		QString final_stats = "No data processed.";
		if (backtester) {
			// extract final summary stats from Portfolio
			auto& port = static_cast<NaivePortfolio&>(backtester->get_portfolio());
			auto stats = port.summary_stats();

			// format into a single string for DoneScreen
			final_stats = "";
			for (const auto& stat : stats) {
				final_stats +=
					QString::fromStdString(stat.first) + ": " + QString::fromStdString(stat.second) + "\n";
			}
		}

		emit progressUpdated(100);
		emit simulationFinished(final_stats);
		return;
	}

	auto& dh = backtester->get_data_handler();
	auto& port = static_cast<NaivePortfolio&>(backtester->get_portfolio());
	auto* strat = backtester->get_strategy();

	// get latest price
	auto bars = dh.get_latest_bars(active_symbol, 1);
	if (bars.empty())
		return;
	double price = bars[0].close;

	// get current pos
	int current_pos = port.get_position(active_symbol);

	// calculate total equity: cash + (position * current price)
	double equity = port.get_cash() + (current_pos * price);

	if (current_pos > last_position) {
		emit signalUpdated(time_step, price, true); // buy marker
	} else if (current_pos < last_position) {
		emit signalUpdated(time_step, price, false); // sell marker
	}
	last_position = current_pos;

	double short_ma = 0.0;
	double long_ma = 0.0;

	if (strat) {
		auto indicators = strat->get_indicators();

		if (indicators.count("Short EMA") && indicators.count("Long EMA")) {
			short_ma = indicators["Short EMA"];
			long_ma = indicators["Long EMA"];
		} else if (indicators.count("Stop Level")) {
			long_ma = indicators["Stop Level"];
		}
	}

	emit priceUpdated(time_step, price);
	emit equityUpdated(time_step, equity);

	if (short_ma > 0 || long_ma > 0) {
		emit maUpdated(time_step, short_ma, long_ma);
	}

	if (total_steps > 0) {
		int percentage = static_cast<int>((static_cast<double>(time_step) / total_steps) * 100.0);
		emit progressUpdated(std::min(percentage, 99));
	}

	time_step++;
}

void SimulationEngine::setPaused(bool isPaused) {
	if (!backtester || finished) {
		return;
	}

	if (isPaused) {
		timer.stop();
	} else {
		timer.start(50);
	}
}