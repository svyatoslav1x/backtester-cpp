#include "MainWindow.h"

#include <iostream>

#include "screens/create_strategy_screen.h"
#include "screens/done_screen.h"
#include "screens/select_strategy_screen.h"
#include "screens/start_screen.h"

#include "../../data/database/StrategyDatabase.h"

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
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QUrl>
#include <QVector>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Backtester Application");

    QPointer<QScreen> screen = QGuiApplication::primaryScreen();
    QRect screenSize = screen->geometry();
    int w = screenSize.width();
    int h = screenSize.height();
    resize(w * 0.69, h * 0.67);
    setMinimumSize(760, 520);

    QLinearGradient gradient(0, 0, w, h);
    gradient.setColorAt(0.0, QColor("#EFEFF0"));
    gradient.setColorAt(1.0, QColor("#EEEEEE"));

    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(gradient));
    setPalette(palette);
    setAutoFillBackground(true);

    stacked_widget = new QStackedWidget(this);
    start_screen = new StartScreen(this);
    select_strategy_screen = new SelectStrategyScreen(this);
    create_strategy_screen = new CreateStrategyScreen(this);
    done_screen = new DoneScreen(this);

    stacked_widget->addWidget(start_screen);
    stacked_widget->addWidget(select_strategy_screen);
    stacked_widget->addWidget(create_strategy_screen);
    stacked_widget->addWidget(done_screen);

    stacked_widget->setCurrentIndex(0);
    setCentralWidget(stacked_widget);

    if (!StrategyDatabase::initialize()) {
        QMessageBox::warning(this, "Database Error", "Could not initialize strategies database.");
    }

    seedStrategiesIfNeeded();
    refreshStrategyList();
    loadDatasets();

    auto start = qobject_cast<StartScreen *>(stacked_widget->widget(0));
    auto select = qobject_cast<SelectStrategyScreen *>(stacked_widget->widget(1));
    auto create = qobject_cast<CreateStrategyScreen *>(stacked_widget->widget(2));
    auto done = qobject_cast<DoneScreen *>(stacked_widget->widget(3));

    network_manager = new QNetworkAccessManager(this);

    connect(network_manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply *reply) {
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
    });

    auto refreshNews = [this]() {
        if (last_news_refresh.isValid() &&
            last_news_refresh.secsTo(QDateTime::currentDateTime()) < 60) {
            return;
        }

        QUrl url(
            "https://gnews.io/api/v4/top-headlines?category=business&lang=en"
            "&country=us&max=3&apikey=6eb3836c9755cb7dcf344da74c41be07");
        // free api key with 100 запросы per day should actually be put in .env file, but it's a small project so this will do
        QNetworkRequest request(url);
        network_manager->get(request);
    };

    refreshNews();

    connect(stacked_widget, &QStackedWidget::currentChanged, this, [refreshNews](int index) {
        if (index == 0) {
            refreshNews();
        }
    });

    // start screen buttons
    connect(start, &StartScreen::createStrategySwitch, this, [this] {
        stacked_widget->setCurrentWidget(create_strategy_screen);
    });

    connect(start, &StartScreen::startBacktestSwitch, this, [this](const QString &dataset) {
        currentSelectedDataset = dataset.trimmed();

        if (currentSelectedDataset.isEmpty()) {
            QMessageBox::warning(this, "Error", "Choose a dataset");
            return;
        }

        refreshStrategyList();
        stacked_widget->setCurrentWidget(select_strategy_screen);
    });

    // create screen buttons
    connect(create, &CreateStrategyScreen::StartScreenSwitch, this, [this] {
        stacked_widget->setCurrentWidget(start_screen);
    });

    connect(create, &CreateStrategyScreen::saveStrategyRequested, this,
            [this, create](const CreateStrategyInput &input) {
                if (input.name.trimmed().isEmpty()) {
                    QMessageBox::warning(this, "Error", "Strategy name cannot be empty");
                    return;
                }

                QSqlDatabase db = StrategyDatabase::database();

                if (!db.transaction()) {
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
                checkQuery.exec();

                if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
                    db.rollback();
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

                if (!strategyQuery.exec()) {
                    db.rollback();
                    QMessageBox::warning(this, "Error", "Idk what is this");
                    return;
                }

                const int strategyId = strategyQuery.lastInsertId().toInt();
                QSqlQuery paramQuery(db);

                if (input.type == "MovingAveragesLongStrategy") {
                    paramQuery.prepare(R"(
                        INSERT INTO strategy_parameters (strategy_id, param_key, param_value, value_type, is_editable)
                        VALUES (?, ?, ?, ?, ?)
                    )");
                    paramQuery.addBindValue(strategyId);
                    paramQuery.addBindValue("short_window");
                    paramQuery.addBindValue(QString::number(input.shortWindow));
                    paramQuery.addBindValue("int");
                    paramQuery.addBindValue(1);

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
                    paramQuery.addBindValue(QString::number(input.longWindow));
                    paramQuery.addBindValue("int");
                    paramQuery.addBindValue(1);

                    if (!paramQuery.exec()) {
                        db.rollback();
                        QMessageBox::warning(this, "Error", "idk what this is");
                        return;
                    }
                } else if (input.type == "StopLossStrategy") {
                    paramQuery.prepare(R"(
                        INSERT INTO strategy_parameters (strategy_id, param_key, param_value, value_type, is_editable)
                        VALUES (?, ?, ?, ?, ?)
                    )");
                    paramQuery.addBindValue(strategyId);
                    paramQuery.addBindValue("stop_loss_percentage");
                    paramQuery.addBindValue(QString::number(input.stopLossPercentage, 'f', 2));
                    paramQuery.addBindValue("double");
                    paramQuery.addBindValue(1);

                    if (!paramQuery.exec()) {
                        db.rollback();
                        QMessageBox::warning(this, "Error", "idk what this is");
                        return;
                    }
                }

                if (!db.commit()) {
                    QMessageBox::warning(this, "Error", "idk what this is");
                    return;
                }

                create->resetForm();
                refreshStrategyList();
                stacked_widget->setCurrentWidget(start_screen);
            });

    // select screen buttons
    connect(select, &SelectStrategyScreen::StartScreenSwitch, this, [this] {
        stacked_widget->setCurrentWidget(start_screen);
    });

    connect(select, &SelectStrategyScreen::StartBacktestSwitch, this, [this, select] {
        const int selectedId = select->selectedStrategyId();
        const QString selectedName = select->selectedStrategyText();

        if (selectedId < 0) {
            QMessageBox::warning(this, "Error", "Choose a strategy");
            return;
        }

        if (currentSelectedDataset.trimmed().isEmpty()) {
            currentSelectedDataset = start_screen ? start_screen->selectedDataset().trimmed() : QString();
        }

        if (currentSelectedDataset.trimmed().isEmpty()) {
            QMessageBox::warning(this, "Error", "Choose a dataset");
            stacked_widget->setCurrentWidget(start_screen);
            return;
        }

        if (!saveAppState(currentSelectedDataset, selectedId)) {
            QMessageBox::warning(this, "Error", "Didn`t save selected dataset and strategy.");
            return;
        }
    });

    // done screen buttons
    connect(done, &DoneScreen::BacktestScreenSwitch, this, [this] {
        stacked_widget->setCurrentWidget(select_strategy_screen);
    });

    connect(done, &DoneScreen::StartScreenSwitch, this, [this] {
        stacked_widget->setCurrentWidget(start_screen);
    });
}

