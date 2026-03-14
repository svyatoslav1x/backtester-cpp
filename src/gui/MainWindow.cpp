#include "MainWindow.h"

#include "../../data/database/StrategyDatabase.h"

#include "screens/create_strategy_screen.h"
#include "screens/done_screen.h"
#include "screens/select_strategy_screen.h"
#include "screens/start_screen.h"

#include <QDir>
#include <QGuiApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLinearGradient>
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPalette>
#include <QSqlQuery>
#include <QUrl>
#include <QUrlQuery>
#include <memory>
#include <variant>

#include "../../include/backtester.h"
#include "../../include/data.h"
#include "../../include/execution.h"
#include "../../include/portfolio.h"
#include "../../strategies/macd.h"
#include "../../strategies/stop_loss.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Backtester Application"); // window title

    screen = QGuiApplication::primaryScreen(); // getting the main screen in the system
    screenSize = screen->geometry();
    double w = screenSize.width();
    double h = screenSize.height();
    resize(static_cast<int>(w * 0.69), static_cast<int>(h * 0.67)); // funny numbers
    setMinimumSize(1300, 800); // shouldn't be smaller than this, because to some screens its vital

    gradient = QLinearGradient(0, 0, w, h); // a gradient from one smooth gray to another
    gradient.setColorAt(0.0, QColor(239, 239, 240)); // #EFEFF0 color, changed to RGB for efficiency
    gradient.setColorAt(1.0, QColor(238, 238, 238)); // #EEEEEE color, changed to RGB for efficiency

    palette.setBrush(QPalette::Window, QBrush(gradient)); // set the gradient for background
    setPalette(palette); // tells the window to use this pallet
    setAutoFillBackground(true); // autofill the background

    stacked_widget = new QStackedWidget(this); // will have all the widgets and show only one

    if (!StrategyDatabase::initialize()) {
        QMessageBox::warning(this, "Database Error", "Could not initialize strategies database.");
    } // initializing the database (vital for program)

    start_screen = new StartScreen(this); // screen with news + choice of dataset
    select_strategy_screen =
            new SelectStrategyScreen(this); // choosing a strategy after which you can start backtest
    create_strategy_screen = new CreateStrategyScreen(this); // screen for creating a custom strategy
    done_screen = new DoneScreen(this); // screen with overall results of backtest
    edit_strategy_screen = new StrategyManager(this); // screen to edit strategies
    backtest_screen = new BacktestWindow(this); // screen with charts

    stacked_widget->addWidget(start_screen); // 0 - start_screen
    stacked_widget->addWidget(select_strategy_screen); // 1 - select_strategy_screen
    stacked_widget->addWidget(create_strategy_screen); // 2 - create_strategy_screen
    stacked_widget->addWidget(done_screen); // 3 - done_screen
    stacked_widget->addWidget(edit_strategy_screen); // 4 - edit strategy screen
    stacked_widget->addWidget(backtest_screen); // 5 - charts screen

    stacked_widget->setCurrentIndex(0); // the first screen is the starting one
    setCentralWidget(stacked_widget); // makes the stacket_widget the main widget for the window

    seedStrategiesIfNeeded();
    // adds the default 2 strategies
    refreshStrategyList(); // makes a list for all the strategies
    loadDatasets(); // gets all the datasets (data/*.csv)

    setupNewsManager(); // sets up what happens when the news reply comes back
    fetchNews(); // Loads news just when the app starts

    simulation_engine = nullptr;

    connect(stacked_widget, &QStackedWidget::currentChanged, this, [this](int index) {
        if (index == 0) {
            fetchNews(); // if we get back to the main screen have to update news
        }
    });

    // edit strategy buttons
    connect(edit_strategy_screen, &StrategyManager::startScreenSwitch, this,
            [this] { stacked_widget->setCurrentWidget(start_screen); });

    // backtest buttons
    connect(backtest_screen, &BacktestWindow::toSelectStrategyScreen, this,
            [this] {
                stopCurrentSimulation();
                backtest_screen->resetUI();
                stacked_widget->setCurrentWidget(select_strategy_screen);
            });

    connect(backtest_screen, &BacktestWindow::toDoneScreen, this, [this] {
        stacked_widget->setCurrentWidget(done_screen);
    });

    // start screen buttons
    connect(start_screen, &StartScreen::createStrategySwitch, this, [this] {
        stacked_widget->setCurrentWidget(create_strategy_screen); // switch from start screen to create screen
    });

    connect(start_screen, &StartScreen::startBacktestSwitch, this, [this] {
        refreshStrategyList(); // refresh the strategy list before switching there
        stacked_widget->setCurrentWidget(
            select_strategy_screen); // switching to the strategy selecting screen
    });

    connect(start_screen, &StartScreen::manageStrategiesSwitch, this, [this] {
        edit_strategy_screen->load_strategies();
        stacked_widget->setCurrentWidget(edit_strategy_screen);
    });

    // create screen buttons
    connect(create_strategy_screen, &CreateStrategyScreen::StartScreenSwitch, this, [this] {
        stacked_widget->setCurrentWidget(start_screen); // from create strategy back to start screen
    });

    connect(
        create_strategy_screen, &CreateStrategyScreen::saveStrategyRequested, this,
        [this](const CreateStrategyInput &input) {
            // when clicked save
            if (input.name.trimmed().isEmpty()) {
                // check if there is a name
                QMessageBox::warning(this, "Error", "Strategy name cannot be empty");
                return;
            }

            QSqlDatabase db = StrategyDatabase::database(); // get the database connection

            if (!db.transaction()) {
                // trying to start a transaction
                QMessageBox::warning(this, "Error", "Could not start");
                return;
            }

            QSqlQuery checkQuery(db);
            checkQuery.prepare(R"(
                    SELECT COUNT(*)
                    FROM strategies
                    WHERE name = ?
                )");
            checkQuery.addBindValue(input.name.trimmed());
            // since the name isn't static I have to update it through ? and addBindValue interface
            if (!checkQuery.exec()) {
                // execute the SQL command with a check
                db.rollback();
                QMessageBox::warning(this, "Error", "Idk what is this");
                return;
            }

            if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
                // check if the name is unique
                db.rollback(); // rollback to before the transaction
                QMessageBox::warning(this, "Error", "Not a unique name");
                return;
            }

            QSqlQuery strategyQuery(db);
            strategyQuery.prepare(R"(
                    INSERT INTO strategies (name, model_type, is_editable)
                    VALUES (?, ?, ?)
                )");
            strategyQuery.addBindValue(input.name.trimmed());
            strategyQuery.addBindValue(input.type);
            strategyQuery.addBindValue(1);
            // adding the values to the database

            if (!strategyQuery.exec()) {
                db.rollback();
                QMessageBox::warning(this, "Error", "Idk what is this");
                return;
            }

            const int strategyId = strategyQuery.lastInsertId().toInt(); // to add parameters
            QSqlQuery paramQuery(db);

            if (std::holds_alternative<MovingAverageParams>(input.params)) {
                const auto &params = std::get<MovingAverageParams>(input.params);
                // depending on the type of strategy we can adjust different things
                paramQuery.prepare(R"(
                        INSERT INTO strategy_parameters (strategy_id, param_key, param_value, value_type, is_editable)
                        VALUES (?, ?, ?, ?, ?)
                    )");
                paramQuery.addBindValue(strategyId);
                paramQuery.addBindValue("short_window");
                paramQuery.addBindValue(QString::number(params.shortWindow)); // convert to string the number
                paramQuery.addBindValue("int");
                paramQuery.addBindValue(1);
                // adding the short_window value for the newly created strategy

                if (!paramQuery.exec()) {
                    db.rollback();
                    QMessageBox::warning(this, "Error", "idk what this is");
                    return;
                }

                paramQuery.prepare(R"(
                        INSERT INTO strategy_parameters (strategy_id, param_key, param_value, value_type, is_editable)
                        VALUES (?, ?, ?, ?, ?)
                    )");
                paramQuery.addBindValue(strategyId);
                paramQuery.addBindValue("long_window");
                paramQuery.addBindValue(QString::number(params.longWindow)); // convert to string the number
                paramQuery.addBindValue("int");
                paramQuery.addBindValue(1); // needed for edit strategies screen
                // adding the long_window value for the newly created strategy

                if (!paramQuery.exec()) {
                    db.rollback();
                    QMessageBox::warning(this, "Error", "idk what this is");
                    return;
                }
            } else if (std::holds_alternative<StopLossParams>(input.params)) {
                const auto &params = std::get<StopLossParams>(input.params);
                paramQuery.prepare(R"(
                        INSERT INTO strategy_parameters (strategy_id, param_key, param_value, value_type, is_editable)
                        VALUES (?, ?, ?, ?, ?)
                    )");
                paramQuery.addBindValue(strategyId);
                paramQuery.addBindValue("stop_loss_percentage");
                paramQuery.addBindValue(QString::number(params.stopLossPercentage, 'f', 2));
                paramQuery.addBindValue("double");
                paramQuery.addBindValue(1); // needed for edit strategies screen

                if (!paramQuery.exec()) {
                    db.rollback();
                    QMessageBox::warning(this, "Error", "idk what this is");
                    return;
                }
            }

            if (!db.commit()) {
                // commit all the changes and close transaction
                db.rollback();
                QMessageBox::warning(this, "Error", "idk what this is");
                return;
            }

            create_strategy_screen->resetForm(); // reset every value to default ones
            stacked_widget->setCurrentWidget(start_screen);
        });

    // select screen buttons
    connect(select_strategy_screen, &SelectStrategyScreen::StartScreenSwitch, this, [this] {
        stacked_widget->setCurrentWidget(start_screen); // when clicked on back
    });

    // THE CORE INTEGRATION
    connect(select_strategy_screen, &SelectStrategyScreen::StartBacktestSwitch, this, [this] {
        backtest_screen->resetUI();

        const auto selectedId = select_strategy_screen->selectedStrategyId();
        const auto datasetFile = start_screen->selectedDataset();

        if (!selectedId || !datasetFile) {
            QMessageBox::warning(this, "Error", "Select a valid dataset and a strategy.");
            return;
        }

        const int strategyId = *selectedId;
        QString dataset_file = *datasetFile;

        // clean up any previous simulation engine to prevent memory leaks and dangling connections
        stopCurrentSimulation();

        // create a FRESH engine for this specific run
        simulation_engine = std::make_unique<SimulationEngine>(this);

        try {
            std::string symbol = dataset_file.replace(".csv", "").toStdString();
            std::string csv_dir = "data";

            auto events = std::make_shared<std::queue<std::unique_ptr<Event> > >();
            auto dh =
                    std::make_unique<HistoricCSVDataHandler>(*events, csv_dir, std::vector<std::string>{symbol});
            auto port = std::make_unique<NaivePortfolio>(*dh, *events, "GUI_Run", 100000.0);
            auto exec = std::make_unique<SimulatedExecutionHandler>(*events);

            auto bt = std::make_unique<Backtester>(events, std::move(dh), std::move(port), std::move(exec));

            StrategyData strat_data = edit_strategy_screen->get_strategy(strategyId).value();
            std::unique_ptr<Strategy> strategy_instance;

            if (strat_data.model_type == "MovingAveragesLongStrategy") {
                int s_win = strat_data.parameters.value("short_window", "12").toInt();
                int l_win = strat_data.parameters.value("long_window", "50").toInt();
                strategy_instance = std::make_unique<MovingAveragesLongStrategy>(
                    bt->get_data_handler(), bt->get_events(), bt->get_portfolio(), s_win, l_win);
            } else if (strat_data.model_type == "StopLossStrategy") {
                double sl_pct = strat_data.parameters.value("stop_loss_percentage", "0.95").toDouble();
                strategy_instance = std::make_unique<StopLossStrategy>(
                    bt->get_data_handler(), bt->get_events(), bt->get_portfolio(), sl_pct);
            } else {
                QMessageBox::warning(this, "Error",
                                     "Unknown Strategy Type in Database: " + strat_data.model_type);
                simulation_engine->deleteLater(); // clean up if strategy fails
                return;
            }

            bt->set_strategy(std::move(strategy_instance));
            simulation_engine->setup(std::move(bt), symbol);

            // connect signals from the NEWLY created engine
            connect(simulation_engine.get(), &SimulationEngine::priceUpdated, backtest_screen,
                    &BacktestWindow::add_data_point);
            connect(simulation_engine.get(), &SimulationEngine::equityUpdated, backtest_screen,
                    &BacktestWindow::add_equity_point);
            connect(simulation_engine.get(), &SimulationEngine::maUpdated, backtest_screen,
                    &BacktestWindow::add_ma_point);
            connect(simulation_engine.get(), &SimulationEngine::signalUpdated, backtest_screen,
                    &BacktestWindow::add_signal_marker);
            connect(backtest_screen, &BacktestWindow::pauseToggled, simulation_engine.get(),
                    &SimulationEngine::setPaused);
            connect(simulation_engine.get(), &SimulationEngine::progressUpdated,
                    backtest_screen, &BacktestWindow::update_progress);

            connect(simulation_engine.get(), &SimulationEngine::simulationFinished, this,
                    [this](const QString &stats) {
                        done_screen->setResults(stats);
                        backtest_screen->set_simulation_finished();
                    });

            stacked_widget->setCurrentWidget(backtest_screen);
            simulation_engine->startSimulation();
        } catch (const std::exception &e) {
            QMessageBox::critical(this, "Backtest Error", QString("Failed to start backtest:\n") + e.what());
            if (simulation_engine) {
                simulation_engine->deleteLater(); // clean up on failure
                simulation_engine = nullptr;
            }
        }
    });

    // done screen buttons
    connect(done_screen, &DoneScreen::BacktestScreenSwitch, this,
            [this] { stacked_widget->setCurrentWidget(backtest_screen); });

    connect(done_screen, &DoneScreen::StartScreenSwitch, this,
            [this] {
                stopCurrentSimulation();
                backtest_screen->resetUI();
                stacked_widget->setCurrentWidget(start_screen);
            });
}

