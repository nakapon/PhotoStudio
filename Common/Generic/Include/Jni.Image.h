#pragma once

#include <jni.h>

#include <IImageData.h>

namespace Jni
{
	class Image
	{
	public:
		static bool GetImageInfo(JNIEnv* pEnv, jobject objImageInfo, IImageData::IMAGEINFO& ImageInfo);

		static jobject NewImageInfo(JNIEnv* pEnv, IImageData::IMAGEINFO ImageInfo);

		static jobject NewImageObject(JNIEnv* pEnv);
		static jobject NewImageObject(JNIEnv* pEnv, IImageData *pImageData);

		static bool ConvertToAwtDataBuffer(JNIEnv* pEnv, IImageData *pImageData, jbyteArray Pixels);

	private:
		Image() = delete;
		~Image() = delete;
		Image(const Image&) = delete;
		Image& operator=(const Image&) = delete;
	};
}
