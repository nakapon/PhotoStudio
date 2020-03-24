#pragma once

#include <IImageData.h>

class ImageProc
{
public:
	static void Fill(IImageData* pImageData, UInt32 ValueCh1, UInt32 ValueCh2, UInt32 ValueCh3);

	static void GrayScale(IImageData* pDstImage, const IImageData* pSrcImage);

private:
	ImageProc() = delete;
	~ImageProc() = delete;
	ImageProc(const ImageProc&) = delete;
	ImageProc& operator=(const ImageProc&) = delete;
};
