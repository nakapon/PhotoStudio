#include <Platform.h>

#include <ImageData.h>

#include <Jni.Base.h>
#include <Jni.Image.h>

bool Jni::Image::GetImageInfo(JNIEnv* pEnv, jobject objImageInfo, IImageData::IMAGEINFO& ImageInfo)
{
	jclass clsImageInfo;
	jfieldID Field;

	if(pEnv == nullptr || objImageInfo == nullptr)
		return false;

	clsImageInfo = pEnv->GetObjectClass(objImageInfo);
	if(clsImageInfo == nullptr)
		return false;

	// width
	Field = pEnv->GetFieldID(clsImageInfo, "width", "I");
	if(Field == nullptr)
		return false;

	ImageInfo.Width = pEnv->GetIntField(objImageInfo, Field);

	// height
	Field = pEnv->GetFieldID(clsImageInfo, "height", "I");
	if(Field == nullptr)
		return false;

	ImageInfo.Height = pEnv->GetIntField(objImageInfo, Field);

	// channel count
	Field = pEnv->GetFieldID(clsImageInfo, "channelCount", "I");
	if(Field == nullptr)
		return false;

	ImageInfo.ChannelCount = pEnv->GetIntField(objImageInfo, Field);

	// bits per channel
	Field = pEnv->GetFieldID(clsImageInfo, "bitsPerChannel", "I");
	if(Field == nullptr)
		return false;

	ImageInfo.BitsPerChannel = pEnv->GetIntField(objImageInfo, Field);

	// bytes per line
	Field = pEnv->GetFieldID(clsImageInfo, "bytesPerLine", "I");
	if(Field == nullptr)
		return false;

	ImageInfo.BytesPerLine = pEnv->GetIntField(objImageInfo, Field);

	return true;
}

jobject Jni::Image::NewImageInfo(JNIEnv* pEnv, IImageData::IMAGEINFO ImageInfo)
{
	return Jni::Base::NewObject(pEnv, "Lphotostudio/image/ImageInfo;", "(IIII)V", ImageInfo.Width, ImageInfo.Height, ImageInfo.ChannelCount, ImageInfo.BitsPerChannel);
}

jobject Jni::Image::NewImageObject(JNIEnv* pEnv)
{
	IImageData *pNewObject = nullptr;

	pNewObject = new CImageData;

	return Jni::Base::NewObject(pEnv, "Lphotostudio/image/ImageData;", "(J)V", pNewObject);
}

jobject Jni::Image::NewImageObject(JNIEnv* pEnv, IImageData *pImageData)
{
	IImageData *pNewObject = nullptr;

	pNewObject = new CImageData;

	pImageData->CopyTo(pNewObject);

	return Jni::Base::NewObject(pEnv, "Lphotostudio/image/ImageData;", "(J)V", pNewObject);
}

template <typename T>
static bool ConvDibFormat(JNIEnv* pEnv, IImageData *pImageData, jbyteArray Pixels);

bool Jni::Image::ConvertToAwtDataBuffer(JNIEnv* pEnv, IImageData *pImageData, jbyteArray Pixels)
{
	bool bReturn;

	IImageData::IMAGEINFO ImageInfo = { 0 };
	UInt32 BytesPerChannel;

	if(pEnv == nullptr || pImageData == nullptr || !pImageData->IsCreated() || Pixels == nullptr)
		return false;

	ImageInfo = pImageData->GetImageInfo();

	BytesPerChannel = (ImageInfo.BitsPerChannel + 7) >> 3;

	switch(BytesPerChannel)
	{
	case 1:
		bReturn = ::ConvDibFormat<BYTE>(pEnv, pImageData, Pixels);
		break;

	case 2:
		bReturn = ::ConvDibFormat<WORD>(pEnv, pImageData, Pixels);
		break;

	case 4:
		bReturn = ::ConvDibFormat<DWORD>(pEnv, pImageData, Pixels);
		break;

	default:
		bReturn = false;
		break;
	}

	return bReturn;
}

