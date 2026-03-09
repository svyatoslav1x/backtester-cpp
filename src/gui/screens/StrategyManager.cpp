#include "StrategyManager.h"
#include "../../../data/database/StrategyDatabase.h"

#include <QDir>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QLabel>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QDialog>
#include <QDialogButtonBox>

StrategyManager::StrategyManager(QWidget *parent)
    : QWidget(parent) {
    setup_ui();
}

StrategyManager::~StrategyManager() {
}

void StrategyManager::setup_ui() {
    main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(40, 40, 40, 40);

    title = new QLabel("<h1 style='color: #111827;'>📚 Strategy Management</h1>");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("background: transparent; font-size: 24pt; font-weight: bold;");
    main_layout->addWidget(title);

    subtitle = new QLabel("<p style='color: #94A3B8;'>View, Edit and Delete Saved Strategies</p>");
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("background: transparent; font-size: 14pt; margin-bottom: 20px;");
    main_layout->addWidget(subtitle);

    main_layout->addSpacing(20);

    strategy_list = new QListWidget();
    strategy_list->setFocusPolicy(Qt::NoFocus);
    strategy_list->setSelectionMode(QAbstractItemView::NoSelection);
    strategy_list->setStyleSheet(
        "QListWidget { background-color: #FAFFFF; color: #111827; border: 2px solid #CBD5E1; "
        "border-radius: 8px; padding: 10px; font-size: 12pt; }"
        "QListWidget::item { background-color: #FFF8FA; padding: 10px; border-bottom: 1px solid #E2E8F0; }"
        "QListWidget::item:hover { background-color: #F8FAFC; }"
        "QListWidget::item:selected { background-color: #DBEAFE; }"
    );
    connect(strategy_list, &QListWidget::itemDoubleClicked, this, &StrategyManager::on_list_item_double_clicked);

    main_layout->addWidget(strategy_list);
    main_layout->addSpacing(20);

    button_layout = new QHBoxLayout();

    back_button = new QPushButton("Back");
    back_button->setMinimumSize(150, 60);
    back_button->setStyleSheet(
        "QPushButton { background-color: #475569; color: #F5F7FA; border: none; "
        "border-radius: 8px; font-size: 16pt; font-weight: bold; }"
        "QPushButton:hover { background-color: #334155; }"
        "QPushButton:pressed { background-color: #1F2937; }"
    );
    connect(back_button, &QPushButton::clicked, this, [this] {
        emit startScreenSwitch();
    });

    button_layout->addStretch();
    button_layout->addWidget(back_button);
    button_layout->addStretch();

    main_layout->addLayout(button_layout);
}

void StrategyManager::load_strategies() {
    strategy_list->clear();

    QVector<StrategyData> strategies = get_all_strategies();

    for (const StrategyData &strategy: strategies) {
        item_widget = new QWidget();

        item_layout = new QHBoxLayout(item_widget);
        item_layout->setContentsMargins(5, 5, 5, 5);

        QString paramsStr;
        for (auto it = strategy.parameters.constBegin(); it != strategy.parameters.constEnd(); ++it) {
            paramsStr += QString("%1: %2   ").arg(it.key(), it.value());
        }

        info_label = new QLabel(QString("%1 (%2)\n%3").arg(strategy.name).arg(strategy.model_type).arg(paramsStr));
        info_label->setStyleSheet("background: transparent; color: #111827; font-size: 11pt;");
        item_layout->addWidget(info_label);

        item_layout->addStretch();

        int id = strategy.id;

        edit_btn = new QPushButton("Edit");
        edit_btn->setFixedSize(60, 20);
        edit_btn->setStyleSheet(
            "QPushButton { background-color: #2563EB; color: white; border: none; "
            "border-radius: 5px; font-size: 8pt; font-weight: bold; }"
            "QPushButton:hover { background-color: #1D4ED8; }"
            "QPushButton:pressed { background-color: #1E40AF; }"
        );
        edit_btn->setEnabled(strategy.is_editable);
        connect(edit_btn, &QPushButton::clicked, [this, id]() { on_edit_clicked(id); });
        item_layout->addWidget(edit_btn);

        delete_btn = new QPushButton("Delete");
        delete_btn->setFixedSize(60, 20);
        delete_btn->setStyleSheet(
            "QPushButton { background-color: #DC2626; color: white; border: none; "
            "border-radius: 5px; font-size: 8pt; font-weight: bold; }"
            "QPushButton:hover { background-color: #B91C1C; }"
            "QPushButton:pressed { background-color: #991B1B; }"
        );
        connect(delete_btn, &QPushButton::clicked, [this, id]() { on_delete_clicked(id); });
        item_layout->addWidget(delete_btn);

        QListWidgetItem *list_item = new QListWidgetItem(strategy_list);
        list_item->setSizeHint(QSize(item_widget->sizeHint().width(), 70));
        strategy_list->addItem(list_item);
        strategy_list->setItemWidget(list_item, item_widget);
        list_item->setData(Qt::UserRole, id);
    }
}

