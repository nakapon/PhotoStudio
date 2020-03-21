#pragma once

#include <QImage>

#include <IImageData.h>

class ImageLibraryQt
{
public:
	static QImage* ImageToQImage(const IImageData* pImage);

private:
	ImageLibraryQt() = delete;
	~ImageLibraryQt() = delete;
	ImageLibraryQt(const ImageLibraryQt&) = delete;
	ImageLibraryQt& operator=(const ImageLibraryQt&) = delete;
};
