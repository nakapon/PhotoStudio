#include <PlatformQt.h>

#include <QtWidgets>

#include <ImageReader.h>
#include <ImageWriter.h>

#include <ImageProc.h>

#include "ImageLibraryQt.h"

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, viewImage(nullptr)
{
	qApp->installEventFilter(this);

	// initialize and show main window
	ui->setupUi(this);

	// create main menu
	this->createFileMenu();
	this->createImageMenu();

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
		this->fileToShow();
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

void MainWindow::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	QSize viewSize = this->size();

	// background
	{
		QColor backColor(0, 0, 0);

		painter.fillRect(0, 0, viewSize.width(), viewSize.height(), backColor);
	}

	// image
	if(this->viewImage != nullptr)
	{
		QRect srcRect;
		QRect dstRect;

		Int32 srcWidth, srcHeight;
		Int32 dstWidth, dstHeight;

		float scaleH, scaleV, scale;

		srcWidth = this->viewImage->size().width();
		srcHeight = this->viewImage->size().height();

		srcRect.setRect(0, 0, srcWidth, srcHeight);

		scaleH = (float)viewSize.width() / srcWidth;
		scaleV = (float)viewSize.height() / srcHeight;
		scale = std::min(scaleH, scaleV);

		dstWidth = (UInt32)(scale * srcWidth + 0.5f);
		dstHeight = (UInt32)(scale * srcHeight + 0.5f);

		dstRect.setRect((viewSize.width() - dstWidth) / 2,
						(viewSize.height() - dstHeight) / 2,
						dstWidth, dstHeight);

		painter.drawImage(dstRect, *this->viewImage, srcRect);
	}
}

void MainWindow::updateViewImage(const IImageData* image)
{
	PF_SAFE_DELETE(this->viewImage);

	if(image != nullptr)
	{
		this->viewImage = ImageLibraryQt::ImageToQImage(image);
	}

	this->update();
}

void MainWindow::createFileMenu()
{
	connect(ui->menuFile, SIGNAL(aboutToShow()), this, SLOT(fileToShow()));

	connect(ui->actFileNew, SIGNAL(triggered()), this, SLOT(fileNew()));
	connect(ui->actFileOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
	connect(ui->actFileClose, SIGNAL(triggered()), this, SLOT(fileClose()));
	connect(ui->actFileSave, SIGNAL(triggered()), this, SLOT(fileSave()));

}

void MainWindow::fileToShow()
{
	ui->actFileClose->setEnabled(this->imageData.IsCreated());
	ui->actFileSave->setEnabled(this->imageData.IsCreated());
}

void MainWindow::fileNew()
{
	// TODO
}

void MainWindow::fileOpen()
{
	QStringList filters;

	filters.append("All Files (*.*)");

	const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
	QFileDialog dialog(this, tr("Open File"), picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
	dialog.setAcceptMode(QFileDialog::AcceptOpen);
	dialog.setFileMode(QFileDialog::ExistingFiles);
	dialog.setNameFilters(filters);
	dialog.selectNameFilter(filters[filters.size() - 1]);

	if(dialog.exec() == QDialog::Accepted)
	{
		QString file = dialog.selectedFiles()[0];
		TCHAR filePath[MAX_PATH] = { 0 };

		this->procImage.Destroy();

		PFQT_COPY_QSTR_TO_TSTR(filePath, sizeof(filePath) / sizeof(filePath[0]), file);
		if(ImageReader::ReadImage(filePath, &this->imageData))
		{
			this->updateViewImage(&this->imageData);
		}
	}
}

void MainWindow::fileClose()
{
	this->imageData.Destroy();
	this->procImage.Destroy();

	this->updateViewImage(nullptr);
}

void MainWindow::fileSave()
{
	QStringList filters;

	filters.append("All Files (*.*)");

	const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
	QFileDialog dialog(this, tr("Save File"), picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setNameFilters(filters);
//    dialog.selectNameFilter(filters[filters.size() - 1]);

	if(dialog.exec() == QDialog::Accepted)
	{
		IImageData* image = this->procImage.IsCreated() ? &this->procImage : &this->imageData;

		QString file = dialog.selectedFiles()[0];
		TCHAR filePath[MAX_PATH] = { 0 };

		PFQT_COPY_QSTR_TO_TSTR(filePath, sizeof(filePath) / sizeof(filePath[0]), file);
		ImageWriter::WriteImage(filePath, image);
	}
}

void MainWindow::createImageMenu()
{
	connect(ui->menuImage, SIGNAL(aboutToShow()), this, SLOT(imageToShow()));

	connect(ui->actImageGray, SIGNAL(triggered()), this, SLOT(imageGray()));

}

void MainWindow::imageToShow()
{
	ui->actImageGray->setEnabled(this->imageData.IsCreated());
}

void MainWindow::imageGray()
{
	ImageProc::GrayScale(&this->procImage, &this->imageData);

	this->updateViewImage(&this->procImage);
}
