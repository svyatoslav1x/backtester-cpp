MainWindow::MainWindow(QWidget* parent): QMainWindow(parent) {
    setWindowTitle("Backtester Application");

    Qpointer<QScreen> screen = QGuiApplication::primaryScreen();
    QRect screenSize = screen->geometry();
    int width = screenGeometry.width();
    int height = screenGeometry.height();
    resize(width * 0.69, height * 0.67);

    stacked_widget->addWidget(start_screen);
    stacked_widget->addWidget(strategy_screen);
    stacked_widget->addWidget(create_strategy_screen);
    stacked_widget->addWidget(manage_strategies_screen);
    stacked_widget->addWidget(backtest_screen);
    stacked_widget->addWidget(done_screen);

    stacked_widget->setCurrentIndex(0);
};