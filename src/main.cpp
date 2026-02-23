#include <QApplication>
#include "StrategyManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    StrategyManager window;
    window.setWindowTitle("Backtester");
    window.resize(1200, 800);

    window.show();

    return app.exec();
}