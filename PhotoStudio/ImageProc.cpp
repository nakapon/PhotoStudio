#include <windows.h>

#include "ImageProc.h"

template <typename T>
static void Fill(IImageData* pImage, UINT ValueCh1, UINT ValueCh2, UINT ValueCh3)
{
	IImageData::IMAGEINFO ImageInfo;

	BYTE* pbyBits;

	T* pLine;

	UINT MaxValue;

	ImageInfo = pImage->GetImageInfo();

	pbyBits = pImage->GetDataPtr();

	// �ő�l�N���b�v
	MaxValue = (1 << ImageInfo.BitsPerChannel) - 1;

	ValueCh1 = min(MaxValue, ValueCh1);
	ValueCh2 = min(MaxValue, ValueCh2);
	ValueCh3 = min(MaxValue, ValueCh3);

	switch(ImageInfo.ChannelCount)
	{
	case 1:
		for(UINT y = 0; y < ImageInfo.Height; y++)
		{
			pLine = (T*)&pbyBits[y * ImageInfo.BytesPerLine];

			for(UINT x = 0; x < ImageInfo.Width; x++)
			{
				pLine[x] = (T)ValueCh1;
			}
		}
		break;

	case 2:
		for(UINT y = 0; y < ImageInfo.Height; y++)
		{
			pLine = (T*)&pbyBits[y * ImageInfo.BytesPerLine];

			for(UINT x = 0; x < ImageInfo.Width; x++)
			{
				pLine[2 * x + 0] = (T)ValueCh1;
				pLine[2 * x + 1] = (T)ValueCh2;
			}
		}
		break;

	case 3:
	case 4:
		for(UINT y = 0; y < ImageInfo.Height; y++)
		{
			pLine = (T*)&pbyBits[y * ImageInfo.BytesPerLine];

			for(UINT x = 0; x < ImageInfo.Width; x++)
			{
				pLine[ImageInfo.ChannelCount * x + 0] = (T)ValueCh1;
				pLine[ImageInfo.ChannelCount * x + 1] = (T)ValueCh2;
				pLine[ImageInfo.ChannelCount * x + 2] = (T)ValueCh3;
			}
		}
		break;
	}
}

void ImageProc::Fill(IImageData* pImage, UINT ValueCh1, UINT ValueCh2, UINT ValueCh3)
{
	IImageData::IMAGEINFO ImageInfo;

	if(pImage == nullptr || !pImage->IsCreated())
		return;

	ImageInfo = pImage->GetImageInfo();

	UINT BytesPerChannel = (ImageInfo.BitsPerChannel + 7) >> 3;

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

	// �摜�������ʗp�摜�f�[�^���쐬
	pDstImage->Create(nullptr, SrcInfo);
	DstInfo = pDstImage->GetImageInfo();

	pSrcBits = pSrcImage->GetDataPtr();
	pDstBits = pDstImage->GetDataPtr();

	// 1��f�����[�v�񂵂ď�������

	for(UINT y = 0; y < SrcInfo.Height; y++)
	{
		pSrcLine = &pSrcBits[y * SrcInfo.BytesPerLine];
		pDstLine = &pDstBits[y * DstInfo.BytesPerLine];

		for(UINT x = 0; x < SrcInfo.Width; x++)
		{
			pSrcPixel = &pSrcLine[x * SrcInfo.ChannelCount];
			pDstPixel = &pDstLine[x * DstInfo.ChannelCount];

			UINT R, G, B;
			UINT Y;

			// ���W (x, y) �̉�f�l RGB
			R = pSrcPixel[0];
			G = pSrcPixel[1];
			B = pSrcPixel[2];

			// RGB -> Y �ɕϊ�
			Y = (UINT)(0.299f * R + 0.587f * G + 0.114f * B + 0.5f);

			pDstPixel[0] = Y;
			pDstPixel[1] = Y;
			pDstPixel[2] = Y;
		}
	}

}
