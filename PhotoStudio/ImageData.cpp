#include <windows.h>

#include "ImageData.h"

CImageData::CImageData()
{
	memset(&this->m_ImageInfo, 0, sizeof(this->m_ImageInfo));
}

CImageData::~CImageData()
{
	this->Destroy();
}

CImageData::CImageData(const CImageData& ImageData)
{
	*this = ImageData;
}

CImageData& CImageData::operator=(const CImageData& ImageData)
{
	this->Destroy();

	this->Create(ImageData.GetImageInfo());

	IMAGEINFO SrcInfo = ImageData.GetImageInfo();

	UINT LineSize = min(this->m_ImageInfo.BytesPerLine, SrcInfo.BytesPerLine);

	for(UINT i = 0; i < LineSize; i++)
	{
		memcpy(&this->m_ImageData[i * this->m_ImageInfo.BytesPerLine], &ImageData.GetDataPtr()[i * SrcInfo.BytesPerLine], LineSize);
	}

	return *this;
}

bool CImageData::Create(const IMAGEINFO& ImageInfo)
{
	this->Destroy();

	UINT BytesPerLine = ImageInfo.Width * ImageInfo.ChannelCount * ((ImageInfo.BitsPerChannel + 7) / 8);

	UINT ImageDataSize = ImageInfo.Height * BytesPerLine;

	if(ImageDataSize == 0)
		return false;

	this->m_ImageData.resize(ImageDataSize);

	this->m_ImageInfo = ImageInfo;
	this->m_ImageInfo.BytesPerLine = BytesPerLine;

	return true;
}

void CImageData::Destroy()
{
	memset(&this->m_ImageInfo, 0, sizeof(this->m_ImageInfo));

	this->m_ImageData.clear();
}

bool CImageData::IsCreated() const
{
	return !this->m_ImageData.empty();
}

const IMAGEINFO& CImageData::GetImageInfo() const
{
	return this->m_ImageInfo;
}

const BYTE* CImageData::GetDataPtr() const
{
	if(this->m_ImageData.empty())
		return nullptr;

	return &this->m_ImageData[0];
}

BYTE* CImageData::GetDataPtr()
{
	if(this->m_ImageData.empty())
		return nullptr;

	return &this->m_ImageData[0];
}
