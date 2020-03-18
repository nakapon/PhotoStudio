#include <Platform.h>

#include "MainWindow.h"

#include <QApplication>

INT main(INT argc, CHAR *argv[])
{
	QApplication app(argc, argv);

	// resource
//	Q_INIT_RESOURCE(PhotoStudioQt);

	// main window
	MainWindow window;
	window.show();

	return app.exec();
}
