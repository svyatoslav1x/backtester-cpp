#include "StrategyManager.h"

#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

StrategyManager::StrategyManager(QWidget* parent)
: QWidget(parent) {
    setup_database();
}

StrategyManager::~StrategyManager() {
    if (db.isOpen()) {
        db.close();
    }
}

// Sets up the SQLite database where strategies are saved
void StrategyManager::setup_database() {

    // Finds a place on the user's computer to save the file
    QString data_path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    dir.mkpath(data_path);

    QString db_path = QDir(data_path).filePath("strategies.db"); // The path to database file "strategies.db"

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(db_path);

    if (!db.open()) {
        QMessageBox::critical(this, "Database Error",
            "Failed to open database: " + db.lastError().text());
        return;
    }

    // Creates the "strategies" table if it's the first time the app is run
    QSqlQuery query(db);
    QString create_table = R"(
        CREATE TABLE IF NOT EXISTS strategies (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT UNIQUE NOT NULL,
            type TEXT NOT NULL,
            short_window INTEGER NOT NULL,
            long_window INTEGER NOT NULL
        )
    )";

    if (!query.exec(create_table)) {
        QMessageBox::critical(this, "Database Error",
            "Failed to create table: " + query.lastError().text());
    }

    query.exec("SELECT COUNT(*) FROM strategies");
    if (query.next() && query.value(0).toInt() == 0) {
        // If empty, adds a default strategy
        add_strategy("Default MA Strategy", "Moving Average Crossover", 50, 200);
    }
}

// Adds a new strategy to the database
bool StrategyManager::add_strategy(const QString& name, const QString& type, int shortWindow, int longWindow) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO strategies (name, type, short_window, long_window) VALUES (?, ?, ?, ?)");
    query.addBindValue(name);
    query.addBindValue(type);
    query.addBindValue(shortWindow);
    query.addBindValue(longWindow);

    if (!query.exec()) {
        QMessageBox::warning(this, "Error",
            "Failed to add strategy: " + query.lastError().text());
        return false;
    }

    return true;
}
