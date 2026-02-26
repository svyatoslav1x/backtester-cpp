#include "start_screen.h"
#include "style.h"
#include <QSizePolicy>

StartScreen::StartScreen(QWidget *parent) : QWidget(parent) {
    buildUi();
}

void StartScreen::buildUi() {
    main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(20, 10, 20, 10);
    main_layout->setSpacing(10);

    const int controlHeight = 42;

    title_label = new QLabel("Backtester", this);
    title_label->setAlignment(Qt::AlignCenter);
    title_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setLabelStyle(title_label, colors[3], 24, true);
    main_layout->addWidget(title_label, 0);

    main_layout->addSpacing(6);

    news_header = new QLabel("Economic News", this);
    news_header->setAlignment(Qt::AlignCenter);
    news_header->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setLabelStyle(news_header, colors[10], 14, true);
    main_layout->addWidget(news_header, 0);

    news_display = new QTextEdit(this);
    news_display->setReadOnly(true);
    news_display->setMinimumSize(520, 140);
    news_display->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setTextDisplayStyle(news_display, colors[6], colors[3], 10, false);
    main_layout->addWidget(news_display, 0);

    main_layout->addSpacing(8);

    dataset_text = new QLabel("Select Dataset", this);
    dataset_text->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    dataset_text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setLabelStyle(dataset_text, colors[3], 12, true);
    main_layout->addWidget(dataset_text, 0);

    dataset_combo = new QComboBox(this);
    dataset_combo->setMinimumHeight(controlHeight);
    dataset_combo->setMinimumWidth(220);
    dataset_combo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setComboStyle(dataset_combo, colors[6], colors[3], 11, false);
    main_layout->addWidget(dataset_combo, 0);

    main_layout->addSpacing(14);

    button_layout = new QHBoxLayout();
    button_layout->setSpacing(12);

    manage_strategies_button = new QPushButton("Manage Strategies", this);
    manage_strategies_button->setMinimumHeight(42);
    manage_strategies_button->setMinimumWidth(150);
    manage_strategies_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setSecondaryButtonStyle(manage_strategies_button, 11, true);

    create_strategy_button = new QPushButton("Create Strategy", this);
    create_strategy_button->setMinimumHeight(42);
    create_strategy_button->setMinimumWidth(140);
    create_strategy_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setButtonStyle(create_strategy_button, colors[0], colors[1], 11, true);

    start_button = new QPushButton("Start Backtest", this);
    start_button->setMinimumHeight(42);
    start_button->setMinimumWidth(140);
    start_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setButtonStyle(start_button, colors[0], colors[1], 11, true);

    button_layout->addStretch();
    button_layout->addWidget(manage_strategies_button);
    button_layout->addWidget(create_strategy_button);
    button_layout->addWidget(start_button);
    button_layout->addStretch();

    main_layout->addLayout(button_layout);
}

void StartScreen::setNewsHtml(const QString &html) {
    if (news_display) {
        news_display->setHtml(html);
    }
}

void StartScreen::setNewsText(const QString &text) {
    if (news_display) {
        news_display->setPlainText(text);
    }
}

void StartScreen::setDatasets(const QStringList &names) {
    if (!dataset_combo) return;
    dataset_combo->clear();

    if (names.isEmpty()) {
        dataset_combo->addItem("No datasets found");
    } else {
        dataset_combo->addItems(names);
    }
}

QString StartScreen::selectedDataset() const {
    if (dataset_combo) {
        return dataset_combo->currentText();
    }
    return "";
}