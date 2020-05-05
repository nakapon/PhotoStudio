#pragma once

class IAudioData
{
public:
	enum class EDataTypes
	{
		Unknown = 0,
		UInt8,
		Int16,
		Float32,
	};

	struct SAudioInfo
	{
		EDataTypes DataType;	/* データ型 */
		UInt32 ChannelCount;	/* チャンネル数 */
		UInt32 SamplesPerSec;	/* サンプリング周波数 Hz */
		UInt32 BytesPerSample;	/* 1サンプル当たりのバイト数 */
		UInt32 BytesPerSec;		/* 1秒あたりのバイト数 */
		UInt32 SampleCount;		/* トータルのサンプル数 */
	};

public:
	virtual ~IAudioData(){}

	virtual bool Create(LPCTSTR pszAudioName, EDataTypes DataType, UInt32 ChannelCount, UInt32 SamplesPerSec, UInt32 SampleCount) = 0;

	virtual void Destroy() = 0;

	virtual bool IsCreated() const = 0;

	virtual LPCTSTR GetAudioName() const = 0;

	virtual void SetAudioName(LPCTSTR pszAudioName) = 0;

	virtual const SAudioInfo& GetAudioInfo() const = 0;

	virtual const BYTE* GetDataPtr() const = 0;
	virtual BYTE* GetDataPtr() = 0;

	virtual bool CopyTo(IAudioData* pAudioData) const = 0;
};

namespace AudioData
{
	inline UInt32 GetBytesPerChannel(IAudioData::EDataTypes DataType)
	{
		UInt32 BytesPerChannel = 0;

		switch(DataType)
		{
		case IAudioData::EDataTypes::UInt8: BytesPerChannel = 1; break;
		case IAudioData::EDataTypes::Int16: BytesPerChannel = 2; break;
		case IAudioData::EDataTypes::Float32: BytesPerChannel = 4; break;
		}

		return BytesPerChannel;
	}

	inline UInt32 GetBytesPerChannel(const IAudioData::SAudioInfo& AudioInfo)
	{
		return AudioData::GetBytesPerChannel(AudioInfo.DataType);
	}

	inline UInt32 GetBitsPerChannel(IAudioData::EDataTypes DataType)
	{
		return 8 * AudioData::GetBytesPerChannel(DataType);
	}

	inline UInt32 GetBitsPerChannel(const IAudioData::SAudioInfo& AudioInfo)
	{
		return 8 * AudioData::GetBytesPerChannel(AudioInfo.DataType);
	}
}
