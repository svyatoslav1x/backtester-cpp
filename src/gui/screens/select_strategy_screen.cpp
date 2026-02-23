#include "select_strategy_screen.h"
#include "style.h"

QColor backgroundColor(0, 0, 0, 80);

SelectStrategyScreen::SelectStrategyScreen(QWidget *parent) : QWidget(parent) {
    buildUi();
}

void SelectStrategyScreen::buildUi() {
    layout = new QVBoxLayout(this);

    layout->addStretch();
    layout->setContentsMargins(50, 10, 50, 10);

    title = new QLabel("Select Strategy", this);
    title->setAlignment(Qt::AlignCenter);
    setLabelStyle(title, 25, true, QColor("black"));
    layout->addWidget(title);

    strategy_label = new QLabel("Choose Trading Strategy:", this);
    strategy_label->setAlignment(Qt::AlignCenter);
    setLabelStyle(strategy_label, 13, true, QColor("black"));
    layout->addWidget(strategy_label);

    strategy_combo = new QComboBox(this);
    strategy_combo->setMinimumHeight(40);
    strategy_combo->setMinimumWidth(350);
    strategy_combo->setMaximumWidth(400);
    QFont f = strategy_combo->font();
    f.setPointSize(15);
    strategy_combo->setFont(f);
    QPalette p = strategy_combo->palette();
    p.setColor(QPalette::Base, backgroundColor);
    strategy_combo->setPalette(p);
    strategy_combo->setAutoFillBackground(true);
    layout->addWidget(strategy_combo, 0, Qt::AlignCenter);

    button_layout = new QHBoxLayout();
    button_layout->addStretch();

    back_button = new QPushButton("Back");
    button_layout->addWidget(back_button);
    back_button->setMinimumSize(150, 45);
    setButtonStyle(back_button, QColor(208, 0, 255), QColor("black"));

    button_layout->addSpacing(15);

    next_button = new QPushButton("Start Backtest");
    button_layout->addWidget(next_button);
    next_button->setMinimumSize(200, 45);
    setButtonStyle(next_button, QColor(37, 150, 190), QColor("black"));
    button_layout->addStretch();

    layout->addLayout(button_layout);
    layout->addStretch();
}
