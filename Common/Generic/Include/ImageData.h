#pragma once

#include <vector>

#include "IImageData.h"

class CImageData : public IImageData
{
public:
	CImageData();
	virtual ~CImageData();
	CImageData(const CImageData& ImageData);
	CImageData& operator=(const CImageData& ImageData);

	virtual bool Create(LPCTSTR pszImageName, const IMAGEINFO& ImageInfo) override;

	virtual bool Create(LPCTSTR pszImageName, UINT Width, UINT Height, UINT ChannelCount, UINT BitsPerChannel) override;

	virtual void Destroy() override;

	virtual bool IsCreated() const override;

	virtual LPCTSTR GetImageName() const override;

	virtual void SetImageName(LPCTSTR pszImageName) override;

	virtual const IMAGEINFO& GetImageInfo() const override;

	virtual const BYTE* GetDataPtr() const override;
	virtual BYTE* GetDataPtr() override;

private:
	// TODO ������N���X����낤 (std::basic_string �͂�����Ƃˁc)
	TCHAR m_szImageName[MAX_PATH];

	IMAGEINFO m_ImageInfo;

	std::vector<BYTE> m_ImageData;
};