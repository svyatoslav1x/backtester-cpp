#include "create_strategy_screen.h"
#include "style.h"
#include <QSizePolicy>

CreateStrategyScreen::CreateStrategyScreen(QWidget *parent) : QWidget(parent) {
    buildUi();
}

void CreateStrategyScreen::buildUi() {
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 10, 20, 10);
    layout->setSpacing(8);

    const int controlHeight = 42;
    const int spinWidth = 120;
    const int labelMinWidth = 280;

    layout->addStretch();

    form_container = new QWidget(this);
    form_container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    form_layout = new QVBoxLayout(form_container);
    form_layout->setContentsMargins(0, 0, 0, 0);
    form_layout->setSpacing(8);

    title = new QLabel("Create Custom Strategy", this);
    title->setAlignment(Qt::AlignCenter);
    title->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setLabelStyle(title, colors[3], 24, true);
    form_layout->addWidget(title, 0, Qt::AlignHCenter);

    form_layout->addSpacing(4);

    name_label = new QLabel("Strategy Name:", this);
    name_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setLabelStyle(name_label, colors[3], 11, true);
    form_layout->addWidget(name_label, 0, Qt::AlignLeft);

    strategy_name_edit = new QLineEdit(this);
    strategy_name_edit->setPlaceholderText("Enter a unique name for your strategy");
    strategy_name_edit->setMinimumHeight(controlHeight);
    strategy_name_edit->setMinimumWidth(260);
    strategy_name_edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setEditStyle(strategy_name_edit, colors[6], colors[3], colors[2], 11, false);
    form_layout->addWidget(strategy_name_edit);

    form_layout->addSpacing(2);

    type_label = new QLabel("Strategy Type:", this);
    type_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setLabelStyle(type_label, colors[3], 11, true);
    form_layout->addWidget(type_label, 0, Qt::AlignLeft);

    strategy_type_combo = new QComboBox(this);
    strategy_type_combo->setMinimumHeight(controlHeight);
    strategy_type_combo->setMinimumWidth(260);
    strategy_type_combo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setComboStyle(strategy_type_combo, colors[6], colors[3], 11, false);
    form_layout->addWidget(strategy_type_combo);

    form_layout->addSpacing(6);

    params_label = new QLabel("Strategy Parameters", this);
    params_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setLabelStyle(params_label, colors[10], 13, true);
    form_layout->addWidget(params_label, 0, Qt::AlignCenter);

    short_row = new QWidget(this);
    short_row->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    short_layout = new QHBoxLayout(short_row);
    short_layout->setContentsMargins(0, 0, 0, 0);
    short_layout->setSpacing(6);

    short_layout->addStretch();

    short_label = new QLabel("Short Moving Average Window:", this);
    short_label->setMinimumWidth(labelMinWidth);
    short_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    short_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setLabelStyle(short_label, colors[3], 11, false);
    short_layout->addWidget(short_label, 0, Qt::AlignVCenter);

    short_window_spin = new QSpinBox(this);
    short_window_spin->setRange(5, 100);
    short_window_spin->setValue(20);
    short_window_spin->setMinimumHeight(controlHeight);
    short_window_spin->setFixedWidth(spinWidth);
    short_window_spin->setAlignment(Qt::AlignCenter);
    short_window_spin->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setEditStyle(short_window_spin, QColor(240, 244, 248), colors[3], colors[2], 11, true);
    short_layout->addWidget(short_window_spin, 0, Qt::AlignVCenter);

    short_layout->addStretch();

    form_layout->addWidget(short_row);

    long_row = new QWidget(this);
    long_row->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    long_layout = new QHBoxLayout(long_row);
    long_layout->setContentsMargins(0, 0, 0, 0);
    long_layout->setSpacing(6);

    long_layout->addStretch();

    long_label = new QLabel("Long Moving Average Window:", this);
    long_label->setMinimumWidth(labelMinWidth);
    long_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    long_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setLabelStyle(long_label, colors[3], 11, false);
    long_layout->addWidget(long_label, 0, Qt::AlignVCenter);

    long_window_spin = new QSpinBox(this);
    long_window_spin->setRange(25, 150);
    long_window_spin->setValue(50);
    long_window_spin->setMinimumHeight(controlHeight);
    long_window_spin->setFixedWidth(spinWidth);
    long_window_spin->setAlignment(Qt::AlignCenter);
    long_window_spin->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setEditStyle(long_window_spin, QColor(240, 244, 248), colors[3], colors[2], 11, true);
    long_layout->addWidget(long_window_spin, 0, Qt::AlignVCenter);

    long_layout->addStretch();

    form_layout->addWidget(long_row);

    form_layout->addSpacing(6);

    info_label = new QLabel(
        "Tips:\n"
        "• Short window: reacts faster to price changes\n"
        "• Long window: confirms the broader trend\n"
        "• Golden Cross: buy signal (Short crosses above Long)\n"
        "• Death Cross: sell signal (Short crosses below Long)",
        this
    );
    info_label->setAlignment(Qt::AlignCenter);
    info_label->setWordWrap(true);
    info_label->setMargin(10);
    info_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setFilledLabelStyle(info_label, colors[3], colors[5], 10, false);
    form_layout->addWidget(info_label);

    form_layout->addSpacing(6);

    button_layout = new QHBoxLayout();
    button_layout->setSpacing(14);

    back_button = new QPushButton("Back", this);
    back_button->setMinimumSize(120, 42);
    back_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setSecondaryButtonStyle(back_button, 11, true);
    button_layout->addWidget(back_button);

    button_layout->addStretch();

    save_button = new QPushButton("Save and Use", this);
    save_button->setMinimumSize(150, 42);
    save_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setButtonStyle(save_button, colors[0], colors[1], 11, true);
    button_layout->addWidget(save_button);

    form_layout->addLayout(button_layout);

    layout->addWidget(form_container);
    layout->addStretch();
}
