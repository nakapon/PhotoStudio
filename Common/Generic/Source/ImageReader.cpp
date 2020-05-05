#include <Platform.h>

#if PLATFORM_WINDOWS
#include <freeimage/FreeImage.h>
#pragma comment (lib, "FreeImage.lib")
#else
#define _LIBRAW_TYPES_H
#include <FreeImage.h>
#endif

#include <ImageReader.h>

static FREE_IMAGE_FORMAT GetFileType(LPCTSTR pszFilePath);

static void GetImageInfo(FIBITMAP* dib, IImageData::SImageInfo& ImageInfo);

bool ImageReader::ReadImage(LPCTSTR pszFilePath, IImageData* pImageData)
{
	bool bReturn = false;

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FREE_IMAGE_TYPE fit = FIT_UNKNOWN;
	FIBITMAP *dib;

	IImageData::SImageInfo ImageInfo = { };

	if(pszFilePath == nullptr || pImageData == nullptr)
		return false;

	fif = ::GetFileType(pszFilePath);
	if(fif == FIF_UNKNOWN)
		return false;

#if BUILD_IS_UNICODE
	dib = FreeImage_LoadU(fif, pszFilePath, 0);
#else
	dib = FreeImage_Load(fif, pszFilePath, 0);
#endif
	if(dib == nullptr)
		return false;

	fit = FreeImage_GetImageType(dib);
	switch(fit)
	{
	case FIT_BITMAP:
	case FIT_UINT16:
	case FIT_RGB16:
	case FIT_RGBA16:
	case FIT_RGBF:
	case FIT_RGBAF:
		break;

	default:
		goto LABEL_RETURN;
	}

	if(FIT_BITMAP == fit && 32 != FreeImage_GetBPP(dib))
	{
		FIBITMAP *temp = nullptr;

		temp = FreeImage_ConvertTo24Bits(dib);
		if(temp == nullptr)
			goto LABEL_RETURN;

		FreeImage_Unload(dib);
		dib = temp;
	}

	GetImageInfo(dib, ImageInfo);

	pImageData->Create(pszFilePath, ImageInfo);
	ImageInfo = pImageData->GetImageInfo(); // BytesPerLine を取得する為に Create 後に取得する

	if(ImageInfo.BitsPerChannel == 8)
	{
		static const UInt32 CHANNEL_INDEX_TBL[4] = {2, 1, 0, 3};

		UInt32 uiBytesPerLine = FreeImage_GetPitch(dib);

		PBYTE pbySrcBits, pbySrcLine;
		PBYTE pbyDstBits, pbyDstLine;
		PBYTE pbySrcPixel, pbyDstPixel;

		pbySrcBits = FreeImage_GetBits(dib);
		pbyDstBits = pImageData->GetDataPtr();

		for(UInt32 i = 0; i < ImageInfo.Height; i++)
		{
			pbySrcLine = &pbySrcBits[uiBytesPerLine * i];
			pbyDstLine = &pbyDstBits[ImageInfo.BytesPerLine * (ImageInfo.Height - 1 - i)];

			for(UInt32 j = 0; j < ImageInfo.Width; j++)
			{
				pbySrcPixel = &pbySrcLine[ImageInfo.ChannelCount * j];
				pbyDstPixel = &pbyDstLine[ImageInfo.ChannelCount * j];

				for(UInt32 k = 0; k < ImageInfo.ChannelCount; k++)
				{
					pbyDstPixel[k] = pbySrcPixel[CHANNEL_INDEX_TBL[k]];
				}
			}
		}
	}
	else if(ImageInfo.BitsPerChannel == 16)
	{
		UInt32 uiBytesPerLine = FreeImage_GetPitch(dib);

		PBYTE pbySrcBits, pbyDstBits;
		PWORD pwSrcLine, pwDstLine;
		PWORD pwSrcPixel, pwDstPixel;

		pbySrcBits = FreeImage_GetBits(dib);
		pbyDstBits = pImageData->GetDataPtr();

		for(UInt32 i = 0; i < ImageInfo.Height; i++)
		{
			pwSrcLine = (WORD *)&pbySrcBits[uiBytesPerLine * i];
			pwDstLine = (WORD *)&pbyDstBits[ImageInfo.BytesPerLine * (ImageInfo.Height - 1 - i)];

			for(UInt32 j = 0; j < ImageInfo.Width; j++)
			{
				pwSrcPixel = &pwSrcLine[ImageInfo.ChannelCount * j];
				pwDstPixel = &pwDstLine[ImageInfo.ChannelCount * j];

				for(UInt32 k = 0; k < ImageInfo.ChannelCount; k++)
				{
					pwDstPixel[k] = pwSrcPixel[k];
				}
			}
		}
	}
	else if(ImageInfo.BitsPerChannel == 32)
	{
		UInt32 uiBytesPerLine = FreeImage_GetPitch(dib);

		PBYTE pbySrcBits, pbyDstBits;
		PFLOAT pfSrcLine, pfDstLine;
		PFLOAT pfSrcPixel, pfDstPixel;

		pbySrcBits = FreeImage_GetBits(dib);
		pbyDstBits = pImageData->GetDataPtr();

		for(UInt32 i = 0; i < ImageInfo.Height; i++)
		{
			pfSrcLine = (FLOAT *)&pbySrcBits[uiBytesPerLine * i];
			pfDstLine = (FLOAT *)&pbyDstBits[ImageInfo.BytesPerLine * (ImageInfo.Height - 1 - i)];

			for(UInt32 j = 0; j < ImageInfo.Width; j++)
			{
				pfSrcPixel = &pfSrcLine[ImageInfo.ChannelCount * j];
				pfDstPixel = &pfDstLine[ImageInfo.ChannelCount * j];

				for(UInt32 k = 0; k < ImageInfo.ChannelCount; k++)
				{
					pfDstPixel[k] = pfSrcPixel[k];
				}
			}
		}
	}

	bReturn = true;

LABEL_RETURN:

	FreeImage_Unload(dib);

	return bReturn;
}

