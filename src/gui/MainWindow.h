#ifndef MAIN_WINDOW
#define MAIN_WINDOW
#include <QMainWindow>
#include <memory>
#include <QPointer>
#include <QScreen>
#include <QGuiApplication>

class QStackedWidget;
class QWidget;

class MainWindow: public QMainWindow {
    QObjects;
    // screens
    Qpointer<QStackedWidget> stacked_widget;
    Qpointer<QWidget> start_screen;
    Qpointer<QWidget> strategy_screen;
    Qpointer<QWidget> create_strategy_screen;
    Qpointer<QWidget> manage_strategies_screen;
    Qpointer<QWidget> backtest_screen;
    Qpointer<QWidget> done_screen;

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
};

#endif //MAIN_WINDOW