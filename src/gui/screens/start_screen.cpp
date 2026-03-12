#include "start_screen.h"
#include "style.h"
#include "../MainWindow.h"

StartScreen::StartScreen(QWidget *parent) : QWidget(parent) {
    buildUi();
}

void StartScreen::buildUi() {
    main_layout = new QVBoxLayout(this); // layout for the screen
    main_layout->setContentsMargins(20, 10, 20, 10);

    title_label = new QLabel("Backtester", this); // label at the top of the screen
    title_label->setAlignment(Qt::AlignCenter);
    setLabelStyle(title_label, colors[3], 24, true);
    main_layout->addWidget(title_label);

    news_header = new QLabel("Economic News", this); // Text for news
    news_header->setAlignment(Qt::AlignCenter);
    setLabelStyle(news_header, colors[10], 14, true);
    main_layout->addWidget(news_header);

    news_display = new QTextEdit(this); // placeholder for news
    news_display->setReadOnly(true);
    news_display->setMinimumSize(520, 140);
    setTextDisplayStyle(news_display, colors[6], colors[3], 10, false);
    main_layout->addWidget(news_display);

    dataset_text = new QLabel("Select Dataset", this); // Text to show what the qcombobox is about
    setLabelStyle(dataset_text, colors[3], 12, true);
    main_layout->addWidget(dataset_text);

    dataset_combo = new QComboBox(this); // choices for dataset
    dataset_combo->setMinimumHeight(42);
    dataset_combo->setMinimumWidth(220);
    setComboStyle(dataset_combo, colors[6], colors[3], 11, false);
    main_layout->addWidget(dataset_combo);

    button_layout = new QHBoxLayout();

    manage_strategies_button = new QPushButton("Manage Strategies", this); // button to manage strategies
    manage_strategies_button->setMinimumHeight(42);
    manage_strategies_button->setMinimumWidth(150);
    connect(manage_strategies_button, &QPushButton::clicked, this, [this] {
        emit manageStrategiesSwitch();
    });
    setSecondaryButtonStyle(manage_strategies_button, 11, true);

    create_strategy_button = new QPushButton("Create Strategy", this); // button to create a strategy
    create_strategy_button->setMinimumHeight(42);
    create_strategy_button->setMinimumWidth(140);
    connect(create_strategy_button, &QPushButton::clicked, this, [this] {
        emit createStrategySwitch();
    });
    setButtonStyle(create_strategy_button, colors[0], colors[1], 11, true);

    start_button = new QPushButton("Select Strategy", this); // button to start backtest
    start_button->setMinimumHeight(42);
    start_button->setMinimumWidth(140);
    connect(start_button, &QPushButton::clicked, this, [this] {
        emit startBacktestSwitch();
    });
    setButtonStyle(start_button, colors[0], colors[1], 11, true);

    button_layout->addWidget(manage_strategies_button);
    button_layout->addWidget(create_strategy_button);
    button_layout->addWidget(start_button);

    main_layout->addLayout(button_layout);
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

QComboBox *StartScreen::datasetCombo() const {
    return dataset_combo;
}

QTextEdit *StartScreen::newsTextEdit() const {
    return news_display;
}


QPushButton *StartScreen::startButton() const {
    return start_button;
}

QPushButton *StartScreen::createStrategyButton() const {
    return create_strategy_button;
}

QPushButton *StartScreen::manageStrategiesButton() const {
    return manage_strategies_button;
}

QString StartScreen::newsText() const {
    return news_display ? news_display->toPlainText() : QString{};
}
