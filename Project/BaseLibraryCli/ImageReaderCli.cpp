#include <Platform.h>

#include <ImageReader.h>
#include "ImageReaderCli.h"

System::Boolean PsImage::ImageReader::ReadImage(System::String^ FilePath, IImageData^ pImage)
{
	bool bReturn;

	System::IntPtr Ptr = System::Runtime::InteropServices::Marshal::StringToHGlobalAuto(FilePath);

	bReturn = ::ImageReader::ReadImage(static_cast<LPTSTR>(Ptr.ToPointer()), pImage->NativeObject);

	System::Runtime::InteropServices::Marshal::FreeHGlobal(Ptr);

	return bReturn;
}
