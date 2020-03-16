#pragma once

#include "IImageData.h"

class ImageProc
{
public:
	static void GrayScale(IImageData* pDstImage, const IImageData* pSrcImage);

private:
	ImageProc() = delete;
	~ImageProc() = delete;
	ImageProc(const ImageProc&) = delete;
	ImageProc& operator=(const ImageProc&) = delete;
};
