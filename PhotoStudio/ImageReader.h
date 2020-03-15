#pragma once

#include "ImageData.h"

class ImageReader
{
public:

	// TODO Support 1CH
	static bool ReadImage(LPCTSTR pszFilePath, CImageData& ImageData);

private:
	ImageReader() = delete;
	~ImageReader() = delete;
	ImageReader(const ImageReader&) = delete;
	ImageReader& operator=(const ImageReader&) = delete;
};
