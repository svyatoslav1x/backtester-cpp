#include "SimulationEngine.h"

SimulationEngine::SimulationEngine(QObject *parent)
    : QObject(parent), dist(0.0, 0.5), current_price(150.0), current_equity(1000.0), time_step(0) {

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

void SimulationEngine::simulateStep() {
    if (time_step >= max_steps) {
        timer.stop();
        emit simulationFinished("Final Equity: $" + QString::number(current_equity, 'f', 2));
        return;
    }

    current_price += dist(gen);
    current_equity += dist(gen) * 10.0;

    double short_ma = current_price * 0.995 + (dist(gen) * 0.1);
    double long_ma = current_price * 0.985 + (dist(gen) * 0.2);

    emit priceUpdated(time_step, current_price);
    emit maUpdated(time_step, short_ma, long_ma);
    emit equityUpdated(time_step, current_equity);

    time_step++;
}

void SimulationEngine::setPaused(bool isPaused) {
    if (isPaused) {
        timer.stop();
    } else {
        timer.start(100);
    }
}
