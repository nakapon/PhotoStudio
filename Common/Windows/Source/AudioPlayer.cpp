#include <Platform.h>

#include <process.h>

#include <mmsystem.h>

#include <IAudioData.h>

#include <AudioPlayer.h>

#define WM_APP_END (WM_APP + 10)

CAudioPlayer::CAudioPlayer()
	: m_pAudioData(nullptr)
	, m_hMonitoringThread(nullptr)
	, m_MonitoringThread(0)
	, m_BufferCount(8)
	, m_BufferLengthInMillisec(100)
	, m_BufferLength(1)
	, m_BufferIndex(0)
	, m_StartSamplePos(0)
	, m_IsLoopPlay(false)
	, m_IsPlayable(false)
	, m_IsRangeEnabled(false)
	, m_LastBufferIndex(-1)
	, m_LeftVolume(0x8000)
	, m_RightVolume(0x8000)
	, m_fpCallback(nullptr)
	, m_pvUserData(nullptr)
	, m_fpFilterProc(nullptr)
	, m_pvFilterProcData(nullptr)
{
	this->m_BufferRange[0] = 1;
	this->m_BufferRange[1] = 1;

	::InitializeCriticalSection(&this->m_csSeek);
}

CAudioPlayer::~CAudioPlayer()
{
	this->DestroyThread();

	::DeleteCriticalSection(&this->m_csSeek);

	this->DestroyAudioPlayer();
}

bool CAudioPlayer::DestroyThread()
{
	if(this->m_hMonitoringThread == nullptr)
		return true;

	if(!::PostThreadMessage(this->m_MonitoringThread, WM_QUIT, 0, 0))
	{
		if(::GetLastError() == ERROR_INVALID_THREAD_ID) // The thread has already terminated
			return true;
	}

	::WaitForSingleObject(this->m_hMonitoringThread, INFINITE);

	{
		DWORD ExitCode = 0;

		::GetExitCodeThread(this->m_hMonitoringThread, &ExitCode);
		if(ExitCode == STILL_ACTIVE)
		{
			// ERROR
		}
	}

	if(::CloseHandle(this->m_hMonitoringThread))
	{
		this->m_hMonitoringThread = nullptr;
		this->m_MonitoringThread = 0;
	}
	else
	{
#if BUILD_IS_DEBUG
		::MessageBox(nullptr, TEXT("Failed to close the monitoring thread!"), TEXT("Error - CAudioPlayer"), MB_OK | MB_ICONERROR | MB_TOPMOST);
#endif
		return false;
	}

	return true;
}

void CAudioPlayer::DestroyAudioPlayer()
{
	this->m_IsPlayable = false;

	this->m_AudioPlayer.Reset();

	this->m_AudioPlayer.UnprepareHeaders();

	this->m_AudioPlayer.Close();
}

void CAudioPlayer::AttachCallback(WAVE_PLAYER_CALLBACK pfnCallback, LPVOID pvUserData)
{
	this->m_fpCallback = pfnCallback;
	this->m_pvUserData = pvUserData;
}

void CAudioPlayer::SetFilterProc(WAVE_FILTER_PROC pfnFilterProc, LPVOID pvUserData)
{
	this->m_fpFilterProc = pfnFilterProc;
	this->m_pvFilterProcData = pvUserData;
}

bool CAudioPlayer::SetAudioData(const IAudioData *pAudioData)
{
	IAudioData::AUDIOINFO AudioInfo;

	this->DestroyThread();
	this->DestroyAudioPlayer();

	if(pAudioData == nullptr || !pAudioData->IsCreated())
		return false;

	this->m_pAudioData = pAudioData;

	AudioInfo = this->m_pAudioData->GetAudioInfo();

	this->m_BufferIndex = 0;
	this->m_BufferLength = PFMath::MulDiv(this->m_BufferLengthInMillisec, AudioInfo.SamplesPerSec, 1000);

	this->m_StartSamplePos = 0;

	this->m_BufferRange[0] = 0;
	this->m_BufferRange[1] = AudioInfo.SampleCount - 1;

	this->m_IsRangeEnabled = false;

	return true;
}

bool CAudioPlayer::SetBufferCount(DWORD BufferCount)
{
	if(BufferCount < 1)
		return false;

	if(this->m_pAudioData != nullptr)
		return false;

	this->m_BufferCount = BufferCount;

	return true;
}

