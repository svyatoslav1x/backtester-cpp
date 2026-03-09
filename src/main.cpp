#include <QApplication>
#include "gui/screens/BacktestWindow.h"
#include "gui/screens/SimulationEngine.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    BacktestWindow window;
    window.setWindowTitle("Simulation");
    window.resize(1200, 800);
    window.show();

    SimulationEngine engine;

    QObject::connect(&engine, &SimulationEngine::priceUpdated, &window, &BacktestWindow::add_data_point);
    QObject::connect(&engine, &SimulationEngine::maUpdated, &window, &BacktestWindow::add_ma_point);
    QObject::connect(&engine, &SimulationEngine::equityUpdated, &window, &BacktestWindow::add_equity_point);
    QObject::connect(&engine, &SimulationEngine::simulationFinished, &window, &BacktestWindow::set_simulation_finished);

    QObject::connect(&window, &BacktestWindow::pauseToggled, &engine, &SimulationEngine::setPaused);

    engine.startSimulation();

    return app.exec();
}