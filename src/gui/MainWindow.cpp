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

    stacked_widget->setCurrentIndex(1);
    setCentralWidget(stacked_widget);
}

MainWindow::~MainWindow() = default;