DWORD CAudioPlayer::GetBufferCount()
{
	return this->m_BufferCount;
}

bool CAudioPlayer::SetBufferLength(DWORD Millisec)
{
	if(Millisec < 1)
		return false;

	if(this->m_pAudioData != nullptr)
		return false;

	this->m_BufferLengthInMillisec = Millisec;

	return true;
}

DWORD CAudioPlayer::GetBufferLength()
{
	return this->m_BufferLengthInMillisec;
}

bool CAudioPlayer::Play(UInt32 DeviceId)
{
	UInt32 i;

	IAudioData::AUDIOINFO AudioInfo;

	WAVEFORMATEX WaveFormat;

	if(this->m_pAudioData == nullptr || !this->m_pAudioData->IsCreated())
		return false;

	this->DestroyThread();
	this->DestroyAudioPlayer();

	AudioInfo = this->m_pAudioData->GetAudioInfo();

	WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
	WaveFormat.nChannels = AudioInfo.ChannelCount;
	WaveFormat.nSamplesPerSec = AudioInfo.SamplesPerSec;
	WaveFormat.nAvgBytesPerSec = AudioInfo.BytesPerSec;
	WaveFormat.nBlockAlign = AudioInfo.BytesPerSample;
	WaveFormat.wBitsPerSample = AudioInfo.BitsPerChannel;
	WaveFormat.cbSize = 0;

	// open the player device
	this->m_AudioPlayer.Open(DeviceId, WaveFormat);
	this->m_AudioPlayer.AttachCallback((CWaveDeviceOut::WDO_CALLBACK)this->PreAudioPlayerCallBack, this);

	// set the parameters of player
	this->m_AudioPlayer.SetVolume(this->m_LeftVolume, this->m_RightVolume);

	// prepare headers
	this->m_AudioPlayer.PrepareHeaders(this->m_BufferCount, this->m_BufferLength * AudioInfo.BytesPerSample);

	this->m_BufferIndex = 0;

	this->m_IsPlayable = true;
	this->m_LastBufferIndex = -1;

	// callback start notification
	if(this->m_fpCallback != nullptr)
	{
		this->m_fpCallback(MSG_START, (LPVOID)0, nullptr, this->m_pvUserData);
	}

	// create monitoring thread
	this->m_hMonitoringThread = (HANDLE)_beginthreadex(nullptr, 0, CAudioPlayer::PreMonitoringThread, this, 0, (UInt32 *)&this->m_MonitoringThread);

	// set wave data to buffers
	{
		const BYTE* pbyWaveData, *pbyBuffer;
		DWORD SamplePos;

		pbyWaveData = this->m_pAudioData->GetDataPtr();

		for(i = 0; i < this->m_BufferCount; i++)
		{
			DWORD BufferLength = this->m_BufferLength;
			DWORD BufferSize;

			SamplePos = this->m_StartSamplePos + BufferLength * this->m_BufferIndex;
			pbyBuffer = &pbyWaveData[SamplePos * AudioInfo.BytesPerSample];

			if(this->m_BufferRange[1] <= (SamplePos + BufferLength))
			{
				// this buffer is last buffer
				// compute the residual sample count
				BufferLength = (DWORD)(this->m_BufferRange[1] - SamplePos);
				this->m_LastBufferIndex = i;
			}

			BufferSize = BufferLength * AudioInfo.BytesPerSample;

			this->m_AudioPlayer.SetWaveBuffer(i, (PVOID)pbyBuffer, BufferSize);

			if(this->m_fpFilterProc != nullptr)
			{
				PVOID pvBuffer;

				this->m_AudioPlayer.GetWaveBuffer(i, pvBuffer, nullptr);
				this->m_fpFilterProc(AudioInfo, pvBuffer, BufferSize, this->m_pvFilterProcData);
			}

			this->m_AudioPlayer.Write(i, BufferSize);

			this->m_BufferIndex++;

			if(0 <= this->m_LastBufferIndex)
				break;
		}
	}

	return true;
}

bool CAudioPlayer::Pause()
{
	if(!this->DestroyThread())
		return false;

	this->DestroyAudioPlayer();

	// Update start position
	this->m_StartSamplePos += this->m_BufferLength * this->m_BufferIndex;

	if(this->m_fpCallback != nullptr)
	{
		this->m_fpCallback(MSG_PAUSE, (LPVOID)0, nullptr, this->m_pvUserData);
	}

	return true;
}

