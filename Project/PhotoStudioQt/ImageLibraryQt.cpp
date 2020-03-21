#include <Platform.h>

#include "ImageLibraryQt.h"

template <typename T>
static void ConvertImage(QImage* pDstImage, const IImageData* pSrcImage);

QImage* ImageLibraryQt::ImageToQImage(const IImageData* pImage)
{
	IImageData::IMAGEINFO ImageInfo = { 0 };
	UInt32 BytesPerChannel;

	QImage *pDstImage;

	ImageInfo = pImage->GetImageInfo();

	BytesPerChannel = (ImageInfo.BitsPerChannel + 7) >> 3;

	if(ImageInfo.ChannelCount == 4)
	{
		pDstImage = new QImage(ImageInfo.Width, ImageInfo.Height, QImage::Format_RGBA8888);
	}
	else
	{
		pDstImage = new QImage(ImageInfo.Width, ImageInfo.Height, QImage::Format_RGB888);
	}

	switch(BytesPerChannel)
	{
	case 1:
		::ConvertImage<BYTE>(pDstImage, pImage);
		break;

	case 2:
		::ConvertImage<WORD>(pDstImage, pImage);
		break;
	}

	return pDstImage;
}

template <typename T>
static void ConvertImage(QImage* pDstImage, const IImageData* pSrcImage)
{
	IImageData::IMAGEINFO ImageInfo = { 0 };
	UInt32 BytesPerLine;

	const BYTE *pSrc;
	const T* pSrcLine, *pSrcPixel;
	BYTE* pDst, *pDstLine, *pDstPixel;

	Int32 BitShift;

	ImageInfo = pSrcImage->GetImageInfo();
	pSrc = pSrcImage->GetDataPtr();

	pDst = pDstImage->bits();
	BytesPerLine = pDstImage->bytesPerLine();

	BitShift = (Int32)((8 < ImageInfo.BitsPerChannel) ? (ImageInfo.BitsPerChannel - 8) : 0);

	switch(ImageInfo.ChannelCount)
	{
	case 1:
		for(UInt32 i = 0; i < ImageInfo.Height; i++)
		{
			pSrcLine = (T *)&pSrc[ImageInfo.BytesPerLine * i];
			pDstLine = &pDst[BytesPerLine * i];

			for(UInt32 j = 0; j < ImageInfo.Width; j++)
			{
				UInt32 Value = pSrcLine[j];
				pDstPixel = &pDstLine[3 * j];

				if(0 != BitShift)
				{
					Value = (BYTE)(Value >> BitShift);
				}

				pDstPixel[0] = Value;
				pDstPixel[1] = Value;
				pDstPixel[2] = Value;
			}
		}
		break;

	case 3:
		for(UInt32 i = 0; i < ImageInfo.Height; i++)
		{
			pSrcLine = (T *)&pSrc[ImageInfo.BytesPerLine * i];
			pDstLine = &pDst[BytesPerLine * i];

			for(UInt32 j = 0; j < ImageInfo.Width; j++)
			{
				pSrcPixel = &pSrcLine[ImageInfo.ChannelCount * j];
				pDstPixel = &pDstLine[3 * j];

				if(0 != BitShift)
				{
					pDstPixel[0] = (BYTE)(pSrcPixel[0] >> BitShift);
					pDstPixel[1] = (BYTE)(pSrcPixel[1] >> BitShift);
					pDstPixel[2] = (BYTE)(pSrcPixel[2] >> BitShift);
				}
				else
				{
					pDstPixel[0] = (BYTE)pSrcPixel[0];
					pDstPixel[1] = (BYTE)pSrcPixel[1];
					pDstPixel[2] = (BYTE)pSrcPixel[2];
				}
			}
		}
		break;

	case 4:
		for(UInt32 i = 0; i < ImageInfo.Height; i++)
		{
			pSrcLine = (T *)&pSrc[ImageInfo.BytesPerLine * i];
			pDstLine = &pDst[BytesPerLine * i];

			for(UInt32 j = 0; j < ImageInfo.Width; j++)
			{
				pSrcPixel = &pSrcLine[ImageInfo.ChannelCount * j];
				pDstPixel = &pDstLine[4 * j];

				if(0 != BitShift)
				{
					pDstPixel[0] = (BYTE)(pSrcPixel[0] >> BitShift);
					pDstPixel[1] = (BYTE)(pSrcPixel[1] >> BitShift);
					pDstPixel[2] = (BYTE)(pSrcPixel[2] >> BitShift);
					pDstPixel[3] = (BYTE)(pSrcPixel[3] >> BitShift);
				}
				else
				{
					pDstPixel[0] = (BYTE)pSrcPixel[0];
					pDstPixel[1] = (BYTE)pSrcPixel[1];
					pDstPixel[2] = (BYTE)pSrcPixel[2];
					pDstPixel[3] = (BYTE)pSrcPixel[3];
				}
			}
		}
		break;
	}
}