MainWindow::~MainWindow() = default;

void MainWindow::seedStrategiesIfNeeded() {
    // add 2-3 main strategies
    QSqlDatabase db = StrategyDatabase::database();
    QSqlQuery countQuery(db);

    if (!countQuery.exec("SELECT COUNT(*) FROM strategies")) {
        return;
    }

    if (countQuery.next() && countQuery.value(0).toInt() >= 2) {
        return;
    }

    if (!db.transaction()) {
        return;
    }

    QSqlQuery strategyQuery(db);

    strategyQuery.prepare(R"(
        INSERT INTO strategies (name, model_type, is_editable)
        VALUES (?, ?, ?)
    )");
    strategyQuery.addBindValue("Default Moving Average");
    strategyQuery.addBindValue("MovingAveragesLongStrategy");
    strategyQuery.addBindValue(0);

    if (!strategyQuery.exec()) {
        db.rollback();
        return;
    }

    const int movingAverageId = strategyQuery.lastInsertId().toInt();

    QSqlQuery paramQuery(db);

    paramQuery.prepare(R"(
        INSERT INTO strategy_parameters (strategy_id, param_key, param_value, value_type, is_editable)
        VALUES (?, ?, ?, ?, ?)
    )");
    paramQuery.addBindValue(movingAverageId);
    paramQuery.addBindValue("short_window");
    paramQuery.addBindValue("12");
    paramQuery.addBindValue("int");
    paramQuery.addBindValue(0);

    if (!paramQuery.exec()) {
        db.rollback();
        return;
    }

    paramQuery.prepare(R"(
        INSERT INTO strategy_parameters (strategy_id, param_key, param_value, value_type, is_editable)
        VALUES (?, ?, ?, ?, ?)
    )");
    paramQuery.addBindValue(movingAverageId);
    paramQuery.addBindValue("long_window");
    paramQuery.addBindValue("50");
    paramQuery.addBindValue("int");
    paramQuery.addBindValue(0);

    if (!paramQuery.exec()) {
        db.rollback();
        return;
    }

    strategyQuery.prepare(R"(
        INSERT INTO strategies (name, model_type, is_editable)
        VALUES (?, ?, ?)
    )");
    strategyQuery.addBindValue("Default Stop Loss");
    strategyQuery.addBindValue("StopLossStrategy");
    strategyQuery.addBindValue(0);

    if (!strategyQuery.exec()) {
        db.rollback();
        return;
    }

    const int stopLossId = strategyQuery.lastInsertId().toInt();

    paramQuery.prepare(R"(
        INSERT INTO strategy_parameters (strategy_id, param_key, param_value, value_type, is_editable)
        VALUES (?, ?, ?, ?, ?)
    )");
    paramQuery.addBindValue(stopLossId);
    paramQuery.addBindValue("stop_loss_percentage");
    paramQuery.addBindValue("0.95");
    paramQuery.addBindValue("double");
    paramQuery.addBindValue(0);

    if (!paramQuery.exec()) {
        db.rollback();
        return;
    }

    if (!db.commit()) {
        // commit all the changes and close transaction
        db.rollback();
        QMessageBox::warning(this, "Error", "idk what this is");
        return;
    }
}

