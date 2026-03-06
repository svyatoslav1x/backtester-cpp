#include <QApplication>
#include <QTimer>
#include <random>
#include "ChartWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ChartWidget window("Simulation", true);
    window.resize(1200, 800);
    window.show();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0.0, 0.5);

    static double current_price = 150.0;
    static int time_step = 0;

    for (int i = 0; i < 50; ++i) {
        current_price += dist(gen);
        window.add_data_point(time_step++, current_price);
    }

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        current_price += dist(gen);
        window.add_data_point(time_step++, current_price);
    });

    timer.start(100);

    return app.exec();
}