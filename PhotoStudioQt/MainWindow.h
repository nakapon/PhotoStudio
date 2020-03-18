#pragma once

// #include <PlatformMinimal.h>
#include <Platform.h>

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;

protected:
	void closeEvent(QCloseEvent* event);

	bool eventFilter(QObject *widget, QEvent *event);

	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);

	void moveEvent(QMoveEvent * event);
	void resizeEvent(QResizeEvent *event);

protected slots:

	// File
/*
	void createFileMenu();
	void fileToShow();
	void fileToShow(int event, void* param);

	void fileNew();
	void fileOpen();
	void fileClose();
	void fileCloseAll();
	void fileSave();
	void fileSaveAs();

	void fileExit();
*/
};