template <typename T>
static bool ConvDibFormat(JNIEnv* pEnv, IImageData *pImageData, jbyteArray Pixels)
{
	INT i, j, k;

	IImageData::IMAGEINFO ImageInfo = { 0 };
	UInt32 DstChannelCount, DstBytesPerLine;

	BYTE *pbySrcBits;
	const T* pSrcLine, *pSrcPixel;
	jbyte* pDstBits, *pDstLine, *pDstPixel;

	Int32 BitShift;

	ImageInfo = pImageData->GetImageInfo();

	pbySrcBits = pImageData->GetDataPtr();

	pDstBits = pEnv->GetByteArrayElements(Pixels, nullptr);
	if(pDstBits == nullptr)
		return false;

	DstChannelCount = (ImageInfo.ChannelCount == 4) ? 4 : 3;
	DstBytesPerLine = ImageInfo.Width * DstChannelCount;

	BitShift = (Int32)((8 < ImageInfo.BitsPerChannel) ? (ImageInfo.BitsPerChannel - 8) : 0);

	if(1 == ImageInfo.ChannelCount)
	{
		// Alphaチャンネル以外をコピーする
		#pragma omp parallel for private(i, j, k, pSrcLine, pDstLine, pDstPixel)
		for(i = 0; i < (INT)ImageInfo.Height; i++)
		{
			pSrcLine = (T *)&pbySrcBits[i * ImageInfo.BytesPerLine];
			pDstLine = &pDstBits[DstBytesPerLine * i];

			for(j = 0; j < (INT)ImageInfo.Width; j++)
			{
				pDstPixel = &pDstLine[DstChannelCount * j];

				for(k = 0; k < 3; k++)
				{
					pDstPixel[k] = (BYTE)(pSrcLine[j] >> BitShift);
				}
			}
		}
	}
	else
	{
		UInt32 CopyChannelCount = 0;

		CopyChannelCount = PFMath::Min(DstChannelCount, ImageInfo.ChannelCount);

		if(3 <= CopyChannelCount)
		{
			static const UInt32 CHANNEL_IDX_TBL[4] = {2, 1, 0, 3};

			#pragma omp parallel for private(i, j, k, pSrcLine, pDstLine, pSrcPixel, pDstPixel)
			for(i = 0; i < (INT)ImageInfo.Height; i++)
			{
				pSrcLine = (T *)&pbySrcBits[ImageInfo.BytesPerLine * i];
				pDstLine = &pDstBits[DstBytesPerLine * i];

				for(j = 0; j < (INT)ImageInfo.Width; j++)
				{
					pSrcPixel = &pSrcLine[ImageInfo.ChannelCount * j];
					pDstPixel = &pDstLine[DstChannelCount * j];

					for(k = 0; k < (INT)CopyChannelCount; k++)
					{
						pDstPixel[k] = (BYTE)(pSrcPixel[CHANNEL_IDX_TBL[k]] >> BitShift);
					}
				}
			}
		}
		else
		{
			#pragma omp parallel for private(i, j, k, pSrcLine, pDstLine, pSrcPixel, pDstPixel)
			for(i = 0; i < (INT)ImageInfo.Height; i++)
			{
				pSrcLine = (T *)&pbySrcBits[ImageInfo.BytesPerLine * i];
				pDstLine = &pDstBits[DstBytesPerLine * i];

				for(j = 0; j < (INT)ImageInfo.Width; j++)
				{
					pSrcPixel = &pSrcLine[ImageInfo.ChannelCount * j];
					pDstPixel = &pDstLine[DstChannelCount * j];

					for(k = 0; k < (INT)CopyChannelCount; k++)
					{
						pDstPixel[k] = (BYTE)(pSrcPixel[k] >> BitShift);
					}
				}
			}
		}
	}

	pEnv->ReleaseByteArrayElements(Pixels, pDstBits, 0);

	return true;
}
