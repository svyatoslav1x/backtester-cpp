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
    void load_strategies();
    void edit_strategy(int id);

private slots:
    void on_edit_clicked(int id);

signals:
    void strategy_updated();

public:
    explicit StrategyManager(QWidget* parent = nullptr);
    ~StrategyManager();

    bool add_strategy(const QString& name, const QString& type, int shortWindow, int longWindow);
    StrategyData get_strategy(int id);
    bool strategy_name_exists(const QString& name, int excludeId = -1);
    bool update_strategy(int id, const QString& name, const QString& type, int shortWindow, int longWindow);

};


#endif