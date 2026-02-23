#ifndef BACKTESTER_CPP_STRATEGYMANAGER_H
#define BACKTESTER_CPP_STRATEGYMANAGER_H

#include <QString>
#include <QWidget>
#include <QSqlDatabase>
#include <QListWidget>

struct StrategyData {
    int id;
    QString name;
    QString type;
    int shortWindow;
    int longWindow;
};

class StrategyManager : public QWidget {
    Q_OBJECT

    QSqlDatabase db;
    QListWidget* strategy_list;

    void setup_ui();

    void setup_database();

public:
    explicit StrategyManager(QWidget* parent = nullptr);
    ~StrategyManager();

    bool add_strategy(const QString& name, const QString& type, int shortWindow, int longWindow);

};


#endif