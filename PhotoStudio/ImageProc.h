#pragma once

#include "ImageData.h"

class ImageProc
{
public:
	static void GrayScale(CImageData& ProcImage, const CImageData& ImageData);

private:
	ImageProc() = delete;
	~ImageProc() = delete;
	ImageProc(const ImageProc&) = delete;
	ImageProc& operator=(const ImageProc&) = delete;
};

