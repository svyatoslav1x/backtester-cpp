#ifndef GUI_MAIN_WINDOW_DONE_SCREEN_H
#define GUI_MAIN_WINDOW_DONE_SCREEN_H

#include <QLabel>
#include <QPointer>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

class DoneScreen : public QWidget {
	Q_OBJECT;
public:
	explicit DoneScreen(QWidget* parent = nullptr);

	void setResults(const QString& final_stats);
private:
	void buildUi();

	QPointer<QLabel> text;
	QPointer<QTextEdit> final_result;
	QPointer<QVBoxLayout> main_layout;
	QPointer<QHBoxLayout> buttons;
	QPointer<QPushButton> return_starting_screen;
	QPointer<QPushButton> return_charts_screen;
	QPointer<QWidget> buttons_widget;
signals:
	void StartScreenSwitch();

	void BacktestScreenSwitch();
};
#endif // GUI_MAIN_WINDOW_DONE_SCREEN_H