void MainWindow::refreshStrategyList() const {
    QList<QPair<QString, int> > items;
    QSqlDatabase db = StrategyDatabase::database();
    QSqlQuery query(db);

    if (!query.exec("SELECT name, id FROM strategies ORDER BY id ASC")) {
        // choosing names and ids of strategies
        return;
    }

    while (query.next()) {
        // until I can't stop getting new lines
        items.append({query.value(0).toString(), query.value(1).toInt()});
    }

    select_strategy_screen->setStrategies(items); // sets strategies to choose from
}

void MainWindow::loadDatasets() const {
    QDir dir(QDir::currentPath() + "/../data"); // redirect to the data/ folder
    if (!dir.exists()) {
        return;
    }

    QList<QString> datasetNames = dir.entryList({"*.csv"}, QDir::Files);
    start_screen->setDatasets(datasetNames);
}

void MainWindow::setupNewsManager() {
    network_manager = new QNetworkAccessManager(this);

    connect(network_manager, &QNetworkAccessManager::finished, this, &MainWindow::handleNewsReply);
}

void MainWindow::fetchNews() const {
    if (last_news_refresh.isValid() && last_news_refresh.secsTo(QDateTime::currentDateTime()) < 60) {
        return;
    }

    const QString apiKey = loadApiKeyFromEnvFile();
    if (apiKey.isEmpty()) {
        return;
    }

    QUrl url("https://gnews.io/api/v4/top-headlines");
    QUrlQuery query;
    query.addQueryItem("category", "business");
    query.addQueryItem("lang", "en");
    query.addQueryItem("country", "us");
    query.addQueryItem("max", "3");
    query.addQueryItem("apikey", apiKey);
    url.setQuery(query);

    QNetworkRequest request(url);
    network_manager->get(request);
}

