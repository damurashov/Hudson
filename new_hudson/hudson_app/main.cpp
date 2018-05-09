#include <QApplication>
#include "hudson_app.h"

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	Hudson_App app;

	app.show();
	return a.exec();
}
