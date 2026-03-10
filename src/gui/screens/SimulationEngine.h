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
	void setup(std::unique_ptr<Backtester> bt, const std::string& symbol);
	void startSimulation();
signals:
	// new signal to draw the Green/Red triangles for Buy/Sell
	void signalUpdated(double x, double y, bool is_buy);

	// signals that update the ChartWidget
	void priceUpdated(double time_step, double price);
	void maUpdated(double time_step, double short_ma, double long_ma);
	void equityUpdated(double time_step, double equity);

	// passes the final formatted results to the DoneScreen
	void simulationFinished(const QString& stats);
public slots:
	void setPaused(bool isPaused);
private slots:
	// processes exactly one step (heartbeat) of the backtester
	void simulateStep();
private:
	QTimer timer;
	std::unique_ptr<Backtester> backtester;
	std::string active_symbol;

	int last_position = 0;
	int time_step;
};

#endif
