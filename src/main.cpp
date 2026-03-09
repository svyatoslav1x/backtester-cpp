#include <QApplication>
#include <QMessageBox>
#include "gui/screens/StrategyManager.h"
#include "../data/database/StrategyDatabase.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    if (!StrategyDatabase::initialize()) {
        QMessageBox::critical(nullptr, "Database Error",
                             "Could not initialize the strategy database");
        return 1;
    }

    StrategyManager manager;
    manager.setWindowTitle("Strategy Management");
    manager.resize(800, 600);
    manager.show();

    return app.exec();
}