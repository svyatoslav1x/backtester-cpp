#include "done_screen.h"
#include "style.h"

DoneScreen::DoneScreen(QWidget *parent) : QWidget(parent) {
    buildUi();
}

void DoneScreen::buildUi() {
    main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(20, 10, 20, 10);
    main_layout->addStretch();

    text = new QLabel("Backtester results"); // window text
    text->setAlignment(Qt::AlignCenter);
    setLabelStyle(text, colors[3], 25, true);
    main_layout->addWidget(text, 0, Qt::AlignHCenter);

    final_result = new QTextEdit(this); // for results
    final_result->setReadOnly(true);
    final_result->setMinimumSize(500, 250);
    setTextDisplayStyle(final_result, colors[6], colors[3], 10, false);
    main_layout->addWidget(final_result, 0, Qt::AlignHCenter);

    buttons = new QHBoxLayout();
    buttons->addStretch();

    return_starting_screen = new QPushButton("Return Home"); // button to get to the main window
    return_starting_screen->setMinimumSize(245, 20);
    setSecondaryButtonStyle(return_starting_screen, 11, true);
    connect(return_starting_screen, &QPushButton::clicked, this, [this] {
        emit StartScreenSwitch();
    });
    buttons->addWidget(return_starting_screen);

    return_charts_screen = new QPushButton("Return to Charts"); // return to the charts button
    return_charts_screen->setMinimumSize(245, 20);
    connect(return_charts_screen, &QPushButton::clicked, this, [this] {
        emit BacktestScreenSwitch();
    });
    setButtonStyle(return_charts_screen, colors[0], colors[1], 11, true);
    buttons->addWidget(return_charts_screen);

    buttons->addStretch();

    main_layout->addLayout(buttons);
    main_layout->addStretch();
}

void DoneScreen::setResults(double totalReturn, double sharpeRatio, double maxDrawdown) {
    if (final_result) {
        final_result->setText(
            QString("Total Return: %1\nSharpe Ratio: %2\nMax Drawdown: %3%")
            .arg(totalReturn)
            .arg(sharpeRatio)
            .arg(maxDrawdown)
        );
    }
}
