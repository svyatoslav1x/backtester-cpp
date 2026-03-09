#ifndef BACKTESTER_CPP_STRATEGYMANAGER_H
#define BACKTESTER_CPP_STRATEGYMANAGER_H

#include <QString>
#include <QWidget>
#include <QSqlDatabase>
#include <QListWidget>
#include <QPushButton>
#include <QMap>
#include <QVector>

struct StrategyData {
    int id;
    QString name;
    QString model_type;
    int is_editable;
    QMap<QString, QString> parameters;
};

class StrategyManager : public QWidget {
    Q_OBJECT

    QListWidget* strategy_list;
    QPushButton* back_button;

    void setup_ui();
    void load_strategies();

private slots:
    void on_edit_clicked(int id);
    void on_list_item_double_clicked(QListWidgetItem* item);
    void on_delete_clicked(int id);

signals:
    void strategy_updated();

public:
    explicit StrategyManager(QWidget* parent = nullptr);
    ~StrategyManager();

    bool add_strategy(const QString& name, const QString& model_type, const QMap<QString, QString>& parameters);
    StrategyData get_strategy(int id);
    bool strategy_name_exists(const QString& name, int excludeId = -1);
    bool update_strategy(int id, const QString& name, const QMap<QString, QString>& parameters);
    QVector<StrategyData> get_all_strategies();
    bool remove_strategy(int id);
};

#endif