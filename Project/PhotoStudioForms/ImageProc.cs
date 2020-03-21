using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PhotoStudioForms
{
	public sealed class ImageProc
	{
		public static bool Grayscale(PsImage.IImageData DstImage, PsImage.IImageData SrcImage)
		{
			PsImage.ImageInfo SrcInfo = SrcImage.ImageInfo;

			if(SrcInfo.ChannelCount != 3 && SrcInfo.ChannelCount != 4)
				return false;

			if(SrcInfo.BitsPerChannel != 8)
				return false;

			if(!DstImage.IsCreated)
			{
				DstImage.Create("ProcImage", SrcInfo);
			}

			PsImage.ImageInfo DstInfo = DstImage.ImageInfo;

			unsafe
			{
				byte* pSrcBits, pSrcLine, pSrcPixel;
				byte* pDstBits, pDstLine, pDstPixel;

				pSrcBits = (byte*)SrcImage.DataPtr.ToPointer();
				pDstBits = (byte*)DstImage.DataPtr.ToPointer();

				for(int y = 0; y < SrcInfo.Height; y++)
				{
					pSrcLine = &pSrcBits[SrcInfo.BytesPerLine * y];
					pDstLine = &pDstBits[SrcInfo.BytesPerLine * y];

					for(int x = 0; x < SrcInfo.Width; x++)
					{
						pSrcPixel = &pSrcLine[SrcInfo.ChannelCount * x];
						pDstPixel = &pDstLine[SrcInfo.ChannelCount * x];

						float ValueF = 0.299f * pSrcPixel[0] + 0.587f * pSrcPixel[1] + 0.114f * pSrcPixel[2];
						int Value = System.Math.Min(255, (int)(ValueF + 0.5f));

						pDstPixel[0] = (byte)Value;
						pDstPixel[1] = (byte)Value;
						pDstPixel[2] = (byte)Value;
					}
				}
			}

			return true;
		}
	}
}
