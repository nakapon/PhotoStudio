#pragma once

#include <IAudioData.h>

class WaveAudioWriter
{
public:

	static bool WriteAudio(LPCTSTR pszFilePath, const IAudioData* pAudioData);

private:
	WaveAudioWriter() = delete;
	~WaveAudioWriter() = delete;
	WaveAudioWriter(const WaveAudioWriter&) = delete;
	WaveAudioWriter& operator=(const WaveAudioWriter&) = delete;
};
