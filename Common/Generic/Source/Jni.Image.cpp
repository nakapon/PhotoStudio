#include <Platform.h>

#include <ImageData.h>

#include <Jni.Base.h>
#include <Jni.Image.h>

bool Jni::Image::GetImageInfo(JNIEnv* pEnv, jobject objImageInfo, IImageData::IMAGEINFO& ImageInfo)
{
	jclass clsImageInfo;
	jfieldID Field;

	if(pEnv == nullptr || objImageInfo == nullptr)
		return false;

	clsImageInfo = pEnv->GetObjectClass(objImageInfo);
	if(clsImageInfo == nullptr)
		return false;

	// width
	Field = pEnv->GetFieldID(clsImageInfo, "width", "I");
	if(Field == nullptr)
		return false;

	ImageInfo.Width = pEnv->GetIntField(objImageInfo, Field);

	// height
	Field = pEnv->GetFieldID(clsImageInfo, "height", "I");
	if(Field == nullptr)
		return false;

	ImageInfo.Height = pEnv->GetIntField(objImageInfo, Field);

	// channel count
	Field = pEnv->GetFieldID(clsImageInfo, "channelCount", "I");
	if(Field == nullptr)
		return false;

	ImageInfo.ChannelCount = pEnv->GetIntField(objImageInfo, Field);

	// bits per channel
	Field = pEnv->GetFieldID(clsImageInfo, "bitsPerChannel", "I");
	if(Field == nullptr)
		return false;

	ImageInfo.BitsPerChannel = pEnv->GetIntField(objImageInfo, Field);

	// bytes per line
	Field = pEnv->GetFieldID(clsImageInfo, "bytesPerLine", "I");
	if(Field == nullptr)
		return false;

	ImageInfo.BytesPerLine = pEnv->GetIntField(objImageInfo, Field);

	return true;
}

jobject Jni::Image::NewImageInfo(JNIEnv* pEnv, IImageData::IMAGEINFO ImageInfo)
{
	return Jni::Base::NewObject(pEnv, "Lphotostudio/image/ImageInfo;", "(IIII)V", ImageInfo.Width, ImageInfo.Height, ImageInfo.ChannelCount, ImageInfo.BitsPerChannel);
}

jobject Jni::Image::NewImageObject(JNIEnv* pEnv)
{
	IImageData *pNewObject = nullptr;

	pNewObject = new CImageData;

	return Jni::Base::NewObject(pEnv, "Lphotostudio/image/ImageData;", "(J)V", pNewObject);
}

jobject Jni::Image::NewImageObject(JNIEnv* pEnv, IImageData *pImageObject)
{
	IImageData *pNewObject = nullptr;

	pNewObject = new CImageData;

	pImageObject->CopyTo(pNewObject);

	return Jni::Base::NewObject(pEnv, "Lphotostudio/image/ImageData;", "(J)V", pNewObject);
}
