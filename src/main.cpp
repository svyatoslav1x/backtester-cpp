#include "gui/MainWindow.h"
#include <QApplication>

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

	QApplication::setApplicationName("C++ Event-Driven Backtester");
	QApplication::setApplicationVersion("1.0");

	MainWindow window;

	window.show();

	return app.exec();
}
