#ifndef GUI_MAIN_WINDOW_SELECT_STRATEGY_SCREEN_H
#define GUI_MAIN_WINDOW_SELECT_STRATEGY_SCREEN_H

#include <QHBoxLayout>

class SelectStrategyScreen : public QWidget {
    Q_OBJECT

public:
    explicit SelectStrategyScreen(QWidget *parent = nullptr);

private:
    void buildUi();

    QPointer<QVBoxLayout> layout;

};


#endif //GUI_MAIN_WINDOW_SELECT_STRATEGY_SCREEN_H
