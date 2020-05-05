#pragma once

class IImageData;
#pragma make_public(IImageData)

namespace PsImage
{
	public enum class DataTypes
	{
		Unknown = 0,
		UnsignedInt,	/* Unsigned Integer */
		SingleFloat,	/* Single Precision Float (BitsPerChannel must be 32) */
		DoubleFloat,	/* Dobule Precision Float (BitsPerChannel must be 64) */
	};

	[System::Serializable]
	public value class ImageInfo
	{
	public:
		Int32 Width;			// ��
		Int32 Height;			// ����
		Int32 ChannelCount;		// �`���l����
		Int32 BitsPerChannel;	// 1�`���l��������̃r�b�g��
		Int32 BytesPerLine;		// �摜1���C�����̃o�C�g�T�C�Y
	};

	public interface class IImageData
	{
	public:
		System::Boolean Create(System::String^ ImageName, PsImage::DataTypes DataType, PsImage::ImageInfo ImageInfo);
		System::Boolean Create(System::String^ ImageName, PsImage::DataTypes DataType, Int32 Width, Int32 Height, Int32 ChannelCount, Int32 BitsPerChannel);
		void Destroy();

		property System::Boolean IsCreated
		{
			System::Boolean get();
		}

		property System::String^ ImageName
		{
			void set(System::String^ value);
			System::String^ get();
		}

		property PsImage::DataTypes DataType
		{
			PsImage::DataTypes get();
		}

		property PsImage::ImageInfo ImageInfo
		{
			PsImage::ImageInfo get();
		}

		property System::IntPtr DataPtr
		{
			System::IntPtr get();
		}

		property ::IImageData* NativeObject
		{
			::IImageData* get();
		}
	};
}
