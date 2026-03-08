#ifndef MAIN_WINDOW
#define MAIN_WINDOW

#include <QStackedWidget>
#include <QMainWindow>
#include <QPointer>
#include <QScreen>
#include <QNetworkAccessManager>

#include "screens/start_screen.h"
#include "screens/select_strategy_screen.h"
#include "screens/create_strategy_screen.h"
#include "screens/done_screen.h"

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

    QPointer<StartScreen> start_screen;
    QPointer<SelectStrategyScreen> select_strategy_screen;
    QPointer<CreateStrategyScreen> create_strategy_screen;
    QPointer<DoneScreen> done_screen;

    // todo: change after linked with nikita
    QPointer<QWidget> strategy_screen;
    QPointer<QWidget> backtest_screen;

    void seedStrategiesIfNeeded();

    void refreshStrategyList();

    void loadDatasets();

    bool saveAppState(const QString &dataset, int strategyId);

    QString currentSelectedDataset;

    QPointer<QNetworkAccessManager> network_manager;
    QDateTime last_news_refresh;
};

#endif //MAIN_WINDOW
