#ifndef BACKTESTER_CPP_STRATEGYMANAGER_H
#define BACKTESTER_CPP_STRATEGYMANAGER_H

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QString>
#include <QWidget>
#include <QSqlDatabase>
#include <QListWidget>
#include <QPushButton>
#include <QMap>
#include <QVector>
#include <QPointer>
#include <QVBoxLayout>

struct StrategyData {
    int id;
    QString name;
    QString model_type;
    int is_editable;
    QMap<QString, QString> parameters;
};

class StrategyManager : public QWidget {
    Q_OBJECT

    QPointer<QListWidget> strategy_list;
    QPointer<QPushButton> back_button;

    QPointer<QVBoxLayout> main_layout;

    QPointer<QLabel> title;
    QPointer<QLabel> subtitle;

    QPointer<QHBoxLayout> button_layout;
    QPointer<QWidget> item_widget;

    QPointer<QHBoxLayout> item_layout;
    QPointer<QLabel> info_label;

    QPointer<QPushButton> edit_btn;
    QPointer<QPushButton> delete_btn;

    QPointer<QVBoxLayout> layout;
    QPointer<QFormLayout> form_layout;
    QPointer<QLineEdit> name_edit;
    QPointer<QLabel> type_label;
    QPointer<QDialogButtonBox> button_box;

    void setup_ui();

private slots:
    void on_edit_clicked(int id);

    void on_list_item_double_clicked(QListWidgetItem *item);

    void on_delete_clicked(int id);

signals:
    void strategy_updated();

    void startScreenSwitch();

public:
    explicit StrategyManager(QWidget *parent = nullptr);

    void load_strategies();

    ~StrategyManager();

    bool add_strategy(const QString &name, const QString &model_type, const QMap<QString, QString> &parameters);

    StrategyData get_strategy(int id);

    bool strategy_name_exists(const QString &name, int excludeId = -1);

    bool update_strategy(int id, const QString &name, const QMap<QString, QString> &parameters);

    QVector<StrategyData> get_all_strategies();

    bool remove_strategy(int id);
};

#endif
