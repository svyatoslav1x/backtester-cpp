#ifndef GUI_MAIN_WINDOW_DONE_SCREEN_H
#define GUI_MAIN_WINDOW_DONE_SCREEN_H

#include <QLabel>
#include <QTextEdit>
#include <QPointer>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>

class DoneScreen : public QWidget {
    Q_OBJECT;

public:
    explicit DoneScreen(QWidget *parent = nullptr);

    void setResults(double totalReturn, double sharpeRatio, double maxDrawdown);

private:
    void buildUi();

    QPointer<QLabel> text;
    QPointer<QTextEdit> final_result;
    QPointer<QVBoxLayout> main_layout;
    QPointer<QHBoxLayout> buttons;
    QPointer<QPushButton> return_starting_screen;
    QPointer<QPushButton> return_charts_screen;
    QPointer<QWidget> buttons_widget;
signals:
    void StartScreenSwitch();

    void BacktestScreenSwitch();
};
#endif //GUI_MAIN_WINDOW_DONE_SCREEN_H
