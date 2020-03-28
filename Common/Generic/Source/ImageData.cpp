#include <Platform.h>

#include <ImageData.h>

CImageData::CImageData()
{
	PFMemory::Zero(&this->m_ImageInfo, sizeof(this->m_ImageInfo));
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

	ImageData.CopyTo(this);

	return *this;
}

bool CImageData::Create(LPCTSTR pszImageName, const IMAGEINFO& ImageInfo)
{
	return this->Create(pszImageName, ImageInfo.Width, ImageInfo.Height, ImageInfo.ChannelCount, ImageInfo.BitsPerChannel);
}

bool CImageData::Create(LPCTSTR pszImageName, UInt32 Width, UInt32 Height, UInt32 ChannelCount, UInt32 BitsPerChannel)
{
	this->Destroy();

	UInt32 BytesPerLine = Width * ChannelCount * ((BitsPerChannel + 7) / 8);

	UInt32 ImageDataSize = Height * BytesPerLine;

	if(ImageDataSize == 0)
		return false;

	if(pszImageName != nullptr && pszImageName[0] != _T('\0'))
	{
		this->m_ImageName = pszImageName;
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
	this->m_ImageName.Clear();

	PFMemory::Zero(&this->m_ImageInfo, sizeof(this->m_ImageInfo));

	this->m_ImageData.clear();
}

bool CImageData::IsCreated() const
{
	return !this->m_ImageData.empty();
}

LPCTSTR CImageData::GetImageName() const
{
	return this->m_ImageName;
}

void CImageData::SetImageName(LPCTSTR pszImageName)
{
	if(pszImageName != nullptr && pszImageName[0] != _T('\0'))
	{
		this->m_ImageName = pszImageName;
	}
	else
	{
		this->m_ImageName.Clear();
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

bool CImageData::CopyTo(IImageData* pImageData) const
{
	IImageData::IMAGEINFO SrcInfo, DstInfo;

	const BYTE* pbySrcBits, *pbySrcLine;
	BYTE* pbyDstBits, *pbyDstLine;

	UInt32 CopyLineSize;

	if(this->m_ImageData.empty() || pImageData == nullptr)
		return false;

	pImageData->Create(this->m_ImageName, this->m_ImageInfo);

	SrcInfo = this->m_ImageInfo;
	DstInfo = pImageData->GetImageInfo();

	pbySrcBits = &this->m_ImageData[0];
	pbyDstBits = pImageData->GetDataPtr();

	CopyLineSize = PFMath::Min(SrcInfo.BytesPerLine, DstInfo.BytesPerLine);

	for(UInt32 y = 0; y < SrcInfo.Height; y++)
	{
		pbySrcLine = &pbySrcBits[SrcInfo.BytesPerLine * y];
		pbyDstLine = &pbyDstBits[DstInfo.BytesPerLine * y];

		PFMemory::Copy(pbyDstLine, pbySrcLine, CopyLineSize);
	}

	return true;
}
