#pragma once

class IAudioData
{
public:
	typedef struct
	{
		UInt32 ChannelCount;	/* チャンネル数 */
		UInt32 BitsPerChannel;	/* 1チャンネル当たりのビット数 */
		UInt32 SamplesPerSec;	/* サンプリング周波数 Hz */
		UInt32 BytesPerSample;	/* 1サンプル当たりのバイト数 */
		UInt32 BytesPerSec;		/* 1秒あたりのバイト数 */
		UInt32 SampleCount;		/* トータルのサンプル数 */
	} AUDIOINFO;

public:
	virtual ~IAudioData(){}

	virtual bool Create(LPCTSTR pszAudioName, UInt32 ChannelCount, UInt32 BitsPerChannel, UInt32 SamplesPerSec, UInt32 SampleCount) = 0;

	virtual void Destroy() = 0;

	virtual bool IsCreated() const = 0;

	virtual LPCTSTR GetAudioName() const = 0;

	virtual void SetAudioName(LPCTSTR pszAudioName) = 0;

	virtual const AUDIOINFO& GetAudioInfo() const = 0;

	virtual const BYTE* GetDataPtr() const = 0;
	virtual BYTE* GetDataPtr() = 0;

	virtual bool CopyTo(IAudioData* pAudioData) const = 0;
};
