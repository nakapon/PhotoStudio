﻿#include <Platform.h>

#include <AudioData.h>

CAudioData::CAudioData()
{
	memset(&this->m_szAudioName, 0, sizeof(this->m_szAudioName));

	memset(&this->m_AudioInfo, 0, sizeof(this->m_AudioInfo));
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

bool CAudioData::Create(LPCTSTR pszAudioName, UInt32 ChannelCount, UInt32 BitsPerChannel, UInt32 SamplesPerSec, UInt32 SampleCount)
{
	this->Destroy();

	UInt32 BytesPerSample = ChannelCount * ((BitsPerChannel + 7) >> 3);
	UInt32 BytesPerSec = SamplesPerSec * BytesPerSample;

	UInt32 AudioDataSize = SampleCount * BytesPerSample;

	if(AudioDataSize == 0)
		return false;

	if(pszAudioName != nullptr && pszAudioName[0] != _T('\0'))
	{
		PFString::Copy(this->m_szAudioName, pszAudioName);
	}

	this->m_AudioData.resize((SIZE_T)AudioDataSize);

	this->m_AudioInfo.ChannelCount = ChannelCount;
	this->m_AudioInfo.BitsPerChannel = BitsPerChannel;
	this->m_AudioInfo.SamplesPerSec = SamplesPerSec;
	this->m_AudioInfo.BytesPerSample = BytesPerSample;
	this->m_AudioInfo.BytesPerSec = BytesPerSec;
	this->m_AudioInfo.SampleCount = SampleCount;

	return true;
}

void CAudioData::Destroy()
{
	memset(&this->m_szAudioName, 0, sizeof(this->m_szAudioName));

	memset(&this->m_AudioInfo, 0, sizeof(this->m_AudioInfo));

	this->m_AudioData.clear();
}

bool CAudioData::IsCreated() const
{
	return !this->m_AudioData.empty();
}

LPCTSTR CAudioData::GetAudioName() const
{
	return this->m_szAudioName;
}

void CAudioData::SetAudioName(LPCTSTR pszAudioName)
{
	if(pszAudioName != nullptr && pszAudioName[0] != _T('\0'))
	{
		PFString::Copy(this->m_szAudioName, pszAudioName);
	}
	else
	{
		memset(&this->m_szAudioName, 0, sizeof(this->m_szAudioName));
	}
}

const IAudioData::AUDIOINFO& CAudioData::GetAudioInfo() const
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
	IAudioData::AUDIOINFO AudioInfo;

	if(this->m_AudioData.empty() || pAudioData == nullptr)
		return false;

	AudioInfo = this->m_AudioInfo;

	pAudioData->Create(this->m_szAudioName, AudioInfo.ChannelCount, AudioInfo.BitsPerChannel, AudioInfo.SamplesPerSec, AudioInfo.SampleCount);

	memcpy(pAudioData->GetDataPtr(), &this->m_AudioData[0], (SIZE_T)(AudioInfo.BytesPerSample * AudioInfo.SampleCount));

	return true;
}