#include <windows.h>

#include <stdio.h>
#include <tchar.h>

#include "ImageData.h"

CImageData::CImageData()
{
	memset(&this->m_szImageName, 0, sizeof(this->m_szImageName));

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

	this->Create(ImageData.GetImageName(), ImageData.GetImageInfo());

	IMAGEINFO SrcInfo = ImageData.GetImageInfo();

	UINT LineSize = min(this->m_ImageInfo.BytesPerLine, SrcInfo.BytesPerLine);

	for(UINT i = 0; i < LineSize; i++)
	{
		memcpy(&this->m_ImageData[i * this->m_ImageInfo.BytesPerLine], &ImageData.GetDataPtr()[i * SrcInfo.BytesPerLine], LineSize);
	}

	return *this;
}

bool CImageData::Create(LPCTSTR pszImageName, const IMAGEINFO& ImageInfo)
{
	return this->Create(pszImageName, ImageInfo.Width, ImageInfo.Height, ImageInfo.ChannelCount, ImageInfo.BitsPerChannel);
}

bool CImageData::Create(LPCTSTR pszImageName, UINT Width, UINT Height, UINT ChannelCount, UINT BitsPerChannel)
{
	this->Destroy();

	UINT BytesPerLine = Width * ChannelCount * ((BitsPerChannel + 7) / 8);

	UINT ImageDataSize = Height * BytesPerLine;

	if(ImageDataSize == 0)
		return false;

	if(pszImageName != nullptr && pszImageName[0] != _T('\0'))
	{
		_tcscpy_s(this->m_szImageName, pszImageName);
	}

	this->m_ImageData.resize(ImageDataSize);

	this->m_ImageInfo.Width = Width;
	this->m_ImageInfo.Height = Height;
	this->m_ImageInfo.ChannelCount = ChannelCount;
	this->m_ImageInfo.BitsPerChannel = BitsPerChannel;
	this->m_ImageInfo.BytesPerLine = BytesPerLine;

	return true;
}

void CImageData::Destroy()
{
	memset(&this->m_szImageName, 0, sizeof(this->m_szImageName));

	memset(&this->m_ImageInfo, 0, sizeof(this->m_ImageInfo));

	this->m_ImageData.clear();
}

bool CImageData::IsCreated() const
{
	return !this->m_ImageData.empty();
}


LPCTSTR CImageData::GetImageName() const
{
	return this->m_szImageName;
}

void CImageData::SetImageName(LPCTSTR pszImageName)
{
	if(pszImageName != nullptr && pszImageName[0] != _T('\0'))
	{
		_tcscpy_s(this->m_szImageName, pszImageName);
	}
	else
	{
		memset(&this->m_szImageName, 0, sizeof(this->m_szImageName));
	}
}

const IImageData::IMAGEINFO& CImageData::GetImageInfo() const
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
