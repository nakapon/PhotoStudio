#pragma once

#include <IImageData.h>

class ImageReader
{
public:
	// TODO Support 1CH
	static bool ReadImage(LPCTSTR pszFilePath, IImageData* pImageData);

private:
	ImageReader() = delete;
	~ImageReader() = delete;
	ImageReader(const ImageReader&) = delete;
	ImageReader& operator=(const ImageReader&) = delete;
};
