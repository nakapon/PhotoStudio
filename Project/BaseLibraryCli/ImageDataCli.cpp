#include <Platform.h>

#include <ImageData.h>
#include "ImageDataCli.h"

PsImage::ImageData::ImageData()
	: m_pImage(nullptr)
{
	this->m_pImage = this->CreateImageObject();
}

PsImage::ImageData::~ImageData()
{
	PF_SAFE_DELETE(this->m_pImage);
}

PsImage::ImageData::!ImageData()
{
	this->~ImageData();
}

System::Boolean PsImage::ImageData::Create(System::String^ ImageName, PsImage::ImageInfo ImageInfo)
{
	bool bReturn;

	System::IntPtr Ptr = System::Runtime::InteropServices::Marshal::StringToHGlobalAuto(ImageName);

	bReturn = this->m_pImage->Create(static_cast<LPTSTR>(Ptr.ToPointer()), (::IImageData::EDataTypes)ImageInfo.DataType,
									 ImageInfo.Width, ImageInfo.Height, ImageInfo.ChannelCount, ImageInfo.BitsPerChannel);

	System::Runtime::InteropServices::Marshal::FreeHGlobal(Ptr);

	return bReturn;
}

System::Boolean PsImage::ImageData::Create(System::String^ ImageName, PsImage::DataTypes DataType, Int32 Width, Int32 Height, Int32 ChannelCount, Int32 BitsPerChannel)
{
	bool bReturn;

	System::IntPtr Ptr = System::Runtime::InteropServices::Marshal::StringToHGlobalAuto(ImageName);

	bReturn = this->m_pImage->Create(static_cast<LPTSTR>(Ptr.ToPointer()), (::IImageData::EDataTypes)DataType,
									 Width, Height, ChannelCount, BitsPerChannel);

	System::Runtime::InteropServices::Marshal::FreeHGlobal(Ptr);

	return bReturn;
}

void PsImage::ImageData::Destroy()
{
	this->m_pImage->Destroy();
}

System::Boolean PsImage::ImageData::IsCreated::get()
{
	return this->m_pImage->IsCreated();
}

void PsImage::ImageData::ImageName::set(System::String^ value)
{
	System::IntPtr Ptr = System::Runtime::InteropServices::Marshal::StringToHGlobalAuto(value);

	this->m_pImage->SetImageName(static_cast<LPTSTR>(Ptr.ToPointer()));

	System::Runtime::InteropServices::Marshal::FreeHGlobal(Ptr);
}

System::String^ PsImage::ImageData::ImageName::get()
{
	LPCTSTR pszImageName;

	System::String^ ImageName = nullptr;

	pszImageName = this->m_pImage->GetImageName();
	if(pszImageName != nullptr)
	{
		ImageName = System::Runtime::InteropServices::Marshal::PtrToStringAuto(static_cast<System::IntPtr>((LPTSTR)pszImageName));
	}
	else
	{
		ImageName = gcnew System::String(L"");
	}

	return ImageName;
}

PsImage::ImageInfo PsImage::ImageData::ImageInfo::get()
{
	::IImageData::SImageInfo Info = { };
	PsImage::ImageInfo ImageInfo;

	Info = this->m_pImage->GetImageInfo();

	ImageInfo.Width = Info.Width;
	ImageInfo.Height = Info.Height;
	ImageInfo.ChannelCount = Info.ChannelCount;
	ImageInfo.BitsPerChannel = Info.BitsPerChannel;
	ImageInfo.BytesPerLine = Info.BytesPerLine;

	return ImageInfo;
}

System::IntPtr PsImage::ImageData::DataPtr::get()
{
	return System::IntPtr(this->m_pImage->GetDataPtr());
}

::IImageData* PsImage::ImageData::NativeObject::get()
{
	return this->m_pImage;
}

::IImageData* PsImage::ImageData::CreateImageObject()
{
	return new ::CImageData;
}
