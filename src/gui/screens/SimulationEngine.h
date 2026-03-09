#ifndef SIMULATIONENGINE_H
#define SIMULATIONENGINE_H

#include <QObject>
#include <QTimer>
#include <random>

class SimulationEngine : public QObject {
    Q_OBJECT

public:
    explicit SimulationEngine(QObject *parent = nullptr);
    void startSimulation();

signals:
    void priceUpdated(double time_step, double price);
    void maUpdated(double time_step, double short_ma, double long_ma);
    void equityUpdated(double time_step, double equity);
    void simulationFinished(const QString& stats);

public slots:
    void setPaused(bool isPaused);

private slots:
    void simulateStep();

private:
    QTimer timer;
    std::mt19937 gen;
    std::normal_distribution<double> dist;

    double current_price;
    double current_equity;
    int time_step;
    const int max_steps = 100;
};

#endif