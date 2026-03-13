#ifndef STYLE_H
#define STYLE_H

#include <QColor>
#include <QComboBox>
#include <QFont>
#include <QLabel>
#include <QLineEdit>
#include <qlistview.h>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QWidget>
#include <vector>

inline const std::vector<QColor> colors = {
    QColor(30, 64, 175), // 0 deep blue
    QColor(245, 247, 250), // 1 cool white
    QColor(148, 163, 184), // 2 muted gray
    QColor(17, 24, 39), // 3 near-black
    QColor(232, 240, 234), // 4 soft mist
    QColor(220, 230, 223), // 5 pale
    QColor(250, 255, 255), // 6 icy white
    QColor(120, 138, 126), // 7 gray
    QColor(71, 85, 105), // 8 blue-gray
    QColor(245, 247, 250), // 9 light gray-white
    QColor(37, 99, 235), // 10 vivid blue
    QColor(30, 64, 175), // 11 strong blue
    QColor(240, 244, 248), // 12 pale
    QColor(236, 241, 246) // 13 soft blue-gray
};

inline QColor transparentBlack(int transparent) {
    return QColor(0, 0, 0, transparent);
}

inline QString cssColor(const QColor &color) {
    return color.name(QColor::HexArgb);
}

inline void setWidgetFont(QWidget *widget, int pointSize, bool bold) {
    QFont f = widget->font();
    f.setPointSize(pointSize);
    f.setBold(bold);
    widget->setFont(f);
}

inline void setLabelStyle(QLabel *label, const QColor &color, int pointSize, bool bold) {
    setWidgetFont(label, pointSize, bold);
    label->setStyleSheet(QString("QLabel { color: %1; background: transparent; }").arg(cssColor(color)));
}

inline void setButtonStyle(QPushButton *button, const QColor &backgroundColor, const QColor &textColor, int pointSize,
                           bool bold) {
    setWidgetFont(button, pointSize, bold);

    const QColor borderColor = backgroundColor.darker(115);
    const QColor hoverColor = backgroundColor.lighter(108);
    const QColor pressedColor = backgroundColor.darker(110);
    const QColor disabledBackground = backgroundColor.lighter(125);
    const QColor disabledText = textColor.alpha() > 0 ? textColor.darker(130) : colors[7];

    button->setStyleSheet(QString(
            "QPushButton {"
            " background-color: %1;"
            " color: %2;"
            " border: 1px solid %3;"
            " border-radius: 4px;"
            " padding: 8px 14px;"
            "}"
            "QPushButton:hover {"
            " background-color: %4;"
            "}"
            "QPushButton:pressed {"
            " background-color: %5;"
            "}"
            "QPushButton:disabled {"
            " background-color: %6;"
            " color: %7;"
            " border: 1px solid %3;"
            "}")
        .arg(cssColor(backgroundColor), cssColor(textColor), cssColor(borderColor), cssColor(hoverColor),
             cssColor(pressedColor), cssColor(disabledBackground), cssColor(disabledText)));
}

inline void setSecondaryButtonStyle(QPushButton *button, int pointSize, bool bold) {
    setButtonStyle(button, colors[8], colors[9], pointSize, bold);
}

inline void setEditStyle(QWidget *widget, const QColor &baseColor, const QColor &textColor,
                         const QColor &placeholderColor, int pointSize, bool bold) {
    setWidgetFont(widget, pointSize, bold);

    const QColor borderColor = baseColor.darker(110);
    const QString commonStyle = QString(
        "background-color: %1;"
        "color: %2;"
        "border: 1px solid %3;"
        "border-radius: 4px;"
        "padding: 6px 8px;").arg(cssColor(baseColor), cssColor(textColor), cssColor(borderColor));

    widget->setStyleSheet(QString("QWidget { %1 }").arg(commonStyle));

    if (auto *spin = qobject_cast<QSpinBox *>(widget)) {
        spin->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        spin->setStyleSheet(QString(
            "QSpinBox { %1 }"
            "QSpinBox::up-button, QSpinBox::down-button {"
            " background-color: %2;"
            " width: 18px;"
            " border-left: 1px solid %3;"
            "}"
            "QSpinBox::up-button:hover, QSpinBox::down-button:hover {"
            " background-color: %4;"
            "}").arg(commonStyle, cssColor(colors[13]), cssColor(borderColor), cssColor(colors[12])));
        return;
    }

    if (auto *lineEdit = qobject_cast<QLineEdit *>(widget)) {
        lineEdit->setPlaceholderText(lineEdit->placeholderText());
        lineEdit->setStyleSheet(QString(
            "QLineEdit { %1 }"
            "QLineEdit[echoMode='2'] { letter-spacing: 1px; }").arg(commonStyle));
        lineEdit->setProperty("placeholderTextColor", placeholderColor);
    }
}

inline void setComboStyle(QComboBox *combo,
                          const QColor &backgroundColor,
                          const QColor &textColor,
                          int pointSize,
                          bool bold) {
    setWidgetFont(combo, pointSize, bold);

    combo->setStyleSheet(QString(
        "QComboBox {"
        " background-color: %1;"
        " color: %2;"
        " border: 1px solid %2;"
        " padding: 4px 8px;"
        " min-height: 28px;"
        "}"
        "QComboBox QAbstractItemView {"
        " background-color: %1;"
        " color: %2;"
        " border: 1px solid %2;"
        "}"
    ).arg(backgroundColor.name(), textColor.name()));
}

inline void setTextDisplayStyle(QTextEdit *textEdit, const QColor &baseColor, const QColor &textColor, int pointSize,
                                bool bold) {
    setWidgetFont(textEdit, pointSize, bold);

    const QColor borderColor = colors[10];
    textEdit->setStyleSheet(QString(
            "QTextEdit {"
            " background-color: %1;"
            " color: %2;"
            " border: 1px solid %3;"
            " border-radius: 4px;"
            " padding: 8px;"
            "}")
        .arg(cssColor(baseColor), cssColor(textColor), cssColor(borderColor), cssColor(colors[11]),
             cssColor(colors[1])));
}

inline void setFilledLabelStyle(QLabel *label, const QColor &textColor, const QColor &bgColor, int pointSize,
                                bool bold) {
    setWidgetFont(label, pointSize, bold);
    label->setStyleSheet(QString(
            "QLabel {"
            " color: %1;"
            " background-color: %2;"
            " padding: 4px 6px;"
            "}")
        .arg(cssColor(textColor), cssColor(bgColor)));
}

#endif // STYLE_H
