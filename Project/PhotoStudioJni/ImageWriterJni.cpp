#include <Platform.h>

#include <Jni.Base.h>

#include <ImageWriter.h>

#include <photostudio_image_ImageWriter.h>

#define HANDLE_NAME "_handle"

JNIEXPORT jboolean JNICALL Java_photostudio_image_ImageWriter_write(JNIEnv* pEnv, jclass, jstring FilePath, jobject ImageData)
{
	bool bReturn;

	IImageData *pImage;

	pImage = (IImageData *)Jni::Base::GetNativeHandle(pEnv, ImageData, HANDLE_NAME);
	if(pImage == nullptr || !pImage->IsCreated())
		return false;

	LPCWSTR pszFilePath = (LPCWSTR)pEnv->GetStringChars(FilePath, nullptr);

	bReturn = ImageWriter::WriteImage(pszFilePath, pImage);

	pEnv->ReleaseStringChars(FilePath, (const jchar*)pszFilePath);

	return bReturn;
}