QVector<StrategyData> StrategyManager::get_all_strategies() {
    QVector<StrategyData> strategies;
    QSqlDatabase db = StrategyDatabase::database();

    QSqlQuery query(db);
    query.exec("SELECT id, name, model_type, is_editable FROM strategies ORDER BY name");

    while (query.next()) {
        StrategyData data;
        data.id = query.value(0).toInt();
        data.name = query.value(1).toString();
        data.model_type = query.value(2).toString();
        data.is_editable = query.value(3).toInt();

        QSqlQuery pQuery(db);
        pQuery.prepare("SELECT param_key, param_value FROM strategy_parameters WHERE strategy_id = ?");
        pQuery.addBindValue(data.id);
        pQuery.exec();

        while (pQuery.next()) {
            data.parameters[pQuery.value(0).toString()] = pQuery.value(1).toString();
        }
        strategies.append(data);
    }

    return strategies;
}

StrategyData StrategyManager::get_strategy(int id) {
    StrategyData data;
    data.id = -1;
    QSqlDatabase db = StrategyDatabase::database();

    QSqlQuery query(db);
    query.prepare("SELECT id, name, model_type, is_editable FROM strategies WHERE id = ?");
    query.addBindValue(id);
    query.exec();

    if (query.next()) {
        data.id = query.value(0).toInt();
        data.name = query.value(1).toString();
        data.model_type = query.value(2).toString();
        data.is_editable = query.value(3).toInt();

        QSqlQuery pQuery(db);
        pQuery.prepare("SELECT param_key, param_value FROM strategy_parameters WHERE strategy_id = ?");
        pQuery.addBindValue(id);
        pQuery.exec();

        while (pQuery.next()) {
            data.parameters[pQuery.value(0).toString()] = pQuery.value(1).toString();
        }
    }

    return data;
}

