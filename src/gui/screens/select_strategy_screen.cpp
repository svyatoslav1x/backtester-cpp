#include "select_strategy_screen.h"
#include "style.h"
#include <QSizePolicy>

SelectStrategyScreen::SelectStrategyScreen(QWidget *parent) : QWidget(parent) {
    buildUi();
}

void SelectStrategyScreen::buildUi() {
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 10, 20, 10);
    layout->setSpacing(10);

    layout->addStretch();

    title = new QLabel("Select Strategy", this);
    title->setAlignment(Qt::AlignCenter);
    title->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setLabelStyle(title, colors[3], 22, true);
    layout->addWidget(title);

    strategy_label = new QLabel("Choose Trading Strategy:", this);
    strategy_label->setAlignment(Qt::AlignCenter);
    strategy_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setLabelStyle(strategy_label, colors[3], 12, true);
    layout->addWidget(strategy_label);

    layout->addSpacing(4);

    strategy_combo = new QComboBox(this);
    strategy_combo->setMinimumHeight(42);
    strategy_combo->setMinimumWidth(220);
    strategy_combo->setMaximumWidth(600);
    strategy_combo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setComboStyle(strategy_combo, colors[6], colors[3], 11, false);
    layout->addWidget(strategy_combo, 0, Qt::AlignCenter);

    layout->addSpacing(12);

    button_layout = new QHBoxLayout();
    button_layout->setSpacing(14);
    button_layout->addStretch();

    back_button = new QPushButton("Back");
    back_button->setMinimumSize(120, 42);
    back_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setSecondaryButtonStyle(back_button, 11, true);
    button_layout->addWidget(back_button);

    next_button = new QPushButton("Start Backtest");
    next_button->setMinimumSize(170, 42);
    next_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setButtonStyle(next_button, colors[0], colors[1], 11, true);
    button_layout->addWidget(next_button);

    button_layout->addStretch();

    layout->addLayout(button_layout);
    layout->addStretch();
}