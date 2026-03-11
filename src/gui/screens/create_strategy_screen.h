#ifndef GUI_MAIN_WINDOW_CREATE_STRATEGY_SCREEN_H
#define GUI_MAIN_WINDOW_CREATE_STRATEGY_SCREEN_H

#include <QKeyEvent>
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
    double stopLossPercentage = 0.90;
};

class CreateStrategyScreen : public QWidget {
    Q_OBJECT

public:
    explicit CreateStrategyScreen(QWidget *parent = nullptr);

    CreateStrategyInput input() const;

    void resetForm();

    void updateParameterVisibility();

    QLineEdit *nameEdit() const;

    QComboBox *typeCombo() const;

    QSpinBox *shortWindowSpin() const;

    QSpinBox *longWindowSpin() const;

    QDoubleSpinBox *stopLossSpin() const;

    QPushButton *saveButton() const;

    QPushButton *backButton() const;

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

    QPointer<QLabel> stop_loss_label;
    QPointer<QDoubleSpinBox> stop_loss_spin;
    QPointer<QHBoxLayout> stop_loss_layout;

signals:
    void StartScreenSwitch();

    void saveStrategyRequested(const CreateStrategyInput &input);
};

// extended qspinbox class that doesn't allow keyboard
class MouseOnlySpinBox : public QSpinBox {
public:
    using QSpinBox::QSpinBox;

protected:
    void keyPressEvent(QKeyEvent *event) override {
        event->ignore();
    }
};

// extended qdoublespinbox class that doesn't allow keyboard
class MouseOnlyDoubleSpinBox : public QDoubleSpinBox {
public:
    using QDoubleSpinBox::QDoubleSpinBox;

protected:
    void keyPressEvent(QKeyEvent *event) override {
        event->ignore();
    }
};

#endif //GUI_MAIN_WINDOW_CREATE_STRATEGY_SCREEN_H
