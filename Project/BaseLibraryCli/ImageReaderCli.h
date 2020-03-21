#pragma once

#include "IImageDataCli.h"

namespace PsImage
{
	public ref class ImageReader
	{
	public:
		static System::Boolean ReadImage(System::String^ FilePath, IImageData^ pImage);
	};
}
