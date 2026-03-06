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
    const int max_steps = 100;

    auto simulate_step = [&]() {
        current_price += dist(gen);

        double short_ma = current_price * 0.995 + (dist(gen) * 0.1);
        double long_ma = current_price * 0.985 + (dist(gen) * 0.2);

        window.add_data_point(time_step, current_price);

        window.add_ma_point(time_step, short_ma, long_ma);

        time_step++;
    };

    for (int i = 0; i < 50; ++i) {
        simulate_step();
    }

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        if (time_step >= max_steps) {
            timer.stop();
            return;
        }
        simulate_step();
    });

    timer.start(100);

    return app.exec();
}