QString MainWindow::loadApiKeyFromEnvFile() const {
    QDir dir(QDir::currentPath()); // current path will always br "cmake-build-debug"

    if (dir.dirName() == "cmake-build-debug") {
        dir.cdUp(); // go to the project directory
    }
    QFile file(dir.filePath(".env"));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString();
    }

    QTextStream in(&file); // stream for reading the file
    while (!in.atEnd()) {
        // keep reading until the end of the file
        QString line = in.readLine().trimmed(); // read one line and remove surrounding spaces

        if (line.startsWith("GNEWS_API_KEY=")) {
            return line.section('=', 1).trimmed(); // return the value after =
        }
    }

    return QString();
}

void MainWindow::handleNewsReply(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        start_screen->setNewsText("No More News In LA.");
        reply->deleteLater();
        return;
    }

    const QByteArray responseData = reply->readAll();
    reply->deleteLater();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData, &parseError);

    if (parseError.error != QJsonParseError::NoError || !jsonDoc.isObject()) {
        start_screen->setNewsText("No More News In LA.");
        return;
    }

    QJsonObject rootObject = jsonDoc.object();
    QJsonArray articles = rootObject.value("articles").toArray();

    if (articles.isEmpty()) {
        start_screen->setNewsText("No More News In LA.");
        return;
    }

    QString text;

    for (int i = 0; i < std::min<int>(3, articles.size()); ++i) {
        QJsonObject article = articles[i].toObject();

        QString title = article.value("title").toString("No title");
        QString description = article.value("description").toString("No description");
        QString url = article.value("url").toString();

        text += QString("News %1\n").arg(i + 1);
        text += "Title: " + title + "\n";
        text += "Description: " + description + "\n";
        text += "Link: " + url + "\n\n";
    }

    start_screen->setNewsText(text);
    last_news_refresh = QDateTime::currentDateTime();
}

StartScreen *MainWindow::startScreen() const {
    return start_screen;
}

DoneScreen *MainWindow::doneScreen() const {
    return done_screen;
}

SelectStrategyScreen *MainWindow::selectStrategyScreen() const {
    return select_strategy_screen;
}

CreateStrategyScreen *MainWindow::createStrategyScreen() const {
    return create_strategy_screen;
}

QStackedWidget *MainWindow::stackedWidget() const {
    return stacked_widget;
}

void MainWindow::stopCurrentSimulation() {
    if (!simulation_engine) {
        return;
    }

    disconnect(backtest_screen, &BacktestWindow::pauseToggled,
               simulation_engine.get(), &SimulationEngine::setPaused);

    simulation_engine->stop();
    simulation_engine.reset();
}
