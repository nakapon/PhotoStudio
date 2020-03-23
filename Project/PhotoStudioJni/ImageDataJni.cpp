#include <Platform.h>

#include <Jni.Base.h>
#include <Jni.Image.h>

#include <photostudio_image_ImageData.h>

#define HANDLE_NAME "_handle"

JNIEXPORT jboolean JNICALL Java_photostudio_image_ImageData_createJni(JNIEnv* pEnv, jobject objImage, jstring ImageName, jint Width, jint Height, jint ChannelCount, jint BitsPerChannel)
{
	bool bReturn;

	IImageData *pImage;

	pImage = (IImageData *)Jni::Base::GetNativeHandle(pEnv, objImage, HANDLE_NAME);
	if(pImage == nullptr)
		return false;

	LPCWSTR pszImageName = (LPCWSTR)pEnv->GetStringChars(ImageName, nullptr);

	bReturn = pImage->Create(pszImageName, Width, Height, ChannelCount, BitsPerChannel);

	pEnv->ReleaseStringChars(ImageName, (const jchar*)pszImageName);

	return bReturn;
}

JNIEXPORT jboolean JNICALL Java_photostudio_image_ImageData_destroyJni(JNIEnv* pEnv, jobject objImage)
{
	IImageData *pImage;

	pImage = (IImageData *)Jni::Base::GetNativeHandle(pEnv, objImage, HANDLE_NAME);
	if(pImage == nullptr)
		return false;

	pImage->Destroy();

	return true;
}

JNIEXPORT jboolean JNICALL Java_photostudio_image_ImageData_isCreatedJni(JNIEnv* pEnv, jobject objImage)
{
	IImageData *pImage;

	pImage = (IImageData *)Jni::Base::GetNativeHandle(pEnv, objImage, HANDLE_NAME);
	if(pImage == nullptr)
		return false;

	return pImage->IsCreated();
}

JNIEXPORT void JNICALL Java_photostudio_image_ImageData_setImageNameJni(JNIEnv* pEnv, jobject objImage, jstring ImageName)
{
	IImageData *pImage;

	pImage = (IImageData *)Jni::Base::GetNativeHandle(pEnv, objImage, HANDLE_NAME);
	if(pImage == nullptr)
		return;

	LPCWSTR pszImageName = (LPCWSTR)pEnv->GetStringChars(ImageName, nullptr);

	pImage->SetImageName(pszImageName);

	pEnv->ReleaseStringChars(ImageName, (const jchar*)pszImageName);
}

JNIEXPORT jstring JNICALL Java_photostudio_image_ImageData_getImageNameJni(JNIEnv* pEnv, jobject objImage)
{
	IImageData *pImage;

	WCHAR szImageName[MAX_PATH] = { 0 };

	pImage = (IImageData *)Jni::Base::GetNativeHandle(pEnv, objImage, HANDLE_NAME);
	if(pImage != nullptr)
	{
		PFStringW::Copy(szImageName, pImage->GetImageName());
	}

	return pEnv->NewString((const jchar *)szImageName, (jsize)PFString::Length(szImageName));
}

JNIEXPORT jobject JNICALL Java_photostudio_image_ImageData_getImageInfoJni(JNIEnv* pEnv, jobject objImage)
{
	IImageData *pImage;

	IImageData::IMAGEINFO ImageInfo = { 0 };

	pImage = (IImageData *)Jni::Base::GetNativeHandle(pEnv, objImage, HANDLE_NAME);
	if(pImage != nullptr)
	{
		ImageInfo = pImage->GetImageInfo();
	}

	return Jni::Image::NewImageInfo(pEnv, ImageInfo);
}

JNIEXPORT jintArray JNICALL Java_photostudio_image_ImageData_getPixelJni(JNIEnv* pEnv, jobject objImage, jint x, jint y)
{
	IImageData *pImage;

	IImageData::IMAGEINFO ImageInfo = { 0 };

	pImage = (IImageData *)Jni::Base::GetNativeHandle(pEnv, objImage, HANDLE_NAME);
	if(pImage == nullptr)
		return nullptr;

	ImageInfo = pImage->GetImageInfo();

	jintArray Array = pEnv->NewIntArray(ImageInfo.ChannelCount);
	jint* pArrayBits = pEnv->GetIntArrayElements(Array, nullptr);

	{
		UInt32 BytePerChannel = (ImageInfo.BitsPerChannel + 7) >> 3;
		UInt32 BytePerPixel = ImageInfo.ChannelCount * BytePerChannel;

		const BYTE* pbyBits = pImage->GetDataPtr();
		pbyBits += y * ImageInfo.BytesPerLine + x * BytePerPixel;

		switch(BytePerChannel)
		{
		case 1:
			for(UInt32 i = 0; i < ImageInfo.ChannelCount; i++)
			{
				pArrayBits[i] = (jint)pbyBits[i];
			}
			break;

		case 2:
			for(UInt32 i = 0; i < ImageInfo.ChannelCount; i++)
			{
				pArrayBits[i] = (jint)((WORD *)pbyBits)[i];
			}
			break;
		}
	}

	pEnv->ReleaseIntArrayElements(Array, pArrayBits, 0);

	return Array;
}

JNIEXPORT jboolean JNICALL Java_photostudio_image_ImageData_setPixelJni(JNIEnv* pEnv, jobject objImage, jint x, jint y, jintArray Pixel)
{
	IImageData *pImage;

	IImageData::IMAGEINFO ImageInfo = { 0 };

	pImage = (IImageData *)Jni::Base::GetNativeHandle(pEnv, objImage, HANDLE_NAME);
	if(pImage == nullptr)
		return false;

	ImageInfo = pImage->GetImageInfo();

	if((UInt32)pEnv->GetArrayLength(Pixel) < ImageInfo.ChannelCount)
		return false;

	{
		jint Buffer[8] = { 0 };

		UInt32 BytePerChannel = (ImageInfo.BitsPerChannel + 7) >> 3;
		UInt32 BytePerPixel = ImageInfo.ChannelCount * BytePerChannel;

		BYTE* pbyBits = pImage->GetDataPtr();
		pbyBits += y * ImageInfo.BytesPerLine + x * BytePerPixel;

		pEnv->GetIntArrayRegion(Pixel, 0, ImageInfo.ChannelCount, Buffer);

		switch(BytePerChannel)
		{
		case 1:
			for(UInt32 i = 0; i < ImageInfo.ChannelCount; i++)
			{
				pbyBits[i] = (BYTE)Buffer[i];
			}
			break;

		case 2:
			for(UInt32 i = 0; i < ImageInfo.ChannelCount; i++)
			{
				((WORD *)pbyBits)[i] = (WORD)Buffer[i];
			}
			break;
		}
	}

	return true;
}
