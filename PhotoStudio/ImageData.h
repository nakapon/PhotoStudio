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

	virtual bool Create(const IMAGEINFO& ImageInfo) override;

	virtual void Destroy() override;

	virtual bool IsCreated() const override;

	virtual const IMAGEINFO& GetImageInfo() const override;

	virtual const BYTE* GetDataPtr() const override;
	virtual BYTE* GetDataPtr() override;

private:

	IMAGEINFO m_ImageInfo;

	std::vector<BYTE> m_ImageData;
};
