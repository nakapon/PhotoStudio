#pragma once

#include "IImageData.h"

class ImageRenderer
{
public:
	static bool Render(HDC hDC, SIZE ClientSize, const IImageData* pImageData);

private:
	ImageRenderer() = delete;
	~ImageRenderer() = delete;
	ImageRenderer(const ImageRenderer&) = delete;
	ImageRenderer& operator=(const ImageRenderer&) = delete;
};
