#include <Platform.h>

#include <Jni.Base.h>
#include <Jni.Image.h>

#include <photostudio_image_ImageData.h>

#define HANDLE_NAME "_handle"

JNIEXPORT jboolean JNICALL Java_photostudio_image_ImageData_createJni(JNIEnv* pEnv, jobject objImage, jstring ImageName, jint Width, jint Height, jint ChannelCount, jint BitsPerChannel)
{
	bool bReturn;

	IImageData* pImageData;

	pImageData = (IImageData *)Jni::Base::GetNativeHandle(pEnv, objImage, HANDLE_NAME);
	if(pImageData == nullptr)
		return false;

	LPCWSTR pszImageName = (LPCWSTR)pEnv->GetStringChars(ImageName, nullptr);

	bReturn = pImageData->Create(pszImageName, IImageData::EDataTypes::UInt, Width, Height, ChannelCount, BitsPerChannel);

	pEnv->ReleaseStringChars(ImageName, (const jchar*)pszImageName);

	return bReturn;
}

JNIEXPORT jboolean JNICALL Java_photostudio_image_ImageData_destroyJni(JNIEnv* pEnv, jobject objImage)
{
	IImageData* pImageData;

	pImageData = (IImageData *)Jni::Base::GetNativeHandle(pEnv, objImage, HANDLE_NAME);
	if(pImageData == nullptr)
		return false;

	pImageData->Destroy();

	return true;
}

JNIEXPORT jboolean JNICALL Java_photostudio_image_ImageData_isCreatedJni(JNIEnv* pEnv, jobject objImage)
{
	IImageData* pImageData;

	pImageData = (IImageData *)Jni::Base::GetNativeHandle(pEnv, objImage, HANDLE_NAME);
	if(pImageData == nullptr)
		return false;

	return pImageData->IsCreated();
}

JNIEXPORT void JNICALL Java_photostudio_image_ImageData_setImageNameJni(JNIEnv* pEnv, jobject objImage, jstring ImageName)
{
	IImageData* pImageData;

	pImageData = (IImageData *)Jni::Base::GetNativeHandle(pEnv, objImage, HANDLE_NAME);
	if(pImageData == nullptr)
		return;

	LPCWSTR pszImageName = (LPCWSTR)pEnv->GetStringChars(ImageName, nullptr);

	pImageData->SetImageName(pszImageName);

	pEnv->ReleaseStringChars(ImageName, (const jchar*)pszImageName);
}

JNIEXPORT jstring JNICALL Java_photostudio_image_ImageData_getImageNameJni(JNIEnv* pEnv, jobject objImage)
{
	IImageData* pImageData;

	WCHAR szImageName[MAX_PATH] = { 0 };

	pImageData = (IImageData *)Jni::Base::GetNativeHandle(pEnv, objImage, HANDLE_NAME);
	if(pImageData != nullptr)
	{
		PFStringW::Copy(szImageName, pImageData->GetImageName());
	}

	return pEnv->NewString((const jchar *)szImageName, (jsize)PFString::Length(szImageName));
}

JNIEXPORT jobject JNICALL Java_photostudio_image_ImageData_getImageInfoJni(JNIEnv* pEnv, jobject objImage)
{
	IImageData* pImageData;

	IImageData::SImageInfo ImageInfo = { };

	pImageData = (IImageData *)Jni::Base::GetNativeHandle(pEnv, objImage, HANDLE_NAME);
	if(pImageData != nullptr)
	{
		ImageInfo = pImageData->GetImageInfo();
	}

	return Jni::Image::NewImageInfo(pEnv, ImageInfo);
}

JNIEXPORT jintArray JNICALL Java_photostudio_image_ImageData_getPixelJni(JNIEnv* pEnv, jobject objImage, jint x, jint y)
{
	IImageData* pImageData;

	IImageData::SImageInfo ImageInfo = { };

	pImageData = (IImageData *)Jni::Base::GetNativeHandle(pEnv, objImage, HANDLE_NAME);
	if(pImageData == nullptr)
		return nullptr;

	ImageInfo = pImageData->GetImageInfo();

	jintArray Array = pEnv->NewIntArray(ImageInfo.ChannelCount);
	jint* pArrayBits = pEnv->GetIntArrayElements(Array, nullptr);

	{
		UInt32 BytePerChannel = (ImageInfo.BitsPerChannel + 7) >> 3;
		UInt32 BytePerPixel = ImageInfo.ChannelCount * BytePerChannel;

		const BYTE* pbyBits = pImageData->GetDataPtr();
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
	IImageData* pImageData;

	IImageData::SImageInfo ImageInfo = { };

	pImageData = (IImageData *)Jni::Base::GetNativeHandle(pEnv, objImage, HANDLE_NAME);
	if(pImageData == nullptr)
		return false;

	ImageInfo = pImageData->GetImageInfo();

	if((UInt32)pEnv->GetArrayLength(Pixel) < ImageInfo.ChannelCount)
		return false;

	{
		jint Buffer[8] = { 0 };

		UInt32 BytePerChannel = (ImageInfo.BitsPerChannel + 7) >> 3;
		UInt32 BytePerPixel = ImageInfo.ChannelCount * BytePerChannel;

		BYTE* pbyBits = pImageData->GetDataPtr();
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
