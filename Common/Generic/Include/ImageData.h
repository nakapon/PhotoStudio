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

	virtual bool Create(LPCTSTR pszImageName, const IMAGEINFO& ImageInfo) override;

	virtual bool Create(LPCTSTR pszImageName, UInt32 Width, UInt32 Height, UInt32 ChannelCount, UInt32 BitsPerChannel) override;

	virtual void Destroy() override;

	virtual bool IsCreated() const override;

	virtual LPCTSTR GetImageName() const override;

	virtual void SetImageName(LPCTSTR pszImageName) override;

	virtual const IMAGEINFO& GetImageInfo() const override;

	virtual const BYTE* GetDataPtr() const override;
	virtual BYTE* GetDataPtr() override;

private:
	// TODO 文字列クラスを作ろう (std::basic_string はちょっとね…)
	TCHAR m_szImageName[MAX_PATH];

	IMAGEINFO m_ImageInfo;

	std::vector<BYTE> m_ImageData;
};
