#ifndef STYLE_H
#define STYLE_H

#include <QLabel>
#include <QPushButton>


inline void setLabelStyle(QLabel *label, int required_size, bool bold, const QColor &color) {
    QFont f = label->font();
    f.setPointSize(required_size);
    f.setBold(bold);
    label->setFont(f);

    QPalette p = label->palette();
    p.setColor(QPalette::WindowText, color);
    label->setPalette(p);
    label->setAutoFillBackground(false);
}

inline void setButtonStyle(QPushButton *button, const QColor &backgroundColor, const QColor &textColor) {
    QFont f = button->font();
    f.setPointSize(13);
    f.setBold(true);
    button->setFont(f);


    QPalette p = button->palette();
    p.setColor(QPalette::Button, backgroundColor);
    p.setColor(QPalette::ButtonText, textColor);
    button->setPalette(p);
    button->setAutoFillBackground(true);
}
#endif // STYLE_H