bool CAudioPlayer::Stop()
{
	if(!this->DestroyThread())
		return false;

	this->DestroyAudioPlayer();

	if(this->m_fpCallback != nullptr)
	{
		this->m_fpCallback(MSG_STOP, (LPVOID)0, nullptr, this->m_pvUserData);
	}

	this->m_StartSamplePos = this->m_BufferRange[0];
	this->m_BufferIndex = 0;

	return true;
}

bool CAudioPlayer::IsPlaying()
{
	return this->m_IsPlayable;
}

bool CAudioPlayer::Seek(DWORD SampleIndex)
{
	if(this->m_pAudioData == nullptr)
		return false;

	if(this->m_BufferRange[1] < SampleIndex)
		return false;

	if(SampleIndex < this->m_BufferRange[0])
		return false;

	::EnterCriticalSection(&this->m_csSeek);

	this->m_StartSamplePos = SampleIndex;

	::LeaveCriticalSection(&this->m_csSeek);

	return true;
}

bool CAudioPlayer::SetPlaybackRange(DWORD Start, DWORD End)
{
	IAudioData::AUDIOINFO AudioInfo = { };

	DWORD MaxSampleIndex;

	if(End == 0 || End <= Start)
		return false;

	if(this->m_pAudioData == nullptr)
		return false;

	AudioInfo = this->m_pAudioData->GetAudioInfo();

	MaxSampleIndex = AudioInfo.SampleCount - 1;

	if(MaxSampleIndex < Start)
		return false;

	End = PFMath::Min(End, MaxSampleIndex);

	::EnterCriticalSection(&this->m_csSeek);

	this->m_BufferRange[0] = Start;
	this->m_BufferRange[1] = End;

	this->m_StartSamplePos = Start;
	this->m_BufferIndex = 0;

	::LeaveCriticalSection(&this->m_csSeek);

	this->m_IsRangeEnabled = true;

	return true;
}

bool CAudioPlayer::GetPlaybackRange(DWORD* pStart, DWORD* pEnd)
{
	if(pStart == nullptr && pEnd == nullptr)
		return false;

	if(this->m_pAudioData == nullptr)
		return false;

	if(pStart != nullptr)
		*pStart = this->m_BufferRange[0];

	if(pEnd != nullptr)
		*pEnd = this->m_BufferRange[1];

	return true;
}

bool CAudioPlayer::IsPlaybackRangeEnabled()
{
	return this->m_IsRangeEnabled;
}

bool CAudioPlayer::DisablePlaybackRange()
{
	IAudioData::AUDIOINFO AudioInfo = { };

	if(this->m_pAudioData == nullptr)
		return false;

	if(!this->m_IsRangeEnabled)
		return true;

	AudioInfo = this->m_pAudioData->GetAudioInfo();

	this->m_BufferRange[0] = 0;
	this->m_BufferRange[1] = AudioInfo.ChannelCount - 1;

	this->m_IsRangeEnabled = false;

	return true;
}

void CAudioPlayer::EnableLoopPlay(bool bEnable)
{
	this->m_IsLoopPlay = bEnable;
}

bool CAudioPlayer::IsLoopPlayEnabled()
{
	return this->m_IsLoopPlay;
}

void CAudioPlayer::SetVolume(WORD Left, WORD Right)
{
	this->m_LeftVolume = Left;
	this->m_RightVolume = Right;

	if(this->m_AudioPlayer.IsOpen())
	{
		this->m_AudioPlayer.SetVolume(Left, Right);
	}
}

bool CAudioPlayer::GetVolume(WORD* pLeft, WORD* pRight)
{
	if(pLeft != nullptr)
	{
		*pLeft = this->m_LeftVolume;
	}

	if(pRight != nullptr)
	{
		*pRight = this->m_RightVolume;
	}

	return true;
}

void CAudioPlayer::PreAudioPlayerCallBack(UInt32 uiMessage, DWORD_PTR dwParam1, DWORD_PTR dwParam2, PVOID pvUserData)
{
	((CAudioPlayer *)pvUserData)->AudioPlayerCallBack(uiMessage, dwParam1, dwParam2);
}

