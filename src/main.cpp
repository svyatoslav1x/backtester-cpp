#include <QApplication>
#include "ChartWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ChartWidget window("Price Chart", true);

    window.resize(1000, 700);
    window.show();

    return app.exec();
}