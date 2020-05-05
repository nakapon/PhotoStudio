#pragma once

#include <ImageData.h>

class CImageView
{
public:
	CImageView();
	~CImageView();

	void Clear();

	void SetImage(IImageData* pImageData);

	void Render(HWND hWindow);

private:
	UInt32 m_TextureId;

	UInt32 m_TextureWidth;
	UInt32 m_TextureHeight;

	IImageData::SImageInfo m_ImageInfo; // original image info

	CImageData m_TextureObject;

private:
	CImageView(const CImageView&) = delete;
	CImageView& operator=(const CImageView&) = delete;
};
