#include "select_strategy_screen.h"

#include "style.h"

#include "optional"

SelectStrategyScreen::SelectStrategyScreen(QWidget *parent) : QWidget(parent) {
    buildUi();
}

void SelectStrategyScreen::buildUi() {
    layout = new QVBoxLayout(this); // our layout - vertical
    layout->setContentsMargins(20, 10, 20, 10);

    layout->addStretch();

    title = new QLabel("Select Strategy", this); // a big text to emphisize the goal of the window
    title->setAlignment(Qt::AlignCenter);
    setLabelStyle(title, colors[3], 22, true);
    layout->addWidget(title);

    strategy_label = new QLabel("Choose Trading Strategy:", this); // test to explain combo box
    strategy_label->setAlignment(Qt::AlignCenter);
    setLabelStyle(strategy_label, colors[3], 12, true);
    layout->addWidget(strategy_label);

    strategy_combo = new QComboBox(this); // combo box for strategies
    strategy_combo->setMinimumHeight(42);
    strategy_combo->setMinimumWidth(325);
    setComboStyle(strategy_combo, colors[6], colors[3], 11, false);
    layout->addWidget(strategy_combo, 0, Qt::AlignCenter);

    button_layout = new QHBoxLayout(); // horizontal box for buttons
    button_layout->addStretch();

    back_button = new QPushButton("Back"); // button to the main window
    back_button->setMinimumSize(120, 42);
    setSecondaryButtonStyle(back_button, 11, true);
    connect(back_button, &QPushButton::clicked, this, [this] {
        emit StartScreenSwitch();
    });
    button_layout->addWidget(back_button);

    next_button = new QPushButton("Start Backtest"); // button to start backtest
    next_button->setMinimumSize(170, 42);
    setButtonStyle(next_button, colors[0], colors[1], 11, true);
    connect(next_button, &QPushButton::clicked, this, [this] {
        emit StartBacktestSwitch();
    });
    button_layout->addWidget(next_button);

    button_layout->addStretch();

    layout->addLayout(button_layout);
    layout->addStretch();
}


void SelectStrategyScreen::setStrategies(const QList<QPair<QString, int> > &items) {
    if (!strategy_combo) {
        return;
    }

    strategy_combo->clear();

    if (items.isEmpty()) {
        strategy_combo->addItem("No strategies available", -1);
        return;
    }

    for (const auto &item: items) {
        strategy_combo->addItem(item.first, item.second);
    }
}

std::optional<int> SelectStrategyScreen::selectedStrategyId() const {
    if (!strategy_combo) {
        return std::nullopt;
    }

    const QVariant data = strategy_combo->currentData();
    if (!data.isValid()) {
        return std::nullopt;
    }

    const int id = data.toInt();
    if (id == -1) {
        return std::nullopt;
    }

    return id;
}

QComboBox *SelectStrategyScreen::strategyCombo() const {
    return strategy_combo;
}

QPushButton *SelectStrategyScreen::backButton() const {
    return back_button;
}

QPushButton *SelectStrategyScreen::startBacktestButton() const {
    return next_button;
}
