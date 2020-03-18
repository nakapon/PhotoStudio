//#include <PlatformQt.h>
#include <Platform.h>

#include <QtWidgets>

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	qApp->installEventFilter(this);

	// initialize and show main window
	ui->setupUi(this);

	// create main menu
//	this->createFileMenu();

//	this->showMaximized();
}

MainWindow::~MainWindow()
{

	delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
}

bool MainWindow::eventFilter(QObject *widget, QEvent *event)
{
	if(event->type() == QEvent::KeyPress)
	{
		// キーボードショートカットが使用できるように
		// aboutToShow に接続しているスロットを呼び出す
//		this->fileToShow();
	}

	return false;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
	event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
//	QMessageBox msgBox(this);

	//xxx
}

void MainWindow::moveEvent(QMoveEvent * event)
{
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
}
