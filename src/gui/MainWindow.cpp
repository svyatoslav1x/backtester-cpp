#include "MainWindow.h"

#include "screens/create_strategy_screen.h"
#include "screens/done_screen.h"
#include "screens/select_strategy_screen.h"
#include "screens/start_screen.h"

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
    this->setPalette(palette);
    this->setAutoFillBackground(true);

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

    auto start = qobject_cast<StartScreen *>(stacked_widget->widget(0));
    auto select = qobject_cast<SelectStrategyScreen *>(stacked_widget->widget(1));
    auto create = qobject_cast<CreateStrategyScreen *>(stacked_widget->widget(2));
    auto done = qobject_cast<DoneScreen *>(stacked_widget->widget(3));

    // buttons on start screen
    connect(start, &StartScreen::manageStrategiesSwitch, this, [this] {
        stacked_widget->setCurrentWidget(select_strategy_screen);
    });

    connect(start, &StartScreen::createStrategySwitch, this, [this] {
        stacked_widget->setCurrentWidget(create_strategy_screen);
    });

    connect(start, &StartScreen::startBacktestSwitch, this, [this](const QString &dataset) {
        // todo: add the function to save dataset
        stacked_widget->setCurrentWidget(select_strategy_screen);
    });

    // buttons on select strategies screen
    connect(select, &SelectStrategyScreen::StartScreenSwitch, this, [this] {
        stacked_widget->setCurrentIndex(0);
    });

    connect(select, &SelectStrategyScreen::StartBacktestSwitch, this, [this] {
        // stacked_widget->setCurrentIndex(); waiting on Nikita
    });

    // buttons on create strategies screen
    connect(create, &CreateStrategyScreen::StartScreenSwitch, this, [this] {
        stacked_widget->setCurrentIndex(0);
    });

    // buttons on done screem
    connect(done, &DoneScreen::BacktestScreenSwitch, this, [this] {
        // stacked_widget->setCurrentIndex(); waiting on Nikita
    });

    connect(done, &DoneScreen::StartScreenSwitch, this, [this] {
        stacked_widget->setCurrentIndex(0);
    });
}

MainWindow::~MainWindow() = default;