void StrategyManager::on_edit_clicked(int id) {
    StrategyData strategy = get_strategy(id);
    if (strategy.id < 0) return;

    if (!strategy.is_editable) {
        QMessageBox::warning(this, "Error", "This strategy is not editable.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Edit Strategy");
    dialog.setMinimumWidth(400);

    layout = new QVBoxLayout(&dialog);
    form_layout = new QFormLayout();

    name_edit = new QLineEdit(strategy.name);
    form_layout->addRow("Strategy Name:", name_edit);

    type_label = new QLabel(strategy.model_type);
    form_layout->addRow("Model Type:", type_label);

    QMap<QString, QSpinBox *> int_spins;
    QMap<QString, QDoubleSpinBox *> double_spins;

    for (auto it = strategy.parameters.constBegin(); it != strategy.parameters.constEnd(); ++it) {
        QString key = it.key();
        QString value = it.value();

        if (key == "short_window" || key == "long_window") {
            QSpinBox *spin = new QSpinBox();
            spin->setRange(1, 10000);
            spin->setValue(value.toInt());
            form_layout->addRow(key + ":", spin);
            int_spins[key] = spin;
        } else if (key == "stop_loss_percentage") {
            QDoubleSpinBox *dspin = new QDoubleSpinBox();
            dspin->setRange(0.0, 100.0);
            dspin->setSingleStep(0.01);
            dspin->setValue(value.toDouble());
            form_layout->addRow(key + ":", dspin);
            double_spins[key] = dspin;
        } else {
            QSpinBox *spin = new QSpinBox();
            spin->setRange(0, 10000);
            spin->setValue(value.toInt());
            form_layout->addRow(key + ":", spin);
            int_spins[key] = spin;
        }
    }

    layout->addLayout(form_layout);

    button_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(button_box, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(button_box, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(button_box);

    if (dialog.exec() == QDialog::Accepted) {
        QString new_name = name_edit->text().trimmed();

        if (new_name.isEmpty()) {
            QMessageBox::warning(this, "Invalid Input", "Strategy name cannot be empty.");
            return;
        }

        if (strategy_name_exists(new_name, id)) {
            QMessageBox::warning(this, "Duplicate Name", "A strategy with this name already exists.");
            return;
        }

        if (int_spins.contains("short_window") && int_spins.contains("long_window")) {
            if (int_spins["long_window"]->value() <= int_spins["short_window"]->value()) {
                QMessageBox::warning(this, "Invalid Parameters",
                                     "Long MA window must be greater than Short MA window.");
                return;
            }
        }

        QMap<QString, QString> new_params;
        for (auto it = int_spins.constBegin(); it != int_spins.constEnd(); ++it) {
            new_params[it.key()] = QString::number(it.value()->value());
        }
        for (auto it = double_spins.constBegin(); it != double_spins.constEnd(); ++it) {
            new_params[it.key()] = QString::number(it.value()->value(), 'f', 2);
        }

        if (update_strategy(id, new_name, new_params)) {
            load_strategies();
            emit strategy_updated();
            QMessageBox::information(this, "Success", "Strategy updated successfully!");
        }
    }
}

bool StrategyManager::update_strategy(int id, const QString &name, const QMap<QString, QString> &params) {
    QSqlDatabase db = StrategyDatabase::database();

    if (!db.transaction()) return false;

    QSqlQuery query(db);
    query.prepare("UPDATE strategies SET name = ? WHERE id = ?");
    query.addBindValue(name);
    query.addBindValue(id);

    if (!query.exec()) {
        db.rollback();
        QMessageBox::warning(this, "Error", "Failed to update strategy: " + query.lastError().text());
        return false;
    }

    for (auto it = params.constBegin(); it != params.constEnd(); ++it) {
        QSqlQuery pQuery(db);
        pQuery.prepare("UPDATE strategy_parameters SET param_value = ? WHERE strategy_id = ? AND param_key = ?");
        pQuery.addBindValue(it.value());
        pQuery.addBindValue(id);
        pQuery.addBindValue(it.key());

        if (!pQuery.exec()) {
            db.rollback();
            QMessageBox::warning(this, "Error", "Failed to update parameters: " + pQuery.lastError().text());
            return false;
        }
    }

    if (!db.commit()) {
        db.rollback();
        return false;
    }

    return true;
}

bool StrategyManager::strategy_name_exists(const QString &name, int excludeId) {
    QSqlDatabase db = StrategyDatabase::database();
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

void StrategyManager::on_list_item_double_clicked(QListWidgetItem *item) {
    if (item) {
        int id = item->data(Qt::UserRole).toInt();
        on_edit_clicked(id);
    }
}

void StrategyManager::on_delete_clicked(int id) {
    StrategyData strategy = get_strategy(id);
    if (strategy.id < 0) return;

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Delete",
                                                              QString("Are you sure you want to delete strategy '%1'?").
                                                              arg(strategy.name),
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (remove_strategy(id)) {
            load_strategies();
            emit strategy_updated();
        }
    }
}

bool StrategyManager::remove_strategy(int id) {
    QSqlDatabase db = StrategyDatabase::database();
    if (!db.transaction()) return false;

    QSqlQuery pQuery(db);
    pQuery.prepare("DELETE FROM strategy_parameters WHERE strategy_id = ?");
    pQuery.addBindValue(id);
    if (!pQuery.exec()) {
        db.rollback();
        return false;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM strategies WHERE id = ?");
    query.addBindValue(id);
    if (!query.exec()) {
        db.rollback();
        QMessageBox::warning(this, "Error", "Failed to delete strategy: " + query.lastError().text());
        return false;
    }

    db.commit();
    return true;
}

bool StrategyManager::add_strategy(const QString &name, const QString &model_type,
                                   const QMap<QString, QString> &params) {
    QSqlDatabase db = StrategyDatabase::database();
    if (!db.transaction()) return false;

    QSqlQuery query(db);
    query.prepare("INSERT INTO strategies (name, model_type, is_editable) VALUES (?, ?, 1)");
    query.addBindValue(name);
    query.addBindValue(model_type);

    if (!query.exec()) {
        db.rollback();
        QMessageBox::warning(this, "Error", "Failed to add strategy: " + query.lastError().text());
        return false;
    }

    int strategyId = query.lastInsertId().toInt();

    for (auto it = params.constBegin(); it != params.constEnd(); ++it) {
        QSqlQuery pQuery(db);
        QString valType = (it.key() == "stop_loss_percentage") ? "double" : "int";

        pQuery.prepare(
            "INSERT INTO strategy_parameters (strategy_id, param_key, param_value, value_type, is_editable) VALUES (?, ?, ?, ?, 1)");
        pQuery.addBindValue(strategyId);
        pQuery.addBindValue(it.key());
        pQuery.addBindValue(it.value());
        pQuery.addBindValue(valType);

        if (!pQuery.exec()) {
            db.rollback();
            return false;
        }
    }

    db.commit();
    return true;
}
