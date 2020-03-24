#include <Platform.h>

#include <ImageWriter.h>
#include "ImageWriterCli.h"

System::Boolean PsImage::ImageWriter::WriteImage(System::String^ FilePath, IImageData^ pImageData)
{
	bool bReturn;

	System::IntPtr Ptr = System::Runtime::InteropServices::Marshal::StringToHGlobalAuto(FilePath);

	bReturn = ::ImageWriter::WriteImage(static_cast<LPTSTR>(Ptr.ToPointer()), pImageData->NativeObject);

	System::Runtime::InteropServices::Marshal::FreeHGlobal(Ptr);

	return bReturn;
}
