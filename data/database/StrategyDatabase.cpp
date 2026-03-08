#include "StrategyDatabase.h"

#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>

QString StrategyDatabase::connectionName() {
    return "strategy_database_connection";
}

QString StrategyDatabase::databasePath() {
    QDir dir(QDir::currentPath());

    if (dir.dirName() == "cmake-build-debug") {
        dir.cdUp();
    }

    return dir.filePath("data/database/strategies.db");
}

bool StrategyDatabase::openDatabase(QSqlDatabase &db) {
    const QString conn = connectionName();

    if (QSqlDatabase::contains(conn)) {
        db = QSqlDatabase::database(conn);
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", conn);
        db.setDatabaseName(databasePath());
    }

    if (db.isOpen()) {
        return true;
    }

    if (!db.open()) {
        return false;
    }

    return true;
}

bool StrategyDatabase::createTables(QSqlDatabase &db) {
    QSqlQuery query(db);

    const QString createStrategies = R"(
        CREATE TABLE IF NOT EXISTS strategies (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL UNIQUE,
            model_type TEXT NOT NULL,
            is_editable INTEGER NOT NULL DEFAULT 1
        )
    )";

    if (!query.exec(createStrategies)) {
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
    )";

    if (!query.exec(createStrategyParameters)) {
        return false;
    }

    const QString createUniqueIndex = R"(
        CREATE UNIQUE INDEX IF NOT EXISTS idx_strategy_param_unique
        ON strategy_parameters(strategy_id, param_key)
    )";

    if (!query.exec(createUniqueIndex)) {
        return false;
    }

    return true;
}

bool StrategyDatabase::initialize() {
    QSqlDatabase db;
    if (!openDatabase(db)) {
        return false;
    }

    return createTables(db);
}

QSqlDatabase StrategyDatabase::database() {
    const QString conn = connectionName();

    if (QSqlDatabase::contains(conn)) {
        return QSqlDatabase::database(conn);
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", conn);
    db.setDatabaseName(databasePath());
    db.open();
    return db;
}
