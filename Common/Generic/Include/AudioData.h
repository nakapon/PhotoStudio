#pragma once

#include <vector>

#include <IAudioData.h>

class CAudioData : public IAudioData
{
public:
	CAudioData();
	virtual ~CAudioData();
	CAudioData(const CAudioData& AudioData);
	CAudioData& operator=(const CAudioData& AudioData);

	virtual bool Create(LPCTSTR pszAudioName, UInt32 ChannelCount, UInt32 BitsPerChannel, UInt32 SamplesPerSec, UInt32 SampleCount) override;

	virtual void Destroy() override;

	virtual bool IsCreated() const override;

	virtual LPCTSTR GetAudioName() const override;

	virtual void SetAudioName(LPCTSTR pszAudioName) override;

	virtual const AUDIOINFO& GetAudioInfo() const override;

	virtual const BYTE* GetDataPtr() const override;
	virtual BYTE* GetDataPtr() override;

	virtual bool CopyTo(IAudioData* pAudioData) const override;

private:
	// TODO 文字列クラスを作ろう (std::basic_string はちょっとね…)
	TCHAR m_szAudioName[MAX_PATH];

	AUDIOINFO m_AudioInfo;

	std::vector<BYTE> m_AudioData;
};
