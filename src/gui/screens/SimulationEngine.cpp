#include "SimulationEngine.h"

SimulationEngine::SimulationEngine(QObject* parent) :
	QObject(parent), dist(0.0, 0.5), current_price(150.0), current_equity(1000.0), time_step(0) {

	std::random_device rd;
	gen = std::mt19937(rd());

	connect(&timer, &QTimer::timeout, this, &SimulationEngine::simulateStep);
}

void SimulationEngine::startSimulation() {
	for (int i = 0; i < 50; ++i) {
		simulateStep();
	}
	timer.start(100);
}

void SimulationEngine::setup(std::unique_ptr<Backtester> bt, const std::string& symbol) {
	backtester = std::move(bt);
	active_symbol = symbol;
	time_step = 0;
	last_position = 0;
}

void SimulationEngine::simulateStep() {
	if (!backtester || !backtester->step()) {
		timer.stop();
		auto& port = static_cast<NaivePortfolio&>(backtester->get_portfolio());
		auto stats = port.summary_stats(); // Get summary for DoneScreen
		emit simulationFinished(stats[0].second); // "Total Return: XX%"
		return;
	}

	double price = dh.get_latest_bars(active_symbol, 1)[0].close;
	double equity = port.get_cash();
	emit priceUpdated(time_step, price);
	emit equityUpdated(time_step, equity);

	int current_pos = port.get_position(active_symbol);
	if (current_pos > last_position)
		emit signalUpdated(time_step, price, true);
	else if (current_pos < last_position)
		emit signalUpdated(time_step, price, false);
	last_position = current_pos;

	auto indicators = strat->get_indicators();

	if (indicators.count("Short EMA") && indicators.count("Long EMA")) {
		emit maUpdated(time_step, indicators["Short EMA"], indicators["Long EMA"]);
	} else if (indicators.count("Stop Level")) {
		emit maUpdated(time_step, 0, indicators["Stop Level"]);
	}

	time_step++;
}

void SimulationEngine::setPaused(bool isPaused) {
	if (isPaused) {
		timer.stop();
	} else {
		timer.start(100);
	}
}
