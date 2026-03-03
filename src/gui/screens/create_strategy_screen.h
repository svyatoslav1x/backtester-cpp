#ifndef GUI_MAIN_WINDOW_CREATE_STRATEGY_SCREEN_H
#define GUI_MAIN_WINDOW_CREATE_STRATEGY_SCREEN_H

#include <QTextEdit>
#include <QPointer>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>

struct CreateStrategyInput {
    QString name;
    QString type;
    int shortWindow = 20;
    int longWindow = 50;
};


class CreateStrategyScreen : public QWidget {
    Q_OBJECT

public:
    explicit CreateStrategyScreen(QWidget *parent = nullptr);

    CreateStrategyInput input() const;

    void resetForm();

private:
    void buildUi();

    QPointer<QVBoxLayout> layout;
    QPointer<QLabel> title;
    QPointer<QLabel> name_label;
    QPointer<QWidget> long_row;
    QPointer<QLabel> type_label;
    QPointer<QWidget> form_container;
    QPointer<QVBoxLayout> form_layout;
    QPointer<QLineEdit> strategy_name_edit;
    QPointer<QComboBox> strategy_type_combo;
    QPointer<QLabel> params_label;
    QPointer<QHBoxLayout> short_layout;
    QPointer<QLabel> short_label;
    QPointer<QWidget> short_row;
    QPointer<QLabel> info_label;
    QPointer<QSpinBox> short_window_spin;
    QPointer<QHBoxLayout> long_layout;
    QPointer<QLabel> long_label;
    QPointer<QSpinBox> long_window_spin;
    QPointer<QHBoxLayout> button_layout;
    QPointer<QPushButton> back_button;
    QPointer<QPushButton> save_button;

signals:
    void StartScreenSwitch();
};


#endif //GUI_MAIN_WINDOW_CREATE_STRATEGY_SCREEN_H