static FREE_IMAGE_FORMAT GetFileType(LPCTSTR pszFilePath)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	if(pszFilePath == nullptr)
		return FIF_UNKNOWN;

#if BUILD_IS_UNICODE
	fif = FreeImage_GetFileTypeU(pszFilePath);
#else
	fif = FreeImage_GetFileType(pszFilePath);
#endif
	if(fif == FIF_UNKNOWN)
	{
		// no signature ?
		// try to guess the file format from the file extension
#if BUILD_IS_UNICODE
		fif = FreeImage_GetFIFFromFilenameU(pszFilePath);
#else
		fif = FreeImage_GetFIFFromFilename(pszFilePath);
#endif
	}

	if(fif == FIF_UNKNOWN)
		return FIF_UNKNOWN;

	if(!FreeImage_FIFSupportsReading(fif))
		return FIF_UNKNOWN;

	return fif;
}

static void GetImageInfo(FIBITMAP* dib, IImageData::SImageInfo& ImageInfo)
{
	FREE_IMAGE_TYPE fit = FIT_UNKNOWN;


	ImageInfo.Width = FreeImage_GetWidth(dib);
	ImageInfo.Height = FreeImage_GetHeight(dib);

	fit = FreeImage_GetImageType(dib);
	switch(fit)
	{
	case FIT_BITMAP:
		ImageInfo.DataType = IImageData::EDataTypes::UInt;
		ImageInfo.ChannelCount = (32 == FreeImage_GetBPP(dib)) ? 4 : 3;
		ImageInfo.BitsPerChannel = 8;
		break;

	case FIT_UINT16:
		ImageInfo.DataType = IImageData::EDataTypes::UInt;
		ImageInfo.ChannelCount = 1;
		ImageInfo.BitsPerChannel = 16;
		break;

	case FIT_RGB16:
	case FIT_RGBA16:
		ImageInfo.DataType = IImageData::EDataTypes::UInt;
		ImageInfo.ChannelCount = (FIT_RGBA16 == fit) ? 4 : 3;
		ImageInfo.BitsPerChannel = 16;
		break;

	case FIT_RGBF:
	case FIT_RGBAF:
		ImageInfo.DataType = IImageData::EDataTypes::Float32;
		ImageInfo.ChannelCount = (FIT_RGBAF == fit) ? 4 : 3;
		ImageInfo.BitsPerChannel = 32;
		break;

	default:
		break;
	}
}