MainWindow::~MainWindow() = default;

void MainWindow::seedStrategiesIfNeeded() {
    QSqlDatabase db = StrategyDatabase::database();
    QSqlQuery countQuery(db);

    if (!countQuery.exec("SELECT COUNT(*) FROM strategies")) {
        return;
    }

    if (countQuery.next() && countQuery.value(0).toInt() > 0) {
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
    strategyQuery.addBindValue(1);

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
    paramQuery.addBindValue(1);

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
    paramQuery.addBindValue(1);

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
    strategyQuery.addBindValue(1);

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
    paramQuery.addBindValue(1);

    if (!paramQuery.exec()) {
        db.rollback();
        return;
    }

    db.commit();
}

void MainWindow::refreshStrategyList() {
    if (!select_strategy_screen) {
        return;
    }

    QList<QPair<QString, int> > items;
    QSqlDatabase db = StrategyDatabase::database();
    QSqlQuery query(db);

    if (!query.exec("SELECT name, id FROM strategies ORDER BY id ASC")) {
        return;
    }

    while (query.next()) {
        items.append({query.value(0).toString(), query.value(1).toInt()});
    }

    select_strategy_screen->setStrategies(items);
}

void MainWindow::loadDatasets() {
    if (!start_screen) {
        return;
    }

    QStringList datasetNames;
    QVector<QString> candidateDirs = {
        QDir::currentPath() + "/data",
        QDir::currentPath() + "/../data"
    };

    for (const QString &path: candidateDirs) {
        QDir dir(path);
        if (!dir.exists()) {
            continue;
        }

        QStringList files = dir.entryList(QStringList() << "*.csv", QDir::Files, QDir::Name);
        for (const QString &file: files) {
            if (!datasetNames.contains(file)) {
                datasetNames.append(file);
            }
        }
    }

    start_screen->setDatasets(datasetNames);
}

bool MainWindow::saveAppState(const QString &dataset, int strategyId) {
    // todo:: link this with the rest of the code so that it can be used for models
    return true;
}
