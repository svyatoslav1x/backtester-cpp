#include "StrategyManager.h"

#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QVBoxLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>

StrategyManager::StrategyManager(QWidget* parent)
: QWidget(parent) {
    setup_database();
    setup_ui();
    load_strategies();
}

StrategyManager::~StrategyManager() {
    if (db.isOpen()) {
        db.close();
    }
}

// Sets up the SQLite database where strategies are saved
void StrategyManager::setup_database() {

    // Finds a place on the user's computer to save the file
    QString data_path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    dir.mkpath(data_path);

    QString db_path = QDir(data_path).filePath("strategies.db"); // The path to database file "strategies.db"

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(db_path);

    if (!db.open()) {
        QMessageBox::critical(this, "Database Error",
            "Failed to open database: " + db.lastError().text());
        return;
    }

    // Creates the "strategies" table if it's the first time the app is run
    QSqlQuery query(db);
    QString create_table = R"(
        CREATE TABLE IF NOT EXISTS strategies (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT UNIQUE NOT NULL,
            type TEXT NOT NULL,
            short_window INTEGER NOT NULL,
            long_window INTEGER NOT NULL
        )
    )";

    if (!query.exec(create_table)) {
        QMessageBox::critical(this, "Database Error",
            "Failed to create table: " + query.lastError().text());
    }

    query.exec("SELECT COUNT(*) FROM strategies");
    if (query.next() && query.value(0).toInt() == 0) {
        // If empty, adds a default strategy
        add_strategy("Default MA Strategy", "Moving Average Crossover", 50, 200);
    }
}

// Adds a new strategy to the database
bool StrategyManager::add_strategy(const QString& name, const QString& type, int shortWindow, int longWindow) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO strategies (name, type, short_window, long_window) VALUES (?, ?, ?, ?)");
    query.addBindValue(name);
    query.addBindValue(type);
    query.addBindValue(shortWindow);
    query.addBindValue(longWindow);

    if (!query.exec()) {
        QMessageBox::warning(this, "Error",
            "Failed to add strategy: " + query.lastError().text());
        return false;
    }

    return true;
}

void StrategyManager::setup_ui() {
    setStyleSheet(
    "QWidget { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #1a1a2e, stop:1 #16213e); }"
);

    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(40, 40, 40, 40);

    QLabel* title = new QLabel("<h1 style='color: #eee;'>📚 Strategy Management</h1>");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("background: transparent; font-size: 24pt; font-weight: bold;");
    main_layout->addWidget(title);

    QLabel* subtitle = new QLabel("<p style='color: #aaa;'>View, Edit and Delete Saved Strategies</p>");
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("background: transparent; font-size: 14pt; margin-bottom: 20px;");
    main_layout->addWidget(subtitle);

    main_layout->addSpacing(20);

    strategy_list = new QListWidget();
    strategy_list->setStyleSheet(
        "QListWidget { background-color: #0f3460; color: #eee; border: 2px solid #4CAF50; "
        "border-radius: 8px; padding: 10px; font-size: 12pt; }"
        "QListWidget::item { padding: 10px; border-bottom: 1px solid #2196F3; }"
        "QListWidget::item:hover { background-color: #1a4d7a; }"
        "QListWidget::item:selected { background-color: #2196F3; }"
    );

    main_layout->addWidget(strategy_list);

    main_layout->addSpacing(20);

    QHBoxLayout* button_layout = new QHBoxLayout();

    back_button = new QPushButton("Back");
    back_button->setMinimumSize(150, 60);
    back_button->setStyleSheet(
        "QPushButton { background-color: #555; color: white; border: none; "
        "border-radius: 8px; font-size: 16pt; font-weight: bold; }"
        "QPushButton:hover { background-color: #666; }"
        "QPushButton:pressed { background-color: #444; }"
    );

    button_layout->addWidget(back_button);
    button_layout->addStretch();

    main_layout->addLayout(button_layout);

}

void StrategyManager::load_strategies() {
    strategy_list->clear();

    QSqlQuery query(db);
    query.exec("SELECT id, name, type, short_window, long_window FROM strategies ORDER BY name");

    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString type = query.value(2).toString();
        int short_win = query.value(3).toInt();
        int long_win = query.value(4).toInt();

        QWidget* item_widget = new QWidget();
        QHBoxLayout* item_layout = new QHBoxLayout(item_widget);
        item_layout->setContentsMargins(5, 5, 5, 5);

        QLabel* info_label = new QLabel(QString("%1 (%2): %3/%4").arg(name).arg(type).arg(short_win).arg(long_win));
        info_label->setStyleSheet("background: transparent; color: #eee; font-size: 13pt;");
        item_layout->addWidget(info_label);

        item_layout->addStretch();

        QPushButton* edit_btn = new QPushButton("Edit");
        edit_btn->setFixedSize(60, 20);
        edit_btn->setStyleSheet(
            "QPushButton { background-color: #0000c9; color: white; border: none; "
            "border-radius: 5px; font-size: 8pt; font-weight: bold; }"
            "QPushButton:hover { background-color: #0000f2; }"
            "QPushButton:pressed { background-color: #000087; }"
        );
        connect(edit_btn, &QPushButton::clicked, [this, id]() { on_edit_clicked(id); });
        item_layout->addWidget(edit_btn);

        QPushButton* delete_btn = new QPushButton("Delete");
        delete_btn->setFixedSize(60, 20);
        delete_btn->setStyleSheet(
            "QPushButton { background-color: #b80000; color: white; border: none; "
            "border-radius: 5px; font-size: 8pt; font-weight: bold; }"
            "QPushButton:hover { background-color: #d60000; }"
            "QPushButton:pressed { background-color: #600000; }"
        );
        item_layout->addWidget(delete_btn);

        QListWidgetItem* list_item = new QListWidgetItem(strategy_list);
        list_item->setSizeHint(QSize(item_widget->sizeHint().width(), 60));
        strategy_list->addItem(list_item);
        strategy_list->setItemWidget(list_item, item_widget);

        item_widget->setLayout(item_layout);
    }
}

