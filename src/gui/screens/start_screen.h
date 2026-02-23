#ifndef GUI_MAIN_WINDOW_START_SCREEN_H
#define GUI_MAIN_WINDOW_START_SCREEN_H

#include <QComboBox>
#include <QPointer>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>

class StartScreen : public QWidget {
    Q_OBJECT

public:
    explicit StartScreen(QWidget *parent = nullptr);

    void chooseDatasets(const QStringList &names);

    QString chosenDataset() const;

private:
    void buildUi();

    void setNewsHtml(const QString &html);

    void setNewsText(const QString &text);

    void setDatasets(const QStringList &names);

    QString selectedDataset() const;

    QPointer<QVBoxLayout> main_layout;
    QPointer<QLabel> title_label;
    QPointer<QLabel> news_header;
    QPointer<QLabel> dataset_text;
    QPointer<QTextEdit> news_display;
    QPointer<QComboBox> dataset_combo;
    QPointer<QHBoxLayout> button_layout;
    QPointer<QPushButton> manage_strategies_button;
    QPointer<QPushButton> create_strategy_button;
    QPointer<QPushButton> start_button;
};


#endif //GUI_MAIN_WINDOW_START_SCREEN_H
