#ifndef BACKTESTER_CPP_STRATEGYMANAGER_H
#define BACKTESTER_CPP_STRATEGYMANAGER_H

#include <QString>
#include <QWidget>
#include <QSqlDatabase>
#include <QListWidget>
#include <QPushButton>

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
    QPushButton* back_button;

    void setup_database();
    void setup_ui();

public:
    explicit StrategyManager(QWidget* parent = nullptr);
    ~StrategyManager();

    bool add_strategy(const QString& name, const QString& type, int shortWindow, int longWindow);

};


#endif