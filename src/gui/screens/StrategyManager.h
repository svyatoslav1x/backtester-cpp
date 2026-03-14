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

// data structure that represents a strategy loaded from the database
struct StrategyData {
    int id;
    QString name;
    QString model_type;
    int is_editable; // can the user modify this strategy?
    QMap<QString, QString> parameters; // pairs for dynamic parameters (e.g. long_window, short_window)
};

// handles viewing, editing and deleting strategies from an SQLite database.
class StrategyManager : public QWidget {
    Q_OBJECT

    // QPointer are used for safety (automatically set to null if the object is destroyed)
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

    // initializes the UI components and layouts
    void setup_ui();

private slots:
    // when the "Edit" button is clicked
    void on_edit_clicked(int id);

    // when a strategy in the list is double-clicked
    void on_list_item_double_clicked(QListWidgetItem *item);

    // when the "Delete" button is clicked
    void on_delete_clicked(int id);

signals:
    // emitted when a strategy is successfully updated, added or deleted to notify other components
    void strategy_updated();

    // emitted to signal the main window to switch back to the start screen
    void startScreenSwitch();

public:
    explicit StrategyManager(QWidget *parent = nullptr);

    // load strategies from the database
    void load_strategies();

    ~StrategyManager();

    // interaction with database
    bool add_strategy(const QString &name, const QString &model_type, const QMap<QString, QString> &parameters);

    std::optional<StrategyData> get_strategy(int id);

    bool strategy_name_exists(const QString &name, int excludeId = -1);

    bool update_strategy(int id, const QString &name, const QMap<QString, QString> &parameters);

    QVector<StrategyData> get_all_strategies();

    bool remove_strategy(int id);

    // for tests
    QPointer<QPushButton> backButton() const { return back_button; }
    QPointer<QListWidget> strategyList() const { return strategy_list; }
};

#endif
