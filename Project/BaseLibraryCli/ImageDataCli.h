#pragma once

#include "IImageDataCli.h"

namespace PsImage
{
	public ref class ImageData : public System::MarshalByRefObject, public IImageData
	{
	public:
		ImageData();
		~ImageData();
		!ImageData();

		virtual System::Boolean Create(System::String^ ImageName, PsImage::ImageInfo ImageInfo);
		virtual System::Boolean Create(System::String^ ImageName, PsImage::DataTypes DataType, Int32 Width, Int32 Height, Int32 ChannelCount, Int32 BitsPerChannel);
		virtual void Destroy();
	
		property System::Boolean IsCreated
		{
			virtual System::Boolean get();
		}

		property System::String^ ImageName
		{
			virtual void set(System::String^ value);
			virtual System::String^ get();
		}

		property PsImage::ImageInfo ImageInfo
		{
			virtual PsImage::ImageInfo get();
		}

		property System::IntPtr DataPtr
		{
			virtual System::IntPtr get();
		}

		property ::IImageData* NativeObject
		{
			virtual ::IImageData* get();
		}

	protected:
		::IImageData* m_pImage;

	protected:
		virtual ::IImageData* CreateImageObject();
	};
}
