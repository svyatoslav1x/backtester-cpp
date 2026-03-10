#ifndef MAIN_WINDOW
#define MAIN_WINDOW

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QStackedWidget>

#include "screens/BacktestWindow.h"
#include "screens/SimulationEngine.h"
#include "screens/StrategyManager.h"
#include "screens/create_strategy_screen.h"
#include "screens/done_screen.h"
#include "screens/select_strategy_screen.h"
#include "screens/start_screen.h"

class QWidget;

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	explicit MainWindow(QWidget* parent = nullptr);

	~MainWindow() override;
private:
	// setup
	void loadDatasets() const;
	void refreshStrategyList() const;
	void seedStrategiesIfNeeded();
	QString loadApiKeyFromEnvFile() const;

	// save data for backtest
	bool saveAppState(const QString& dataset, int strategyId);

	QString currentSelectedDataset;
	QDateTime last_news_refresh;

	// screens
	QPointer<QScreen> screen;
	QRect screenSize;
	QPointer<StartScreen> start_screen;
	QPointer<SelectStrategyScreen> select_strategy_screen;
	QPointer<CreateStrategyScreen> create_strategy_screen;
	QPointer<DoneScreen> done_screen;
	QPointer<StrategyManager> edit_strategy_screen;
	QPointer<BacktestWindow> backtest_screen;
	QPointer<QStackedWidget> stacked_widget;

	// simulation engine
	SimulationEngine* simulation_engine;

	// different helpers
	QLinearGradient gradient;
	QPalette palette;

	// news
	QPointer<QNetworkAccessManager> network_manager;
	void setupNewsManager();
	void fetchNews() const;
	void handleNewsReply(QNetworkReply* reply);
};

#endif // MAIN_WINDOW
