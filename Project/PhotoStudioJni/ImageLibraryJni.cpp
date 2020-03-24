#include <Platform.h>

#include <Jni.Base.h>
#include <Jni.Image.h>

#include <photostudio_image_ImageLibrary.h>

#define HANDLE_NAME "_handle"

JNIEXPORT jboolean JNICALL Java_photostudio_image_ImageLibrary_getDataBufferJni(JNIEnv* pEnv, jclass, jbyteArray Pixels, jobject ImageData)
{
	IImageData* pImageData = nullptr;

	pImageData = (IImageData *)Jni::Base::GetNativeHandle(pEnv, ImageData, HANDLE_NAME);
	if(pImageData == nullptr)
		return false;

	return Jni::Image::ConvertToAwtDataBuffer(pEnv, pImageData, Pixels);
}
