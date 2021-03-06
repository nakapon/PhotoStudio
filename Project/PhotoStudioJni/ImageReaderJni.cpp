#include <Platform.h>

#include <Jni.Base.h>

#include <ImageReader.h>

#include <photostudio_image_ImageReader.h>

#define HANDLE_NAME "_handle"

JNIEXPORT jboolean JNICALL Java_photostudio_image_ImageReader_read(JNIEnv* pEnv, jclass, jstring FilePath, jobject ImageData)
{
	bool bReturn;

	IImageData* pImageData;

	pImageData = (IImageData *)Jni::Base::GetNativeHandle(pEnv, ImageData, HANDLE_NAME);
	if(pImageData == nullptr)
		return false;

	LPCWSTR pszFilePath = (LPCWSTR)pEnv->GetStringChars(FilePath, nullptr);

	bReturn = ImageReader::ReadImage(pszFilePath, pImageData);

	pEnv->ReleaseStringChars(FilePath, (const jchar*)pszFilePath);

	return bReturn;
}
