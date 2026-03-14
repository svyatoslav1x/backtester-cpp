#include "create_strategy_screen.h"
#include "style.h"

CreateStrategyScreen::CreateStrategyScreen(QWidget *parent) : QWidget(parent) {
    buildUi();
}

void CreateStrategyScreen::buildUi() {
    // creating the main vertical layout
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 10, 20, 10);
    layout->addStretch();

    // label for the screen
    title = new QLabel("Create Custom Strategy", this);
    title->setAlignment(Qt::AlignCenter);
    setLabelStyle(title, colors[3], 24, true);
    layout->addWidget(title);

    // label to introduce the possibility to give a strategy name
    name_label = new QLabel("Strategy Name:", this);
    setLabelStyle(name_label, colors[3], 11, true);
    layout->addWidget(name_label);

    // a place to add the name for a strategy
    strategy_name_edit = new QLineEdit(this);
    strategy_name_edit->setPlaceholderText("Enter a unique name for your strategy");
    strategy_name_edit->setMinimumSize(260, 42);
    setEditStyle(strategy_name_edit, colors[6], colors[3], colors[2], 11, false);
    layout->addWidget(strategy_name_edit);

    //introducing the possibility to choose a strategy type
    type_label = new QLabel("Strategy Type:", this);
    setLabelStyle(type_label, colors[3], 11, true);
    layout->addWidget(type_label);

    // combo box for types of strategies
    strategy_type_combo = new QComboBox(this);
    strategy_type_combo->setMinimumSize(260, 42);
    setComboStyle(strategy_type_combo, colors[6], colors[3], 11, false);
    layout->addWidget(strategy_type_combo);

    // in the project we only have 2, so we mainly add them here
    strategy_type_combo->addItem("MovingAveragesLongStrategy");
    strategy_type_combo->addItem("StopLossStrategy");
    // label to introduce the possibility to change the parameters of a strategy
    params_label = new QLabel("Strategy Parameters", this);
    params_label->setAlignment(Qt::AlignCenter);
    setLabelStyle(params_label, colors[10], 13, true);
    layout->addWidget(params_label);

    // layout to accurately have text + mousebox for short window
    short_layout = new QHBoxLayout();
    short_layout->addStretch();

    // text for changing the short moving average window
    short_label = new QLabel("Short Moving Average Window:", this);
    short_label->setMinimumWidth(280);
    short_label->setAlignment(Qt::AlignVCenter);
    setLabelStyle(short_label, colors[3], 11, false);
    short_layout->addWidget(short_label);

    // the spin box (my own class that blocks actions on keyboard) for smaw
    short_window_spin = new MouseOnlySpinBox(this);
    short_window_spin->setRange(5, 50);
    short_window_spin->setValue(12);
    short_window_spin->setMinimumHeight(42);
    short_window_spin->setFixedWidth(120);
    short_window_spin->setAlignment(Qt::AlignVCenter);
    setEditStyle(short_window_spin, colors[12], colors[3], colors[2], 11, true);
    short_layout->addWidget(short_window_spin);

    short_layout->addStretch();
    layout->addLayout(short_layout);

    // layout to accurately have text + mousebox for long window
    long_layout = new QHBoxLayout();
    long_layout->addStretch();

    // text for changing the long moving average window
    long_label = new QLabel("Long Moving Average Window:", this);
    long_label->setMinimumWidth(280);
    long_label->setAlignment(Qt::AlignVCenter);
    setLabelStyle(long_label, colors[3], 11, false);
    long_layout->addWidget(long_label);

    // the spin box (my own class that blocks actions on keyboard) for lmaw
    long_window_spin = new MouseOnlySpinBox(this);
    long_window_spin->setRange(20, 250);
    long_window_spin->setValue(50);
    long_window_spin->setMinimumHeight(42);
    long_window_spin->setFixedWidth(120);
    long_window_spin->setAlignment(Qt::AlignVCenter);
    setEditStyle(long_window_spin, QColor(240, 244, 248), colors[3], colors[2], 11, true);
    long_layout->addWidget(long_window_spin);

    // if there is a change in value for the short window spinbox, then the minimum for long window is updated
    connect(short_window_spin, qOverload<int>(&QSpinBox::valueChanged), this, [this](int shortValue) {
        long_window_spin->setMinimum(shortValue + 1);
        if (long_window_spin->value() <= shortValue) {
            long_window_spin->setValue(shortValue + 1);
        }
    });

    // if there is a change in value for the long window spinbox, then the mazimum for short window is updated
    connect(long_window_spin, qOverload<int>(&QSpinBox::valueChanged), this, [this](int longValue) {
        short_window_spin->setMaximum(longValue - 1);
        if (short_window_spin->value() >= longValue) {
            short_window_spin->setValue(longValue - 1);
        }
    });
    // setting the beginning minimum
    long_window_spin->setMinimum(short_window_spin->value() + 1);
    short_window_spin->setMaximum(long_window_spin->value() - 1);

    long_layout->addStretch();
    layout->addLayout(long_layout);

    // layout for when the type of strategy is stop loss
    stop_loss_layout = new QHBoxLayout();
    stop_loss_layout->addStretch();

    // label to introduce the spin box
    stop_loss_label = new QLabel("Stop Loss Percentage:", this);
    stop_loss_label->setMinimumWidth(280);
    stop_loss_label->setAlignment(Qt::AlignVCenter);
    setLabelStyle(stop_loss_label, colors[3], 11, false);
    stop_loss_layout->addWidget(stop_loss_label);

    // no keyboard spinbox class for double numbers
    stop_loss_spin = new MouseOnlyDoubleSpinBox(this);
    stop_loss_spin->setRange(0.01, 0.99);
    stop_loss_spin->setSingleStep(0.01);
    stop_loss_spin->setDecimals(2);
    stop_loss_spin->setValue(0.95);
    stop_loss_spin->setMinimumHeight(42);
    stop_loss_spin->setFixedWidth(120);
    stop_loss_spin->setAlignment(Qt::AlignVCenter);
    setEditStyle(stop_loss_spin, colors[12], colors[3], colors[2], 11, true);
    stop_loss_layout->addWidget(stop_loss_spin);

    stop_loss_layout->addStretch();
    layout->addLayout(stop_loss_layout);

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

    // horizontal layout for buttons at the bottom
    button_layout = new QHBoxLayout();

    // a button to start screen
    back_button = new QPushButton("Back", this);
    back_button->setMinimumSize(120, 42);
    setSecondaryButtonStyle(back_button, 11, true);
    connect(back_button, &QPushButton::clicked, this, [this] {
        emit StartScreenSwitch();
    });
    button_layout->addWidget(back_button);

    button_layout->addStretch();

    // a button to start screen with the save of the strategy
    save_button = new QPushButton("Save", this);
    save_button->setMinimumSize(150, 42);
    setButtonStyle(save_button, colors[0], colors[1], 11, true);
    connect(save_button, &QPushButton::clicked, this, [this] {
        emit saveStrategyRequested(input());
    });
    button_layout->addWidget(save_button);

    layout->addLayout(button_layout);

    // if the type is changed, the visible parts have to change too
    connect(strategy_type_combo, &QComboBox::currentTextChanged, this, [this] {
        updateParameterVisibility();
    });

    updateParameterVisibility();

    layout->addStretch();
}

