#ifndef MAIN_WINDOW
#define MAIN_WINDOW

#include <QStackedWidget>
#include <QMainWindow>
#include <QPointer>
#include <QScreen>

class QStackedWidget;
class QWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    // screens
    QPointer<QStackedWidget> stacked_widget;
    QPointer<QWidget> start_screen;
    QPointer<QWidget> strategy_screen;
    QPointer<QWidget> create_strategy_screen;
    QPointer<QWidget> select_strategy_screen;
    QPointer<QWidget> backtest_screen;
    QPointer<QWidget> done_screen;
};

#endif //MAIN_WINDOW
