#pragma once

#include <WaveDeviceOut.h>

class CAudioPlayer
{
public:
	typedef enum
	{
		MSG_START,	// pvParam1 : nullptr
					// pvParam2 : nullptr
		MSG_PLAYING,// pvParam1 : playback position (DWORD)
					// pvParam2 : nullptr
		MSG_END,	// pvParam1 : start position (DWORD)
					// pvParam2 : nullptr
		MSG_STOP,	// pvParam1 : nullptr
					// pvParam2 : nullptr
		MSG_PAUSE,	// pvParam1 : nullptr
					// pvParam2 : nullptr
	} EMessages;

	typedef Int32 (CALLBACK *WAVE_PLAYER_CALLBACK)(EMessages Message, LPVOID pvParam1, LPVOID pvParam2, LPVOID pvUserData);

	typedef Int32 (CALLBACK *WAVE_FILTER_PROC)(const IAudioData::SAudioInfo& AudioInfo, PVOID pvBuffer, DWORD dwBufferLength, PVOID pvUserData);

public:
	CAudioPlayer();
	~CAudioPlayer();

	void AttachCallback(WAVE_PLAYER_CALLBACK pfnCallback, LPVOID pvUserData);

	void SetFilterProc(WAVE_FILTER_PROC pfnFilterProc, LPVOID pvUserData);

	bool SetAudioData(const IAudioData *pAudioData);

	bool SetBufferCount(DWORD BufferCount);
	DWORD GetBufferCount();

	bool SetBufferLength(DWORD Millisec);
	DWORD GetBufferLength();

	// Control
	bool Play(UInt32 DeviceId = (UInt32)-1);
	bool Pause();
	bool Stop();
	bool Seek(DWORD SampleIndex);
	bool IsPlaying();

	// Playback range
	//  set ((QWORD)-1, 0) => Select whole range
	bool SetPlaybackRange(DWORD Start, DWORD End);
	bool GetPlaybackRange(DWORD* pStart, DWORD* pEnd);
	bool IsPlaybackRangeEnabled();
	bool DisablePlaybackRange();

	// Loop play
	void EnableLoopPlay(bool bEnable);
	bool IsLoopPlayEnabled();

	// Volume
	//   Volume range: 0xFFFF(full) - 0x0000(silence)
	//   In the case of monaural, the left-channel is the mono volume level.
	void SetVolume(WORD Left, WORD Right);
	bool GetVolume(WORD* pLeft, WORD* pRight);

protected:

	CWaveDeviceOut m_AudioPlayer;

	// Audio
	const IAudioData *m_pAudioData;

	DWORD m_BufferCount;
	DWORD m_BufferLengthInMillisec;

	DWORD m_BufferLength; // m_uiCallbackInterval[ms] 当りのサンプル数
	DWORD m_BufferIndex;

	DWORD m_StartSamplePos; // 再生時の開始サンプル位置
	DWORD m_BufferRange[2];

	CRITICAL_SECTION m_csSeek;

	// Global variables
	bool m_IsPlayable;
	bool m_IsLoopPlay;
	bool m_IsRangeEnabled;
	Int32 m_LastBufferIndex;

	WORD m_LeftVolume;
	WORD m_RightVolume;

	// Monitoring thread
	HANDLE m_hMonitoringThread;
	DWORD m_MonitoringThread;

	// Callbacks
	WAVE_PLAYER_CALLBACK m_fpCallback;
	PVOID m_pvUserData;

	// Filter Proc
	WAVE_FILTER_PROC m_fpFilterProc;
	PVOID m_pvFilterProcData;

protected:
	static void CALLBACK PreAudioPlayerCallBack(UInt32 uiMessage, DWORD_PTR dwParam1, DWORD_PTR dwParam2, PVOID pvUserData);
	void AudioPlayerCallBack(UInt32 uiMessage, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

	static UInt32 WINAPI PreMonitoringThread(PVOID pvParam);
	UInt32 MonitoringThread();

	bool DestroyThread();
	void DestroyAudioPlayer();

private:
	CAudioPlayer(const CAudioPlayer&) = delete;
	CAudioPlayer& operator=(const CAudioPlayer&) = delete;
};
