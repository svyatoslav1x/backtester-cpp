#include "select_strategy_screen.h"
#include "style.h"
#include <QSizePolicy>

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
    button_layout->addWidget(back_button);

    next_button = new QPushButton("Start Backtest"); // button to start backtest
    next_button->setMinimumSize(170, 42);
    setButtonStyle(next_button, colors[0], colors[1], 11, true);
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

int SelectStrategyScreen::selectedStrategyId() const {
    return strategy_combo ? strategy_combo->currentData().toInt() : -1;
}

QString SelectStrategyScreen::selectedStrategyText() const {
    return strategy_combo ? strategy_combo->currentText() : QString();
}