// a function to get what has been
CreateStrategyInput CreateStrategyScreen::input() const {
    CreateStrategyInput out;
    if (strategy_name_edit) {
        out.name = strategy_name_edit->text().trimmed();
    }
    if (strategy_type_combo) {
        out.type = strategy_type_combo->currentText();
    }

    if (out.type == "MovingAveragesLongStrategy") {
        out.params = MovingAverageParams{
            short_window_spin ? short_window_spin->value() : 12,
            long_window_spin ? long_window_spin->value() : 50
        };
    } else {
        out.params = StopLossParams{
            stop_loss_spin ? stop_loss_spin->value() : 0.95
        };
    }

    return out;
}

// cleaning what the person has put in
void CreateStrategyScreen::resetForm() {
    if (strategy_name_edit) {
        strategy_name_edit->clear();
    }
    if (strategy_type_combo) {
        strategy_type_combo->setCurrentText("MovingAveragesLongStrategy");
    }
    if (short_window_spin) {
        short_window_spin->setValue(12);
    }
    if (long_window_spin) {
        long_window_spin->setValue(50);
    }
    if (stop_loss_spin) {
        stop_loss_spin->setValue(0.95);
    }

    updateParameterVisibility();
}

// change what's visible in accordance with the type of a strategy the person has chosen
void CreateStrategyScreen::updateParameterVisibility() {
    const QString type = strategy_type_combo ? strategy_type_combo->currentText() : "";

    const bool isMovingAverage = (type == "MovingAveragesLongStrategy");
    const bool isStopLoss = (type == "StopLossStrategy");

    short_label->setVisible(isMovingAverage);
    short_window_spin->setVisible(isMovingAverage);

    long_label->setVisible(isMovingAverage);
    long_window_spin->setVisible(isMovingAverage);

    stop_loss_label->setVisible(isStopLoss);
    stop_loss_spin->setVisible(isStopLoss);

    if (info_label) {
        if (isMovingAverage) {
            info_label->setText(
                "Tips:\n"
                "• Short window: reacts faster to price changes\n"
                "• Long window: confirms the broader trend\n"
                "• Golden Cross: buy signal (Short crosses above Long)\n"
                "• Death Cross: sell signal (Short crosses below Long)"
            );
        } else if (isStopLoss) {
            info_label->setText(
                "Tips:\n"
                "• Stop loss percentage is stored as a fraction\n"
                "• Example: 0.90 means exit around 10% below price\n"
                "• Higher values trail more tightly\n"
                "• Lower values give the trade more room"
            );
        }
    }
}

// the following are getters for the test
QLineEdit *CreateStrategyScreen::nameEdit() const {
    return strategy_name_edit;
}

QComboBox *CreateStrategyScreen::typeCombo() const {
    return strategy_type_combo;
}

QSpinBox *CreateStrategyScreen::shortWindowSpin() const {
    return short_window_spin;
}

QSpinBox *CreateStrategyScreen::longWindowSpin() const {
    return long_window_spin;
}

QDoubleSpinBox *CreateStrategyScreen::stopLossSpin() const {
    return stop_loss_spin;
}

QPushButton *CreateStrategyScreen::saveButton() const {
    return save_button;
}

QPushButton *CreateStrategyScreen::backButton() const {
    return back_button;
}
