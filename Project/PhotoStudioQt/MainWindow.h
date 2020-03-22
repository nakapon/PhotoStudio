#pragma once

#include <Platform.h>

#include <ImageData.h>

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

	QImage* viewImage;

	CImageData imageData;
	CImageData procImage;

protected:
	void closeEvent(QCloseEvent* event);

	bool eventFilter(QObject *widget, QEvent *event);

	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);

	void moveEvent(QMoveEvent * event);
	void resizeEvent(QResizeEvent *event);

	void paintEvent(QPaintEvent *event);

protected slots:

	void loadImage(LPCTSTR filePath);

	void updateViewImage(const IImageData* image);

	// File
	void createFileMenu();
	void fileToShow();

	void fileNew();
	void fileOpen();
	void fileClose();
	void fileSave();

	// Image
	void createImageMenu();
	void imageToShow();

	void imageGray();
};
