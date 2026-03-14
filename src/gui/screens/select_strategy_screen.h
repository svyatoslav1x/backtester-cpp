#ifndef GUI_MAIN_WINDOW_SELECT_STRATEGY_SCREEN_H
#define GUI_MAIN_WINDOW_SELECT_STRATEGY_SCREEN_H

#include <QVBoxLayout>
#include <QPointer>
#include <optional>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

class SelectStrategyScreen : public QWidget {
    Q_OBJECT

public:
    explicit SelectStrategyScreen(QWidget *parent = nullptr);

    void setStrategies(const QList<QPair<QString, int> > &items);

    std::optional<int> selectedStrategyId() const;

    QComboBox *strategyCombo() const;

    QPushButton *backButton() const;

    QPushButton *startBacktestButton() const;

private:
    void buildUi();


    QPointer<QVBoxLayout> layout;
    QPointer<QLabel> title;
    QPointer<QLabel> strategy_label;
    QPointer<QComboBox> strategy_combo;
    QPointer<QHBoxLayout> button_layout;
    QPointer<QPushButton> back_button;
    QPointer<QPushButton> next_button;
signals:
    void StartScreenSwitch();

    void StartBacktestSwitch();
};


#endif //GUI_MAIN_WINDOW_SELECT_STRATEGY_SCREEN_H
