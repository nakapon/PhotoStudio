#pragma once

#include "IImageDataCli.h"

namespace PsImage
{
	public ref class ImageLibrary
	{
	public:
		static System::Boolean Convert(System::Drawing::Bitmap^ %DstImage, IImageData^ SrcImage);
	};
}
