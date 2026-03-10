#ifndef SIMULATIONENGINE_H
#define SIMULATIONENGINE_H

#include "../../../include/backtester.h"
#include <QObject>
#include <QTimer>
#include <random>

class SimulationEngine : public QObject {
	Q_OBJECT
public:
	explicit SimulationEngine(QObject* parent = nullptr);
	void startSimulation();
signals:
	void signalUpdated(double x, double y, bool is_buy);
	void priceUpdated(double time_step, double price);
	void maUpdated(double time_step, double short_ma, double long_ma);
	void equityUpdated(double time_step, double equity);
	void simulationFinished(const QString& stats);
public slots:
	void setPaused(bool isPaused);
	void setup(std::unique_ptr<Backtester> bt, const std::string& symbol);
private slots:
	void simulateStep();
private:
	QTimer timer;
	std::unique_ptr<Backtester> backtester;
	std::string active_symbol;
	int last_position = 0;

	double current_price;
	double current_equity;
	int time_step;
	const int max_steps = 100;
};

#endif
