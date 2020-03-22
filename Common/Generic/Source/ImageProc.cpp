#include <Platform.h>

#include <ImageProc.h>

template <typename T>
static void Fill(IImageData* pImage, UInt32 ValueCh1, UInt32 ValueCh2, UInt32 ValueCh3)
{
	IImageData::IMAGEINFO ImageInfo;

	BYTE* pbyBits;

	T* pLine;

	UInt32 MaxValue;

	ImageInfo = pImage->GetImageInfo();

	pbyBits = pImage->GetDataPtr();

	// 最大値クリップ
	MaxValue = (1 << ImageInfo.BitsPerChannel) - 1;

	ValueCh1 = PFMath::Min(MaxValue, ValueCh1);
	ValueCh2 = PFMath::Min(MaxValue, ValueCh2);
	ValueCh3 = PFMath::Min(MaxValue, ValueCh3);

	switch(ImageInfo.ChannelCount)
	{
	case 1:
		for(UInt32 y = 0; y < ImageInfo.Height; y++)
		{
			pLine = (T*)&pbyBits[y * ImageInfo.BytesPerLine];

			for(UInt32 x = 0; x < ImageInfo.Width; x++)
			{
				pLine[x] = (T)ValueCh1;
			}
		}
		break;

	case 2:
		for(UInt32 y = 0; y < ImageInfo.Height; y++)
		{
			pLine = (T*)&pbyBits[y * ImageInfo.BytesPerLine];

			for(UInt32 x = 0; x < ImageInfo.Width; x++)
			{
				pLine[2 * x + 0] = (T)ValueCh1;
				pLine[2 * x + 1] = (T)ValueCh2;
			}
		}
		break;

	case 3:
	case 4:
		for(UInt32 y = 0; y < ImageInfo.Height; y++)
		{
			pLine = (T*)&pbyBits[y * ImageInfo.BytesPerLine];

			for(UInt32 x = 0; x < ImageInfo.Width; x++)
			{
				pLine[ImageInfo.ChannelCount * x + 0] = (T)ValueCh1;
				pLine[ImageInfo.ChannelCount * x + 1] = (T)ValueCh2;
				pLine[ImageInfo.ChannelCount * x + 2] = (T)ValueCh3;
			}
		}
		break;
	}
}

void ImageProc::Fill(IImageData* pImage, UInt32 ValueCh1, UInt32 ValueCh2, UInt32 ValueCh3)
{
	IImageData::IMAGEINFO ImageInfo;

	if(pImage == nullptr || !pImage->IsCreated())
		return;

	ImageInfo = pImage->GetImageInfo();

	UInt32 BytesPerChannel = (ImageInfo.BitsPerChannel + 7) >> 3;

	switch(BytesPerChannel)
	{
	case 1: ::Fill<BYTE>(pImage, ValueCh1, ValueCh2, ValueCh3); break;
	case 2: ::Fill<WORD>(pImage, ValueCh1, ValueCh2, ValueCh3); break;
	default: break;
	}
}

void ImageProc::GrayScale(IImageData* pDstImage, const IImageData* pSrcImage)
{
	IImageData::IMAGEINFO SrcInfo;
	IImageData::IMAGEINFO DstInfo;

	const BYTE* pSrcBits, *pSrcLine, *pSrcPixel;
	BYTE* pDstBits, *pDstLine, *pDstPixel;

	if(pDstImage == nullptr || pSrcImage == nullptr || !pSrcImage->IsCreated())
		return;

	SrcInfo = pSrcImage->GetImageInfo();

	// 画像処理結果用画像データを作成
	pDstImage->Create(nullptr, SrcInfo);
	DstInfo = pDstImage->GetImageInfo();

	pSrcBits = pSrcImage->GetDataPtr();
	pDstBits = pDstImage->GetDataPtr();

	// 1画素ずつループ回して処理する

	for(UInt32 y = 0; y < SrcInfo.Height; y++)
	{
		pSrcLine = &pSrcBits[y * SrcInfo.BytesPerLine];
		pDstLine = &pDstBits[y * DstInfo.BytesPerLine];

		for(UInt32 x = 0; x < SrcInfo.Width; x++)
		{
			pSrcPixel = &pSrcLine[x * SrcInfo.ChannelCount];
			pDstPixel = &pDstLine[x * DstInfo.ChannelCount];

			UInt32 R, G, B;
			UInt32 Y;

			// 座標 (x, y) の画素値 RGB
			R = pSrcPixel[0];
			G = pSrcPixel[1];
			B = pSrcPixel[2];

			// RGB -> Y に変換
			Y = (UInt32)(0.299f * R + 0.587f * G + 0.114f * B + 0.5f);

			pDstPixel[0] = Y;
			pDstPixel[1] = Y;
			pDstPixel[2] = Y;
		}
	}

}