void CAudioPlayer::AudioPlayerCallBack(UInt32 uiMessage, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	switch(uiMessage)
	{
	case WOM_OPEN:
		break;

	case WOM_DONE:
		{
			PWAVEHDR pWaveHeader = (PWAVEHDR)dwParam1;

			// check status
			if(!this->m_IsPlayable)
			{
				::PostThreadMessage(this->m_MonitoringThread, WM_APP_END, 0, 0);
				return;
			}

			// check loop play
			if(0 <= this->m_LastBufferIndex && this->m_IsLoopPlay)
			{
				// loop play
				this->m_LastBufferIndex = -1;

				this->m_StartSamplePos = this->m_BufferRange[0];
				this->m_BufferIndex = 0;
			}

			// check last buffer
			if(this->m_LastBufferIndex == (Int32)pWaveHeader->dwUser)
			{
				// last buffer has been written
				this->m_IsPlayable = false;

				::PostThreadMessage(this->m_MonitoringThread, WM_APP_END, 0, 0);
				return;
			}

			// write next buffer
			if(this->m_LastBufferIndex < 0)
			{
				IAudioData::AUDIOINFO AudioInfo = { };
				const BYTE* pbyWaveData, *pbyBuffer;

				DWORD SamplePos;
				DWORD BufferLength = this->m_BufferLength;
				DWORD BufferSize;

				AudioInfo = this->m_pAudioData->GetAudioInfo();
				pbyWaveData = this->m_pAudioData->GetDataPtr();

				SamplePos = this->m_StartSamplePos + BufferLength * this->m_BufferIndex;
				pbyBuffer = &pbyWaveData[SamplePos * AudioInfo.BytesPerSample];

				if(this->m_BufferRange[1] <= (SamplePos + BufferLength))
				{
					// this buffer is last buffer
					// compute the residual sample count
					BufferLength = (DWORD)(this->m_BufferRange[1] - SamplePos);
					this->m_LastBufferIndex = (Int32)pWaveHeader->dwUser;
				}

				BufferSize = BufferLength * AudioInfo.BytesPerSample;

				this->m_AudioPlayer.SetWaveBuffer((UInt32)pWaveHeader->dwUser, (PVOID)pbyBuffer, BufferSize);

				if(this->m_fpFilterProc != nullptr)
				{
					PVOID pvBuffer;

					this->m_AudioPlayer.GetWaveBuffer((UInt32)pWaveHeader->dwUser, pvBuffer, nullptr);
					this->m_fpFilterProc(AudioInfo, pvBuffer, BufferSize, this->m_pvFilterProcData);
				}

				this->m_AudioPlayer.Write((UInt32)pWaveHeader->dwUser, BufferSize);
			}

			this->m_BufferIndex++;
		}
		break;

	case WOM_CLOSE:
		break;
	}
}

UInt32 WINAPI CAudioPlayer::PreMonitoringThread(PVOID pvParam)
{
	return ((CAudioPlayer *)pvParam)->MonitoringThread();
}

UInt32 CAudioPlayer::MonitoringThread()
{
	MSG Message = { 0 };

	for(;;)
	{
		if(::PeekMessage(&Message, (HWND)-1, 0, 0, PM_NOREMOVE))
		{
			switch(Message.message)
			{
			case WM_QUIT:
				goto LABEL_RETURN;

			case WM_APP_END:
				if(this->m_fpCallback != nullptr)
				{
					this->m_fpCallback(MSG_END, (LPVOID)(this->m_StartSamplePos + (this->m_BufferIndex * this->m_BufferLength)), nullptr, this->m_pvUserData);
				}
				goto LABEL_RETURN;

			default:
				continue;
			}
		}

		if(this->m_fpCallback != nullptr)
		{
//			this->m_fpCallback(MSG_PLAYING, (LPVOID)(this->m_StartSamplePos + (this->m_BufferIndex * this->m_BufferLength)), nullptr, this->m_pvUserData);
			Int32 BufferIndex = PFMath::Max(0, (Int32)this->m_BufferIndex - (Int32)this->m_BufferCount);
			this->m_fpCallback(MSG_PLAYING, (LPVOID)(this->m_StartSamplePos + (BufferIndex * this->m_BufferLength)), nullptr, this->m_pvUserData);
		}

		::Sleep(this->m_BufferLengthInMillisec);
	}

LABEL_RETURN:

	this->DestroyAudioPlayer();

	return 1;
}
