﻿#pragma once

#include <vector>

#include <IAudioData.h>

class CAudioData : public IAudioData
{
public:
	CAudioData();
	virtual ~CAudioData();
	CAudioData(const CAudioData& AudioData);
	CAudioData& operator=(const CAudioData& AudioData);

	virtual bool Create(LPCTSTR pszAudioName, EDataTypes DataType, UInt32 ChannelCount, UInt32 SamplesPerSec, UInt32 SampleCount) override;

	virtual void Destroy() override;

	virtual bool IsCreated() const override;

	virtual LPCTSTR GetAudioName() const override;

	virtual void SetAudioName(LPCTSTR pszAudioName) override;

	virtual const SAudioInfo& GetAudioInfo() const override;

	virtual const BYTE* GetDataPtr() const override;
	virtual BYTE* GetDataPtr() override;

	virtual bool CopyTo(IAudioData* pAudioData) const override;

private:
	FString m_AudioName;

	SAudioInfo m_AudioInfo;

	std::vector<BYTE> m_AudioData;
};
