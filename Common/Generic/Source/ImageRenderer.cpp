#include <windows.h>

#include "ImageRenderer.h"

bool ImageRenderer::Render(HDC hDC, SIZE ClientSize, const IImageData* pImageData)
{
	IImageData::IMAGEINFO ImageInfo;;

	BITMAPINFO BitmapInfo = { 0 };
	HBITMAP hBitmap;
	HDC hMemDC;

	const BYTE* pbySrcBits, *pbySrcBitsLine;

	BYTE* pbyDIBits, *pbyDIBitsLine;
	UINT DIBBytesPerLine;

	if(pImageData == nullptr || !pImageData->IsCreated())
		return false;

	ImageInfo = pImageData->GetImageInfo();

	pbySrcBits = pImageData->GetDataPtr();

	{
		BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		BitmapInfo.bmiHeader.biWidth = ImageInfo.Width;
		BitmapInfo.bmiHeader.biHeight = -1 * ImageInfo.Height;
		BitmapInfo.bmiHeader.biPlanes = 1;
		BitmapInfo.bmiHeader.biBitCount = 32;
		BitmapInfo.bmiHeader.biCompression = BI_RGB;

		hBitmap = CreateDIBSection(hDC, &BitmapInfo, DIB_RGB_COLORS, (void**)&pbyDIBits, NULL, 0);
	}

	DIBBytesPerLine = 4 * ImageInfo.Width;

	hMemDC = CreateCompatibleDC(hDC);

	SelectObject(hMemDC, hBitmap);

	for(UINT i = 0; i < ImageInfo.Height; i++)
	{
		pbySrcBitsLine = &pbySrcBits[i * ImageInfo.BytesPerLine];
		pbyDIBitsLine = &pbyDIBits[i * DIBBytesPerLine];

		for(UINT j = 0; j < ImageInfo.Width; j++)
		{
			pbyDIBitsLine[4 * j + 0] = pbySrcBitsLine[ImageInfo.ChannelCount * j + 2];
			pbyDIBitsLine[4 * j + 1] = pbySrcBitsLine[ImageInfo.ChannelCount * j + 1];
			pbyDIBitsLine[4 * j + 2] = pbySrcBitsLine[ImageInfo.ChannelCount * j + 0];
		}
	}

#if 0
	BitBlt(hDC, 0, 0, ImageInfo.Width, ImageInfo.Height, hMemDC, 0, 0, SRCCOPY);
#else
	// 画面中央にウィンドウに納まるようにスケールして表示
	float ScaleH, ScaleV, Scale;
	ScaleH = (float)ClientSize.cx / ImageInfo.Width;
	ScaleV = (float)ClientSize.cy / ImageInfo.Height;
	Scale = min(ScaleH, ScaleV);

	UINT ScaledImageWidth, ScaledImageHeight;
	ScaledImageWidth = min(ClientSize.cx, (LONG)(Scale * ImageInfo.Width + 0.5f));
	ScaledImageHeight = min(ClientSize.cy, (LONG)(Scale * ImageInfo.Height + 0.5f));

	SetStretchBltMode(hDC, COLORONCOLOR);

	StretchBlt(hDC, (ClientSize.cx - ScaledImageWidth) / 2, (ClientSize.cy - ScaledImageHeight) / 2, ScaledImageWidth, ScaledImageHeight
			 , hMemDC, 0, 0, ImageInfo.Width, ImageInfo.Height, SRCCOPY);
#endif

	DeleteDC(hMemDC);
	DeleteObject(hBitmap);

	return true;
}
