#ifndef BACKTESTER_STRATEGYDATABASE_H
#define BACKTESTER_STRATEGYDATABASE_H

#include <QSqlDatabase>
#include <QString>

class StrategyDatabase {
public:
    static QString connectionName();

    static QString databasePath();

    static bool openDatabase(QSqlDatabase &db);

    static bool initialize();

    static QSqlDatabase database();

private:
    static bool createTables(QSqlDatabase &db);
};

#endif //BACKTESTER_STRATEGYDATABASE_H
