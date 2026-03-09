#ifndef MAIN_WINDOW
#define MAIN_WINDOW

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QStackedWidget>

#include "screens/create_strategy_screen.h"
#include "screens/done_screen.h"
#include "screens/select_strategy_screen.h"
#include "screens/start_screen.h"

class QWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    // setup
    void loadDatasets();

    void refreshStrategyList();

    void seedStrategiesIfNeeded();

    // save data for backtest
    bool saveAppState(const QString &dataset, int strategyId);

    QString currentSelectedDataset;
    QDateTime last_news_refresh;

    // screens
    QPointer<StartScreen> start_screen;
    QPointer<SelectStrategyScreen> select_strategy_screen;
    QPointer<CreateStrategyScreen> create_strategy_screen;
    QPointer<DoneScreen> done_screen;
    QPointer<QWidget> strategy_screen; // todo:: should be fixed to nikita's class
    QPointer<QWidget> backtest_screen; // todo:: should be fixed to nikita's class
    QPointer<QStackedWidget> stacked_widget;

    // news
    QPointer<QNetworkAccessManager> network_manager;
};

#endif //MAIN_WINDOW