void StrategyManager::on_edit_clicked(int id) {
    StrategyData strategy = get_strategy(id);
    if (strategy.id < 0) {
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Edit Strategy");
    dialog.setMinimumWidth(400);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QFormLayout* form_layout = new QFormLayout();

    QLineEdit* name_edit = new QLineEdit(strategy.name);
    form_layout->addRow("Strategy Name:", name_edit);

    QComboBox* type_combo = new QComboBox();

    //list of strategies
    type_combo->addItems({"Moving Average Crossover", "strategy 1", "strategy 2", "strategy 3"});
    int type_index = type_combo->findText(strategy.type);
    if (type_index != -1) {
        type_combo->setCurrentIndex(type_index);
    }
    form_layout->addRow("Strategy Type:", type_combo);

    QSpinBox* short_spin = new QSpinBox();
    short_spin->setRange(1, 1000);
    short_spin->setValue(strategy.shortWindow);
    form_layout->addRow("Short MA Window:", short_spin);

    QSpinBox* long_spin = new QSpinBox();
    long_spin->setRange(1, 1000);
    long_spin->setValue(strategy.longWindow);
    form_layout->addRow("Long MA Window:", long_spin);

    layout->addLayout(form_layout);

    QDialogButtonBox* button_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(button_box, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(button_box, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(button_box);

    if (dialog.exec() == QDialog::Accepted) {
        QString new_name = name_edit->text().trimmed();
        QString new_type = type_combo->currentText();
        int short_val = short_spin->value();
        int long_val = long_spin->value();

        if (new_name == strategy.name &&
            short_val == strategy.shortWindow &&
            new_type == strategy.type &&
            long_val == strategy.longWindow) {
            QMessageBox::information(this, "No Changes", "Nothing was changed.");
            return;
            }

        if (new_name.isEmpty()) {
            QMessageBox::warning(this, "Invalid Input", "Strategy name cannot be empty.");
            return;
        }

        if (long_val <= short_val) {
            QMessageBox::warning(this, "Invalid Parameters",
                "Long MA window must be greater than Short MA window.");
            return;
        }

        if (strategy_name_exists(new_name, id)) {
            QMessageBox::warning(this, "Duplicate Name",
                "A strategy with this name already exists. Please choose a different name.");
            return;
        }

        if (update_strategy(id, new_name, new_type, short_val, long_val)) {
            load_strategies();
            emit strategy_updated();
            QMessageBox::information(this, "Success", "Strategy updated successfully!");
        }
    }
}

StrategyData StrategyManager::get_strategy(int id) {
    StrategyData data;
    data.id = -1;

    QSqlQuery query(db);
    query.prepare("SELECT id, name, type, short_window, long_window FROM strategies WHERE id = ?");
    query.addBindValue(id);
    query.exec();

    if (query.next()) {
        data.id = query.value(0).toInt();
        data.name = query.value(1).toString();
        data.type = query.value(2).toString();
        data.shortWindow = query.value(3).toInt();
        data.longWindow = query.value(4).toInt();
    }

    return data;
}

bool StrategyManager::strategy_name_exists(const QString& name, int excludeId) {
    QSqlQuery query(db);
    if (excludeId >= 0) {
        query.prepare("SELECT COUNT(*) FROM strategies WHERE name = ? AND id != ?");
        query.addBindValue(name);
        query.addBindValue(excludeId);
    } else {
        query.prepare("SELECT COUNT(*) FROM strategies WHERE name = ?");
        query.addBindValue(name);
    }

    query.exec();
    if (query.next()) {
        return query.value(0).toInt() > 0;
    }

    return false;
}

bool StrategyManager::update_strategy(int id, const QString& name, const QString& type, int shortWindow, int longWindow) {
    QSqlQuery query(db);
    query.prepare("UPDATE strategies SET name = ?, type = ?, short_window = ?, long_window = ? WHERE id = ?");
    query.addBindValue(name);
    query.addBindValue(type);
    query.addBindValue(shortWindow);
    query.addBindValue(longWindow);
    query.addBindValue(id);

    if (!query.exec()) {
        QMessageBox::warning(this, "Error",
            "Failed to update strategy: " + query.lastError().text());
        return false;
    }

    return true;
}

void StrategyManager::on_list_item_double_clicked(QListWidgetItem* item) {
    int row = strategy_list->row(item);

    auto strategies = get_all_strategies();
    if (row >= 0 && row < strategies.size()) {
        on_edit_clicked(strategies[row].id);
    }
}

QVector<StrategyData> StrategyManager::get_all_strategies() {
    QVector<StrategyData> strategies;

    QSqlQuery query(db);
    query.exec("SELECT id, name, type, short_window, long_window FROM strategies ORDER BY name");

    while (query.next()) {
        StrategyData data;
        data.id = query.value(0).toInt();
        data.name = query.value(1).toString();
        data.type = query.value(2).toString();
        data.shortWindow = query.value(3).toInt();
        data.longWindow = query.value(4).toInt();
        strategies.append(data);
    }

    return strategies;
}
