#include "../include/backtester.h"
#include "../include/data.h"
#include "../include/execution.h"
#include "../include/portfolio.h"
#include "gui/MainWindow.h"
#include "strategies/macd.h"
#include <QApplication>
#include <memory>
#include <queue>

int main() {
	auto events = std::make_shared<std::queue<std::unique_ptr<Event>>>();

	auto dh = std::make_unique<HistoricCSVDataHandler>(*events, "./data", std::vector<std::string>{"AAPL"});

	auto port = std::make_unique<NaivePortfolio>(*dh, *events, "MACD_Strategy", 100000.0);

	auto exec = std::make_unique<SimulatedExecutionHandler>(*events);

	Backtester bt(events, std::move(dh), std::move(port), std::move(exec));

	auto macd_strategy = std::make_unique<MovingAveragesLongStrategy>(bt.get_data_handler(), bt.get_events(),
																	  bt.get_portfolio(), 50, 100);

	bt.set_strategy(std::move(macd_strategy));

	bt.run();

	QApplication app(argc, argv);

	MainWindow w;
	w.show();

	return app.exec();

	return 0;
}
