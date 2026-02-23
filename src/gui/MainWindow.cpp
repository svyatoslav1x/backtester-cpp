#include "MainWindow.h"

#include "screens/start_screen.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Backtester Application");

    QPointer<QScreen> screen = QGuiApplication::primaryScreen();
    QRect screenSize = screen->geometry();
    int w = screenSize.width();
    int h = screenSize.height();
    resize(w * 0.69, h * 0.67);

    QLinearGradient gradient(0, 0, w, h);
    gradient.setColorAt(0.0, QColor("#BADBA2"));
    gradient.setColorAt(1.0, QColor("#42D674"));

    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(gradient));
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    stacked_widget = new QStackedWidget(this);
    start_screen = new StartScreen(this);

    stacked_widget->addWidget(start_screen);

    stacked_widget->setCurrentIndex(0);
    setCentralWidget(stacked_widget);
}

MainWindow::~MainWindow() {
}
