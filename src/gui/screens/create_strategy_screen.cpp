#include "create_strategy_screen.h"
#include "style.h"

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

    strategy_type_combo->addItem("MovingAveragesLongStrategy");
    strategy_type_combo->addItem("StopLossStrategy");

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

    short_window_spin = new MouseOnlySpinBox(this); // qspinbox for short
    short_window_spin->setRange(5, 50);
    short_window_spin->setValue(12);
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

    long_window_spin = new MouseOnlySpinBox(this); // qspinbox for long
    long_window_spin->setRange(20, 250);
    long_window_spin->setValue(50);
    long_window_spin->setMinimumHeight(42);
    long_window_spin->setFixedWidth(120);
    long_window_spin->setAlignment(Qt::AlignCenter);
    setEditStyle(long_window_spin, QColor(240, 244, 248), colors[3], colors[2], 11, true);
    long_layout->addWidget(long_window_spin);

    connect(short_window_spin, qOverload<int>(&QSpinBox::valueChanged), this, [this](int shortValue) {
        if (long_window_spin->minimum() <= shortValue) {
            long_window_spin->setMinimum(shortValue + 1);
        }
        if (long_window_spin->value() <= shortValue) {
            long_window_spin->setValue(shortValue + 1);
        }
    });

    connect(long_window_spin, qOverload<int>(&QSpinBox::valueChanged), this, [this](int longValue) {
        short_window_spin->setMaximum(longValue - 1);
        if (short_window_spin->value() >= longValue) {
            short_window_spin->setValue(longValue - 1);
        }
    });
    long_window_spin->setMinimum(short_window_spin->value() + 1);
    short_window_spin->setMaximum(long_window_spin->value() - 1);

    long_layout->addStretch();
    layout->addLayout(long_layout);

    stop_loss_layout = new QHBoxLayout();
    stop_loss_layout->addStretch();

    stop_loss_label = new QLabel("Stop Loss Percentage:", this);
    stop_loss_label->setMinimumWidth(280);
    stop_loss_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    setLabelStyle(stop_loss_label, colors[3], 11, false);
    stop_loss_layout->addWidget(stop_loss_label);

    stop_loss_spin = new MouseOnlyDoubleSpinBox(this);
    stop_loss_spin->setRange(0.80, 0.99);
    stop_loss_spin->setSingleStep(0.01);
    stop_loss_spin->setDecimals(2);
    stop_loss_spin->setValue(0.95);
    stop_loss_spin->setMinimumHeight(42);
    stop_loss_spin->setFixedWidth(120);
    stop_loss_spin->setAlignment(Qt::AlignCenter);
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

    button_layout = new QHBoxLayout(); // layout for buttons

    back_button = new QPushButton("Back", this); // button to main screen
    back_button->setMinimumSize(120, 42);
    setSecondaryButtonStyle(back_button, 11, true);
    connect(back_button, &QPushButton::clicked, this, [this] {
        emit StartScreenSwitch();
    });
    button_layout->addWidget(back_button);

    button_layout->addStretch();

    save_button = new QPushButton("Save and Use", this); // button to save and get back to main screen
    save_button->setMinimumSize(150, 42);
    setButtonStyle(save_button, colors[0], colors[1], 11, true);
    // todo: logic for adding a strategy
    connect(save_button, &QPushButton::clicked, this, [this] {
        emit StartScreenSwitch();
    });
    button_layout->addWidget(save_button);

    layout->addLayout(button_layout);

    connect(strategy_type_combo, &QComboBox::currentTextChanged, this, [this] {
        updateParameterVisibility();
    });

    updateParameterVisibility();

    layout->addStretch();
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
    if (stop_loss_spin) {
        out.stopLossPercentage = stop_loss_spin->value();
    }
    return out;
}

void CreateStrategyScreen::resetForm() {
    if (strategy_name_edit) {
        strategy_name_edit->clear();
    }
    if (strategy_type_combo) {
        strategy_type_combo->setCurrentText("MovingAveragesLongStrategy");
    }
    if (short_window_spin) {
        short_window_spin->setValue(20);
    }
    if (long_window_spin) {
        long_window_spin->setValue(50);
    }
    if (stop_loss_spin) {
        stop_loss_spin->setValue(0.90);
    }

    updateParameterVisibility();
}

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
