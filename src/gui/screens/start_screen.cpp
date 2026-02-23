#include "start_screen.h"

void setLabelStyle(QLabel *label, int required_size, bool bold, const QColor &color) {
    QFont f = label->font();
    f.setPointSize(required_size);
    f.setBold(bold);
    label->setFont(f);

    QPalette p = label->palette();
    p.setColor(QPalette::WindowText, color);
    label->setPalette(p);
    label->setAutoFillBackground(false);
}

void applyButtonStyle(QPushButton *button, const QColor &backgroundColor, const QColor &textColor) {
    QFont f = button->font();
    f.setPointSize(13);
    f.setBold(true);
    button->setFont(f);

    QPalette p = button->palette();
    p.setColor(QPalette::Button, backgroundColor);
    p.setColor(QPalette::ButtonText, textColor);
    button->setPalette(p);
    button->setAutoFillBackground(true);
}

StartScreen::StartScreen(QWidget *parent) : QWidget(parent) {
    buildUi();
}

void StartScreen::buildUi() {
    main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(10, 10, 10, 10);

    title_label = new QLabel("Backtester", this);
    setLabelStyle(title_label, 24, true, QColor("black"));
    title_label->setAlignment(Qt::AlignCenter);
    main_layout->addWidget(title_label);
    main_layout->addStretch();

    news_header = new QLabel("Economic News", this);
    setLabelStyle(news_header, 15, true, QColor("black"));
    news_header->setAlignment(Qt::AlignCenter);
    main_layout->addWidget(news_header);
    main_layout->addStretch();

    news_display = new QTextEdit(this);
    news_display->setReadOnly(true);
    news_display->setMinimumHeight(150);
    news_display->setMaximumHeight(200);
    QColor backgroundColor(0, 0, 0, 80);
    QColor textColor(Qt::white);
    QPalette pal = news_display->palette();
    pal.setColor(QPalette::Base, backgroundColor);
    pal.setColor(QPalette::Text, textColor);
    news_display->setPalette(pal);
    news_display->setAutoFillBackground(false);
    QFont newsFont = news_display->font();
    newsFont.setPointSize(10);
    news_display->setFont(newsFont);
    main_layout->addWidget(news_display);

    dataset_text = new QLabel("Select Dataset", this);
    setLabelStyle(dataset_text, 15, true, QColor("black"));
    dataset_text->setAlignment(Qt::AlignCenter);
    main_layout->addWidget(dataset_text);

    dataset_combo = new QComboBox(this);
    dataset_combo->setMinimumHeight(40);
    QFont f = dataset_combo->font();
    f.setPointSize(15);
    dataset_combo->setFont(f);

    QPalette p = dataset_combo->palette();
    p.setColor(QPalette::Base, backgroundColor);
    dataset_combo->setPalette(p);
    dataset_combo->setAutoFillBackground(true);
    main_layout->addWidget(dataset_combo);

    button_layout = new QHBoxLayout();

    manage_strategies_button = new QPushButton("Manage Strategies");
    button_layout->addWidget(manage_strategies_button);
    manage_strategies_button->setMinimumSize(180, 50);
    applyButtonStyle(manage_strategies_button, QColor(255, 192, 203), Qt::black);

    create_strategy_button = new QPushButton("Create Strategy");
    button_layout->addWidget(create_strategy_button);
    create_strategy_button->setMinimumSize(180, 50);
    applyButtonStyle(create_strategy_button, QColor(37, 150, 190), Qt::black);

    start_button = new QPushButton("Start Backtest");
    button_layout->addWidget(start_button);
    start_button->setMinimumSize(180, 50);
    applyButtonStyle(start_button, QColor(128, 0, 128), Qt::black);

    main_layout->addLayout(button_layout);
    main_layout->addStretch();
}
