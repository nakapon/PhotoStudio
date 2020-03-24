#pragma once

#include "IImageDataCli.h"

namespace PsImage
{
	public ref class ImageWriter
	{
	public:
		static System::Boolean WriteImage(System::String^ FilePath, IImageData^ pImageData);
	};
}
