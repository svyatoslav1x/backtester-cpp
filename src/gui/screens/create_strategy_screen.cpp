#include "create_strategy_screen.h"
#include "style.h"
#include <QSizePolicy>

CreateStrategyScreen::CreateStrategyScreen(QWidget *parent) : QWidget(parent) {
    buildUi();
}

void CreateStrategyScreen::buildUi() {
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 10, 20, 10);
    layout->addStretch();

    title = new QLabel("Create Custom Strategy", this); // title for the window
    title->setAlignment(Qt::AlignCenter);
    setLabelStyle(title, colors[3], 24, true);
    layout->addWidget(title, 0, Qt::AlignHCenter);

    name_label = new QLabel("Strategy Name:", this); // text to introduce the naming for a strategy
    setLabelStyle(name_label, colors[3], 11, true);
    layout->addWidget(name_label, 0, Qt::AlignLeft);

    strategy_name_edit = new QLineEdit(this); // a place to enter the name of a strategy
    strategy_name_edit->setPlaceholderText("Enter a unique name for your strategy");
    strategy_name_edit->setMinimumSize(260, 42);
    setEditStyle(strategy_name_edit, colors[6], colors[3], colors[2], 11, false);
    layout->addWidget(strategy_name_edit);

    type_label = new QLabel("Strategy Type:", this); // introduce a place to choose a strategy type
    setLabelStyle(type_label, colors[3], 11, true);
    layout->addWidget(type_label, 0, Qt::AlignLeft);

    strategy_type_combo = new QComboBox(this); // choice for a strategy type
    strategy_type_combo->setMinimumSize(260, 42);
    setComboStyle(strategy_type_combo, colors[6], colors[3], 11, false);
    layout->addWidget(strategy_type_combo);

    params_label = new QLabel("Strategy Parameters", this); // text to introduce changes of parameters
    setLabelStyle(params_label, colors[10], 13, true);
    layout->addWidget(params_label, 0, Qt::AlignCenter);

    short_layout = new QHBoxLayout(); // for short text + qspinbox
    short_layout->addStretch();

    short_label = new QLabel("Short Moving Average Window:", this); // text for short
    short_label->setMinimumWidth(280);
    short_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    setLabelStyle(short_label, colors[3], 11, false);
    short_layout->addWidget(short_label);

    short_window_spin = new QSpinBox(this); // qspinbox for short
    short_window_spin->setRange(5, 100);
    short_window_spin->setValue(20);
    short_window_spin->setMinimumHeight(42);
    short_window_spin->setFixedWidth(120);
    short_window_spin->setAlignment(Qt::AlignCenter);
    setEditStyle(short_window_spin, colors[12], colors[3], colors[2], 11, true);
    short_layout->addWidget(short_window_spin);

    short_layout->addStretch();
    layout->addLayout(short_layout);

    long_layout = new QHBoxLayout(); // a layout for long
    long_layout->addStretch();

    long_label = new QLabel("Long Moving Average Window:", this); // text for long
    long_label->setMinimumWidth(280);
    long_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    setLabelStyle(long_label, colors[3], 11, false);
    long_layout->addWidget(long_label);

    long_window_spin = new QSpinBox(this); // qspinbox for long
    long_window_spin->setRange(25, 150);
    long_window_spin->setValue(50);
    long_window_spin->setMinimumHeight(42);
    long_window_spin->setFixedWidth(120);
    long_window_spin->setAlignment(Qt::AlignCenter);
    setEditStyle(long_window_spin, QColor(240, 244, 248), colors[3], colors[2], 11, true);
    long_layout->addWidget(long_window_spin);

    long_layout->addStretch();
    layout->addLayout(long_layout);

    info_label = new QLabel(
        "Tips:\n"
        "• Short window: reacts faster to price changes\n"
        "• Long window: confirms the broader trend\n"
        "• Golden Cross: buy signal (Short crosses above Long)\n"
        "• Death Cross: sell signal (Short crosses below Long)",
        this
    ); // educate them users with this
    info_label->setAlignment(Qt::AlignCenter);
    info_label->setWordWrap(true);
    setFilledLabelStyle(info_label, colors[3], colors[5], 10, false);
    layout->addWidget(info_label);

    button_layout = new QHBoxLayout(); // layout for buttons

    back_button = new QPushButton("Back", this); // button to main screen
    back_button->setMinimumSize(120, 42);
    setSecondaryButtonStyle(back_button, 11, true);
    button_layout->addWidget(back_button);

    button_layout->addStretch();

    save_button = new QPushButton("Save and Use", this); // button to save and get back to main screen
    save_button->setMinimumSize(150, 42);
    setButtonStyle(save_button, colors[0], colors[1], 11, true);
    button_layout->addWidget(save_button);

    layout->addLayout(button_layout);
    layout->addStretch(); // todo: consider what is actually better, for it to be horizontally fixed or not
}

CreateStrategyInput CreateStrategyScreen::input() const {
    CreateStrategyInput out;
    if (strategy_name_edit) {
        out.name = strategy_name_edit->text().trimmed();
    }
    if (strategy_type_combo) {
        out.type = strategy_type_combo->currentText();
    }
    if (short_window_spin) {
        out.shortWindow = short_window_spin->value();
    }
    if (long_window_spin) {
        out.longWindow = long_window_spin->value();
    }
    return out;
}

void CreateStrategyScreen::resetForm() {
    if (strategy_name_edit) {
        strategy_name_edit->clear();
    }
    if (short_window_spin) {
        short_window_spin->setValue(20);
    }
    if (long_window_spin) {
        long_window_spin->setValue(50);
    }
}
