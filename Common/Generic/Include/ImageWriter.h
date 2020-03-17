#pragma once

#include "IImageData.h"

class ImageWriter
{
public:
	// TODO Support 1CH
	static bool WriteImage(LPCTSTR pszFilePath, const IImageData* pImageData);

private:
	ImageWriter() = delete;
	~ImageWriter() = delete;
	ImageWriter(const ImageWriter&) = delete;
	ImageWriter& operator=(const ImageWriter&) = delete;
};
