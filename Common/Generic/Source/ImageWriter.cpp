#include <windows.h>

#include <freeimage/FreeImage.h>
#pragma comment (lib, "FreeImage.lib")

#include "ImageWriter.h"

static FREE_IMAGE_FORMAT GetFileType(LPCTSTR pszFilePath);

bool ImageWriter::WriteImage(LPCTSTR pszFilePath, const IImageData* pImageData)
{
	bool bReturn;

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP *dib = nullptr;

	IImageData::IMAGEINFO ImageInfo = { 0 };

	if(pszFilePath == nullptr || pImageData == nullptr || !pImageData->IsCreated())
		return false;

	fif = ::GetFileType(pszFilePath);
	if(FIF_UNKNOWN == fif)
		return false;

	ImageInfo = pImageData->GetImageInfo();

	if(8 < ImageInfo.BitsPerChannel)
	{
		UINT BytesPerLine;

		const BYTE* pbySrcBits;
		const WORD *pwSrcLine, *pwSrcPixel;

		PBYTE pbyDstBits;
		PWORD pwDstLine, pwDstPixel;

		UINT SrcMax = (1 << ImageInfo.BitsPerChannel) - 1;
		UINT DstMax = (1 << 16) - 1;

		FREE_IMAGE_TYPE ImageType;
		switch(ImageInfo.ChannelCount)
		{
		case 1: ImageType = FIT_UINT16; break;
		case 4: ImageType = FIT_RGBA16; break;
		default:
		case 3: ImageType = FIT_RGB16; break;
		}

		dib = FreeImage_AllocateT(ImageType, ImageInfo.Width, ImageInfo.Height, ImageInfo.ChannelCount * 16);
		if(dib == nullptr)
			return nullptr;

		pbySrcBits = pImageData->GetDataPtr();

		BytesPerLine = FreeImage_GetPitch(dib);
		pbyDstBits = FreeImage_GetBits(dib);

		for(UINT i = 0; i < ImageInfo.Height; i++)
		{
			pwSrcLine = (WORD *)&pbySrcBits[ImageInfo.BytesPerLine * (ImageInfo.Height - 1 - i)];
			pwDstLine = (WORD *)&pbyDstBits[BytesPerLine * i];

			for(UINT j = 0; j < ImageInfo.Width; j++)
			{
				pwSrcPixel = &pwSrcLine[ImageInfo.ChannelCount * j];
				pwDstPixel = &pwDstLine[ImageInfo.ChannelCount * j];

				for(UINT k = 0; k < ImageInfo.ChannelCount; k++)
				{
					pwDstPixel[k] = (DstMax * pwSrcPixel[k]) / SrcMax;
				}
			}
		}
	}
	else
	{
		static const UINT CHANNEL_INDEX_TBL[4] = {2, 1, 0, 3};

		UINT BytesPerLine;

		const BYTE *pbySrcBits, *pbySrcLine, *pbySrcPixel;
		PBYTE pbyDstBits, pbyDstLine, pbyDstPixel;

		dib = FreeImage_AllocateT(FIT_BITMAP, ImageInfo.Width, ImageInfo.Height, ImageInfo.ChannelCount * 8);
		if(dib == nullptr)
			return nullptr;

		pbySrcBits = pImageData->GetDataPtr();

		BytesPerLine = FreeImage_GetPitch(dib);
		pbyDstBits = FreeImage_GetBits(dib);

		for(UINT i = 0; i < ImageInfo.Height; i++)
		{
			pbySrcLine = &pbySrcBits[ImageInfo.BytesPerLine * (ImageInfo.Height - 1 - i)];
			pbyDstLine = &pbyDstBits[BytesPerLine * i];

			for(UINT j = 0; j < ImageInfo.Width; j++)
			{
				pbySrcPixel = &pbySrcLine[ImageInfo.ChannelCount * j];
				pbyDstPixel = &pbyDstLine[ImageInfo.ChannelCount * j];

				for(UINT k = 0; k < ImageInfo.ChannelCount; k++)
				{
					pbyDstPixel[k] = pbySrcPixel[CHANNEL_INDEX_TBL[k]];
				}
			}
		}
	}

	// Resolution
	{
//		FreeImage_SetDotsPerMeterX(dib, (UINT)((dwHRes >> 16) / 0.0254));
//		FreeImage_SetDotsPerMeterY(dib, (UINT)((dwVRes >> 16) / 0.0254));
	}

#if UNICODE
	bReturn = FreeImage_SaveU(fif, dib, pszFilePath, 0) ? true : false;
#else
	bReturn = FreeImage_Save(fif, dib, pszFilePath, 0) ? true : false;
#endif

	FreeImage_Unload(dib);

	return bReturn;
}

static FREE_IMAGE_FORMAT GetFileType(LPCTSTR pszFilePath)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	if(pszFilePath == nullptr)
		return FIF_UNKNOWN;

#if UNICODE
	fif = FreeImage_GetFIFFromFilenameU(pszFilePath);
#else
	fif = FreeImage_GetFIFFromFilename(pszFilePath);
#endif
	if(fif == FIF_UNKNOWN)
		return FIF_UNKNOWN;

	if(!FreeImage_FIFSupportsWriting(fif))
		return FIF_UNKNOWN;

	return fif;
}
