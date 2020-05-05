#pragma once

#include <vector>

#include <IImageData.h>

class CImageData : public IImageData
{
public:
	CImageData();
	virtual ~CImageData();
	CImageData(const CImageData& ImageData);
	CImageData& operator=(const CImageData& ImageData);

	virtual bool Create(LPCTSTR pszImageName, EDataTypes DataType, const SImageInfo& ImageInfo) override;

	virtual bool Create(LPCTSTR pszImageName, EDataTypes DataType, UInt32 Width, UInt32 Height, UInt32 ChannelCount, UInt32 BitsPerChannel) override;

	virtual void Destroy() override;

	virtual bool IsCreated() const override;

	virtual LPCTSTR GetImageName() const override;

	virtual void SetImageName(LPCTSTR pszImageName) override;

	virtual EDataTypes GetDataType() const override;

	virtual const SImageInfo& GetImageInfo() const override;

	virtual const BYTE* GetDataPtr() const override;
	virtual BYTE* GetDataPtr() override;

	virtual bool CopyTo(IImageData* pImageData) const override;

private:
	FString m_ImageName;

	EDataTypes m_DataType;

	SImageInfo m_ImageInfo;

	std::vector<BYTE> m_ImageData;
};
