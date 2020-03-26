#pragma once

#include <mmsystem.h>

class CWaveDeviceOut
{
public:
	// WAVEHDR.dwUser is buffer (header) index (when Message is WOM_DONE)
	// Messages
	//   - WOM_OPEN
	//   - WOM_CLOSE
	//   - WOM_DONE
	typedef void (CALLBACK *WDO_CALLBACK)(UInt32 Message, DWORD_PTR dwParam1, DWORD_PTR dwParam2, PVOID pvUserData);

public:
	CWaveDeviceOut();
	virtual ~CWaveDeviceOut();

	// Set or reset external callback-handler (fpCallback = nullptr is reset operation)
	//  In the case of internal callback, overwrite "WaveOutCallBack"
	void AttachCallback(WDO_CALLBACK fpCallback, PVOID pvUserData);

	// WAVE_MAPPER selects default device in DeviceId
	bool Open(UInt32 DeviceId, const WAVEFORMATEX& WaveFormat);               // Callback (internal or external)
	bool Open(UInt32 DeviceId, const WAVEFORMATEX& WaveFormat, HWND hWindow); // Window
	virtual bool Close();

	bool IsOpen();

	// Playback control
	bool Reset();

	bool Pause();
	bool Restart();

	bool BreakLoop();

	// Volume
	//   Volume range: 0xFFFF(full) - 0x0000(silence)
	//   In the case of monaural, the left-channel is the mono volume level.
	bool IsVolumeControlSupported();
	bool IsLRVolumeControlSupported();
	bool SetVolume(WORD Left, WORD Right);
	bool GetVolume(WORD* pLeft, WORD* pRight);

	bool GetDeviceId(UInt32* pDeviceId);
	bool GetPosition(MMTIME& mmTime);

	// Send a custom message
	bool SendMessage(UInt32 Message, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

	// Playing
	bool PrepareHeaders(UInt32 HeaderCount, DWORD BufferSize);
	bool UnprepareHeaders();

	bool GetWaveBuffer(UInt32 HeaderIndex, PVOID& pvBuffer, DWORD* pBufferSize);
	bool SetWaveBuffer(UInt32 HeaderIndex, PVOID pvBuffer, DWORD BufferSize);

	bool Write(UInt32 HeaderIndex, DWORD WriteSize = (DWORD)-1);

	// Querying a device information
	static UInt32 GetDeviceCount();
	static bool GetDeviceCapabilities(UInt32 DeviceId, WAVEOUTCAPS& Capabilities);

	operator HWAVEOUT();

protected:
	HWAVEOUT m_hWaveOut;

	UInt32 m_BitsPerSample;

	WAVEHDR *m_pWaveOutHeaders;
	UInt32 m_HeaderCount;

	PVOID m_pvUserData;
	WDO_CALLBACK m_fpCallback; // nullptr : internal callback, not-nullptr : external callback

protected:
	static void CALLBACK StaticWaveOutCallBack(HWAVEOUT hWaveOut, UInt32 Message, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
	virtual void WaveOutCallBack(UInt32 Message, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

private:
	CWaveDeviceOut(const CWaveDeviceOut&) = delete;
	CWaveDeviceOut& operator=(const CWaveDeviceOut&) = delete;
};
