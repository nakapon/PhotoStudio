#include <windows.h>

#include "ImageProc.h"

void ImageProc::GrayScale(CImageData& ProcImage, const CImageData& ImageData)
{
	IMAGEINFO SrcInfo = ImageData.GetImageInfo();
	IMAGEINFO DstInfo;

	const BYTE* pSrcBits, *pSrcLine, *pSrcPixel;
	BYTE* pDstBits, *pDstLine, *pDstPixel;

	// �摜�������ʗp�摜�f�[�^���쐬
	ProcImage.Create(SrcInfo);
	DstInfo = ProcImage.GetImageInfo();

	pSrcBits = ImageData.GetDataPtr();
	pDstBits = ProcImage.GetDataPtr();

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
