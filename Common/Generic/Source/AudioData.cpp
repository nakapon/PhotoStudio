#include <Platform.h>

#include <AudioData.h>

CAudioData::CAudioData()
{
	PFMemory::Zero(&this->m_AudioInfo, sizeof(this->m_AudioInfo));
}

CAudioData::~CAudioData()
{
	this->Destroy();
}

CAudioData::CAudioData(const CAudioData& AudioData)
{
	*this = AudioData;
}

CAudioData& CAudioData::operator=(const CAudioData& AudioData)
{
	this->Destroy();

	AudioData.CopyTo(this);

	return *this;
}

bool CAudioData::Create(LPCTSTR pszAudioName, EDataTypes DataType, UInt32 ChannelCount, UInt32 SamplesPerSec, UInt32 SampleCount)
{
	this->Destroy();

	UInt32 BytesPerSample = ChannelCount * AudioData::GetBytesPerChannel(DataType);
	UInt32 BytesPerSec = SamplesPerSec * BytesPerSample;

	UInt32 AudioDataSize = SampleCount * BytesPerSample;

	if(DataType == EDataTypes::Unknown
	|| AudioDataSize == 0)
		return false;

	if(pszAudioName != nullptr && pszAudioName[0] != _T('\0'))
	{
		this->m_AudioName = pszAudioName;
	}

	this->m_AudioData.resize((SIZE_T)AudioDataSize);

	this->m_AudioInfo.DataType = DataType;
	this->m_AudioInfo.ChannelCount = ChannelCount;
	this->m_AudioInfo.SamplesPerSec = SamplesPerSec;
	this->m_AudioInfo.BytesPerSample = BytesPerSample;
	this->m_AudioInfo.BytesPerSec = BytesPerSec;
	this->m_AudioInfo.SampleCount = SampleCount;

	if(this->m_AudioInfo.DataType == EDataTypes::UInt8)
	{
		PFMemory::Fill(&this->m_AudioData[0], 128, this->m_AudioData.size());
	}

	return true;
}

void CAudioData::Destroy()
{
	this->m_AudioName.Clear();

	PFMemory::Zero(&this->m_AudioInfo, sizeof(this->m_AudioInfo));

	this->m_AudioData.clear();
}

bool CAudioData::IsCreated() const
{
	return !this->m_AudioData.empty();
}

LPCTSTR CAudioData::GetAudioName() const
{
	return this->m_AudioName;
}

void CAudioData::SetAudioName(LPCTSTR pszAudioName)
{
	if(pszAudioName != nullptr && pszAudioName[0] != _T('\0'))
	{
		this->m_AudioName = pszAudioName;
	}
	else
	{
		this->m_AudioName.Clear();
	}
}

const IAudioData::SAudioInfo& CAudioData::GetAudioInfo() const
{
	return this->m_AudioInfo;
}

const BYTE* CAudioData::GetDataPtr() const
{
	if(this->m_AudioData.empty())
		return nullptr;

	return &this->m_AudioData[0];
}

BYTE* CAudioData::GetDataPtr()
{
	if(this->m_AudioData.empty())
		return nullptr;

	return &this->m_AudioData[0];
}

bool CAudioData::CopyTo(IAudioData* pAudioData) const
{
	IAudioData::SAudioInfo AudioInfo;

	if(this->m_AudioData.empty() || pAudioData == nullptr)
		return false;

	AudioInfo = this->m_AudioInfo;

	pAudioData->Create(this->m_AudioName, AudioInfo.DataType, AudioInfo.ChannelCount, AudioInfo.SamplesPerSec, AudioInfo.SampleCount);

	PFMemory::Copy(pAudioData->GetDataPtr(), &this->m_AudioData[0], (SIZE_T)(AudioInfo.BytesPerSample * AudioInfo.SampleCount));

	return true;
}
