#pragma once

#include "IImageData.h"

class ImageProc
{
public:
	static void Fill(IImageData* pImage, UINT ValueCh1, UINT ValueCh2, UINT ValueCh3);

	static void GrayScale(IImageData* pDstImage, const IImageData* pSrcImage);

private:
	ImageProc() = delete;
	~ImageProc() = delete;
	ImageProc(const ImageProc&) = delete;
	ImageProc& operator=(const ImageProc&) = delete;
};
