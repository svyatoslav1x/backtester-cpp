#include "StrategyDatabase.h"

#include <qcoreapplication.h>
#include <QDir>
#include <QSqlQuery>
#include <QStandardPaths>

QString StrategyDatabase::connectionName() {
    return "strategy_database_connection";
}

QString StrategyDatabase::databasePath() {
    QDir dir(QCoreApplication::applicationDirPath());

    if (dir.dirName().startsWith("cmake-build")) {
        dir.cdUp();
    }

    if (!dir.exists("data/database")) {
        dir.mkpath("data/database");
    }

    return dir.filePath("data/database/strategies.db");
}

bool StrategyDatabase::openDatabase(QSqlDatabase &db) {
    const QString conn = connectionName();

    if (QSqlDatabase::contains(conn)) {
        db = QSqlDatabase::database(conn);
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", conn);
    }

    db.setDatabaseName(databasePath());

    if (db.isOpen()) {
        return true;
    }

    if (!db.open()) {
        return false;
    }

    return true;
}

bool StrategyDatabase::createTables(QSqlDatabase &db) {
    QSqlQuery query(db); // an object to run the sql commands on

    const QString createStrategies = R"(
        CREATE TABLE IF NOT EXISTS strategies (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL UNIQUE,
            model_type TEXT NOT NULL,
            is_editable INTEGER NOT NULL DEFAULT 1
        )
    )"; // creating a table (the structure of tables is in data/README.md)

    if (!query.exec(createStrategies)) {
        // trying to execute this request if failed returns false
        return false;
    }

    const QString createStrategyParameters = R"(
        CREATE TABLE IF NOT EXISTS strategy_parameters (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            strategy_id INTEGER NOT NULL,
            param_key TEXT NOT NULL,
            param_value TEXT NOT NULL,
            value_type TEXT NOT NULL,
            is_editable INTEGER NOT NULL DEFAULT 1,
            FOREIGN KEY (strategy_id) REFERENCES strategies(id) ON DELETE CASCADE
        )
    )"; // creating a table (the structure of tables is in data/README.md)

    if (!query.exec(createStrategyParameters)) {
        // trying to execute this request if failed returns false
        return false;
    }

    const QString createUniqueIndex = R"(
        CREATE UNIQUE INDEX IF NOT EXISTS idx_strategy_param_unique
        ON strategy_parameters(strategy_id, param_key)
    )"; // for one strategy each parameter key can appear only once

    if (!query.exec(createUniqueIndex)) {
        // trying to execute this request if failed returns false
        return false;
    }

    return true;
}

bool StrategyDatabase::initialize() {
    // initializes our database (runs the creation of tables)
    QSqlDatabase db;
    if (!openDatabase(db)) {
        return false;
    }

    return createTables(db);
}

QSqlDatabase StrategyDatabase::database() {
    QSqlDatabase db;
    openDatabase(db);
    return db;
}
