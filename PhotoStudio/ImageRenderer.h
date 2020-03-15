#pragma once

#include "ImageData.h"

class ImageRenderer
{
public:
	static bool Render(HDC hDC, SIZE ClientSize, const CImageData& ImageData);

private:
	ImageRenderer() = delete;
	~ImageRenderer() = delete;
	ImageRenderer(const ImageRenderer&) = delete;
	ImageRenderer& operator=(const ImageRenderer&) = delete;
};

