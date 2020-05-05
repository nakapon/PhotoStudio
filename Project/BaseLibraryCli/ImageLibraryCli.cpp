#include <Platform.h>

#include <IImageData.h>

#include "ImageLibraryCli.h"

static System::Drawing::Imaging::PixelFormat ConvChannelCountToDrawingPixelFormat(Int32 ChannelCount)
{
	System::Drawing::Imaging::PixelFormat Format = System::Drawing::Imaging::PixelFormat::Format24bppRgb;

	switch(ChannelCount)
	{
	case 1:
		Format = System::Drawing::Imaging::PixelFormat::Format8bppIndexed;
		break;

	case 3:
		Format = System::Drawing::Imaging::PixelFormat::Format24bppRgb;
		break;

	case 4:
		Format = System::Drawing::Imaging::PixelFormat::Format32bppRgb;
		break;

	default:
		break;
	}

	return Format;
}

System::Boolean PsImage::ImageLibrary::Convert(System::Drawing::Bitmap^ %DstImage, IImageData^ SrcImage)
{
	::IImageData *pSrcImage;
	::IImageData::SImageInfo ImageInfo = { 0 };

	Int32 BitShift;

	if(System::Object::ReferenceEquals(SrcImage, nullptr) || !SrcImage->IsCreated)
		return false;

	pSrcImage = SrcImage->NativeObject;
	ImageInfo = pSrcImage->GetImageInfo();

	BitShift = (Int32)((8 < ImageInfo.BitsPerChannel) ? (ImageInfo.BitsPerChannel - 8) : 0);

	if(System::Object::Equals(DstImage, nullptr))
	{
		System::Drawing::Imaging::PixelFormat Format = ::ConvChannelCountToDrawingPixelFormat(ImageInfo.ChannelCount);

		DstImage = gcnew System::Drawing::Bitmap(ImageInfo.Width, ImageInfo.Height, Format);

		if(1 == ImageInfo.ChannelCount)
		{
			System::Drawing::Imaging::ColorPalette^ Palette = DstImage->Palette;

			for(UInt32 i = 0; i < 256; i++)
			{
				Palette->Entries[i] = System::Drawing::Color::FromArgb(i, i, i);
			}

			DstImage->Palette = Palette;
		}
	}

	{
		static const Int32 CHANNEL_IDX_TBL[4] = {2, 1, 0, 3};

		UInt32 x, y, p;

		PBYTE pbySrcBits;
		PBYTE pbyDstBits, pbyDstLine, pbyDstPixel;

		System::Drawing::Rectangle lockRect(0, 0, ImageInfo.Width, ImageInfo.Height);

		System::Drawing::Imaging::BitmapData^ BitmapData = DstImage->LockBits(lockRect
																			, System::Drawing::Imaging::ImageLockMode::ReadWrite
																			, DstImage->PixelFormat);

		pbySrcBits = pSrcImage->GetDataPtr();

		pbyDstBits = (PBYTE)BitmapData->Scan0.ToPointer();

		if(8 < ImageInfo.BitsPerChannel)
		{
			// 16bit
			WORD* pwSrcLine, *pwSrcPixel;

			if(1 == ImageInfo.ChannelCount)
			{
				// 1ch
				for(y = 0; y < ImageInfo.Height; y++)
				{
					pwSrcLine = (WORD *)&pbySrcBits[ImageInfo.BytesPerLine * y];
					pbyDstLine = &pbyDstBits[BitmapData->Stride * y];

					for(x = 0; x < ImageInfo.Width; x++)
					{
						pbyDstLine[x] = (BYTE)(pwSrcLine[x] >> BitShift);
					}
				}
			}
			else
			{
				// 3ch or 4ch
				for(y = 0; y < ImageInfo.Height; y++)
				{
					pwSrcLine = (WORD *)&pbySrcBits[ImageInfo.BytesPerLine * y];
					pbyDstLine = &pbyDstBits[BitmapData->Stride * y];

					for(x = 0; x < ImageInfo.Width; x++)
					{
						pwSrcPixel = &pwSrcLine[ImageInfo.ChannelCount * x];
						pbyDstPixel = &pbyDstLine[ImageInfo.ChannelCount * x];

						for(p = 0; p < ImageInfo.ChannelCount; p++)
						{
							pbyDstPixel[p] = (BYTE)(pwSrcPixel[CHANNEL_IDX_TBL[p]] >> BitShift);
						}
					}
				}
			}
		}
		else
		{
			// 8bit
			PBYTE pbySrcLine, pbySrcPixel;

			if(1 == ImageInfo.ChannelCount)
			{
				// 1ch
				for(y = 0; y < ImageInfo.Height; y++)
				{
					pbySrcLine = &pbySrcBits[ImageInfo.BytesPerLine * y];
					pbyDstLine = &pbyDstBits[BitmapData->Stride * y];

					for(x = 0; x < ImageInfo.Width; x++)
					{
						pbyDstLine[x] = pbySrcLine[x];
					}
				}
			}
			else
			{
				// 3ch or 4ch
				for(y = 0; y < ImageInfo.Height; y++)
				{
					pbySrcLine = &pbySrcBits[ImageInfo.BytesPerLine * y];
					pbyDstLine = &pbyDstBits[BitmapData->Stride * y];

					for(x = 0; x < ImageInfo.Width; x++)
					{
						pbySrcPixel = &pbySrcLine[ImageInfo.ChannelCount * x];
						pbyDstPixel = &pbyDstLine[ImageInfo.ChannelCount * x];

						for(p = 0; p < ImageInfo.ChannelCount; p++)
						{
							pbyDstPixel[p] = pbySrcPixel[CHANNEL_IDX_TBL[p]];
						}
					}
				}
			}
		}

		DstImage->UnlockBits(BitmapData);
	}

	return true;
}
