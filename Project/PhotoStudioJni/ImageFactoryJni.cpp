#include <Platform.h>

#include <Jni.Base.h>
#include <Jni.Image.h>

#include <photostudio_image_ImageFactory.h>

#define HANDLE_NAME "_handle"

JNIEXPORT jobject JNICALL Java_photostudio_image_ImageFactory_createInstance(JNIEnv* pEnv, jclass)
{
	return Jni::Image::NewImageObject(pEnv);
}

JNIEXPORT jobject JNICALL Java_photostudio_image_ImageFactory_duplicateInstance(JNIEnv* pEnv, jclass, jobject ImageData)
{
	IImageData* pImageData;

	pImageData = (IImageData *)Jni::Base::GetNativeHandle(pEnv, ImageData, HANDLE_NAME);
	if(pImageData == nullptr)
		return nullptr;

	return Jni::Image::NewImageObject(pEnv, pImageData);
}

JNIEXPORT void JNICALL Java_photostudio_image_ImageFactory_destroyInstance(JNIEnv* pEnv, jclass, jobject ImageData)
{
	IImageData* pImageData;

	pImageData = (IImageData *)Jni::Base::GetNativeHandle(pEnv, ImageData, HANDLE_NAME);
	if(pImageData != nullptr)
	{
		delete pImageData;
	}
}
