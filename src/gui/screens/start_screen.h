#ifndef GUI_MAIN_WINDOW_START_SCREEN_H
#define GUI_MAIN_WINDOW_START_SCREEN_H

#include <QComboBox>
#include <QPointer>

class StartScreen : public QWidget {
    Q_OBJECT

public:
    explicit StartScreen(QWidget *parent = nullptr);

    void chooseDatasets(const QStringList &names);

    QString chosenDataset() const;

private:
    void buildUi();

    QPointer<QComboBox> dataset_combo;
};


#endif //GUI_MAIN_WINDOW_START_SCREEN_H
