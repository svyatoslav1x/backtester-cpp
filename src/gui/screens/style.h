#ifndef STYLE_H
#define STYLE_H

#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QTextEdit>
#include <QPalette>
#include <QFont>
#include <QWidget>
#include <vector>

inline const std::vector<QColor> colors = {
    QColor(30, 64, 175),   // 0 deep blue
    QColor(245, 247, 250), // 1 cool white
    QColor(148, 163, 184), // 2 muted gray
    QColor(17, 24, 39),    // 3 near-black
    QColor(232, 240, 234), // 4 soft mist
    QColor(220, 230, 223), // 5 pale
    QColor(250, 255, 255), // 6 icy white
    QColor(120, 138, 126), // 7 gray
    QColor(71, 85, 105),   // 8 deep blue
    QColor(245, 247, 250), // 9 light gray-white
    QColor(37, 99, 235),   // 10 vivid blue
    QColor(30, 64, 175),   // 11 strong blue
    QColor(240, 244, 248), // 12 pale
    QColor(236, 241, 246)  // 13 soft blue-gray
};

inline QColor transparentBlack(int transparent) {
    return QColor(0, 0, 0, transparent);
}

inline void setWidgetFont(QWidget *widget, int pointSize, bool bold) {
    QFont f = widget->font();
    f.setPointSize(pointSize);
    f.setBold(bold);
    widget->setFont(f);
}

inline void setLabelStyle(QLabel *label, const QColor &color, int pointSize, bool bold) {
    setWidgetFont(label, pointSize, bold);

    QPalette p = label->palette();
    p.setColor(QPalette::WindowText, color);
    label->setPalette(p);
    label->setAutoFillBackground(false);
}

inline void setButtonStyle(QPushButton *button, const QColor &backgroundColor, const QColor &textColor, int pointSize,
                           bool bold) {
    setWidgetFont(button, pointSize, bold);

    QPalette p = button->palette();
    p.setColor(QPalette::Button, backgroundColor);
    p.setColor(QPalette::ButtonText, textColor);

    button->setPalette(p);
    button->setAutoFillBackground(true);
}

inline void setSecondaryButtonStyle(QPushButton *button, int pointSize, bool bold) {
    setButtonStyle(button, colors[8], colors[9], pointSize, bold);
}

inline void setEditStyle(QWidget *widget, const QColor &baseColor, const QColor &textColor,
                         const QColor &placeholderColor, int pointSize, bool bold) {
    setWidgetFont(widget, pointSize, bold);

    QPalette p = widget->palette();
    p.setColor(QPalette::Base, baseColor);
    p.setColor(QPalette::Text, textColor);
    p.setColor(QPalette::PlaceholderText, placeholderColor);
    p.setColor(QPalette::Button, baseColor);
    p.setColor(QPalette::ButtonText, textColor);

    widget->setPalette(p);
    widget->setAutoFillBackground(true);

    if (auto *spin = qobject_cast<QSpinBox *>(widget)) {
        spin->setFrame(true);
        QPalette sp = spin->palette();
        sp.setColor(QPalette::Base, colors[12]);
        sp.setColor(QPalette::Button, colors[13]);
        sp.setColor(QPalette::Text, textColor);
        sp.setColor(QPalette::ButtonText, textColor);
        spin->setPalette(sp);
        spin->setAutoFillBackground(true);
    }
}

inline void setComboStyle(QComboBox *combo, const QColor &baseColor, const QColor &textColor, int pointSize,
                          bool bold) {
    setWidgetFont(combo, pointSize, bold);

    QPalette p = combo->palette();
    p.setColor(QPalette::Base, baseColor);
    p.setColor(QPalette::Button, baseColor);
    p.setColor(QPalette::Text, textColor);
    p.setColor(QPalette::ButtonText, textColor);
    p.setColor(QPalette::Highlight, colors[11]);
    p.setColor(QPalette::HighlightedText, colors[1]);

    combo->setPalette(p);
    combo->setAutoFillBackground(true);
}

inline void setTextDisplayStyle(QTextEdit *textEdit, const QColor &baseColor, const QColor &textColor, int pointSize,
                                bool bold) {
    setWidgetFont(textEdit, pointSize, bold);

    QPalette p = textEdit->palette();
    p.setColor(QPalette::Base, baseColor);
    p.setColor(QPalette::Text, textColor);
    p.setColor(QPalette::PlaceholderText, colors[2]);
    p.setColor(QPalette::Highlight, colors[11]);
    p.setColor(QPalette::HighlightedText, colors[1]);

    textEdit->setPalette(p);
    textEdit->setAutoFillBackground(true);
}

inline void setFilledLabelStyle(QLabel *label, const QColor &textColor, const QColor &bgColor, int pointSize,
                                bool bold) {
    setLabelStyle(label, textColor, pointSize, bold);

    QPalette p = label->palette();
    p.setColor(QPalette::Window, bgColor);
    p.setColor(QPalette::WindowText, textColor);
    label->setPalette(p);
    label->setAutoFillBackground(true);
}

#endif // STYLE_H
