#pragma once

#include <IAudioData.h>

class WaveAudioReader
{
public:

	static bool ReadAudio(LPCTSTR pszFilePath, IAudioData* pAudioData);

private:
	WaveAudioReader() = delete;
	~WaveAudioReader() = delete;
	WaveAudioReader(const WaveAudioReader&) = delete;
	WaveAudioReader& operator=(const WaveAudioReader&) = delete;
};
