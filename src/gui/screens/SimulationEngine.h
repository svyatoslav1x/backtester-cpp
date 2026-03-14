#ifndef SIMULATIONENGINE_H
#define SIMULATIONENGINE_H

#include "../../../include/backtester.h"
#include <QObject>
#include <QTimer>

// the SimulationEngine drives the core backtesting loop, operating step-by-step
// so that the UI can update live (instead of processing everything instantly)
class SimulationEngine : public QObject {
	Q_OBJECT
public:
	explicit SimulationEngine(QObject* parent = nullptr);

	// injects the backtester logic and selected dataset to prepare for simulation
	void setup(std::unique_ptr<Backtester> bt, const std::string& symbol);
	void startSimulation();
    void stop();

	// for tests
	bool isFinished() const { return finished; }
	bool isTimerActive() const { return timer.isActive(); }
signals:
	// new signal to draw the Green/Red triangles (Buy/Sell)
	void signalUpdated(double x, double y, bool is_buy);

	// signals that update the ChartWidget on every simulated tick
	void priceUpdated(double time_step, double price);
	void maUpdated(double time_step, double short_ma, double long_ma);
	void equityUpdated(double time_step, double equity);

	// passes the final formatted results to the DoneScreen
	void simulationFinished(const QString& stats);

	// Updates the loading percentage indicator
	void progressUpdated(int percentage);
public slots:
	// to pause or resume the QTimer
	void setPaused(bool isPaused);
private slots:
	// processes exactly one step of the backtester. Tied to the QTimer timeout
	void simulateStep();
private:
	QTimer timer; // timer driving the simulation loops
	std::unique_ptr<Backtester> backtester; // core logic backend
	std::string active_symbol;

	int last_position = 0; // tracks position state to detect when buys/sells occur
	int time_step; // the 'x' axis of the simulation
	int total_steps = 0; // used for progress percentage calculations
    bool finished = false;

	static constexpr int TICK_INTERVAL_MS = 50; // delay between chart updates (50ms)
	static constexpr int PREFILL_COUNT = 50; // points to instantly generate so chart isn't empty on load
	static constexpr int MAX_PROGRESS_PERCENT = 99; // hold progress at 99% until fully complete

	// hardcoded steps for these specific assets (used for calculating total progress)
	static constexpr int STEPS_AAPL   = 755;
	static constexpr int STEPS_OMXS30 = 4813;
	static constexpr int STEPS_SNP    = 4528;
};

#endif
