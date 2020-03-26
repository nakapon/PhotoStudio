#include <Platform.h>

#include <WaveDeviceOut.h>

PRAGMA_LINK_LIBRARY("winmm.lib")

CWaveDeviceOut::CWaveDeviceOut()
	: m_hWaveOut(nullptr)
	, m_BitsPerSample(0)
	, m_pvUserData(nullptr)
	, m_pWaveOutHeaders(nullptr)
	, m_HeaderCount(0)
	, m_fpCallback(nullptr)
{
}

CWaveDeviceOut::~CWaveDeviceOut()
{
	this->UnprepareHeaders();

	this->Close();
}

void CWaveDeviceOut::AttachCallback(WDO_CALLBACK fpCallback, PVOID pvUserData)
{
	this->m_fpCallback = fpCallback;
	this->m_pvUserData = pvUserData;
}

bool CWaveDeviceOut::Open(UInt32 DeviceId, const WAVEFORMATEX& WaveFormat)
{
	MMRESULT mmResult;
	HWAVEOUT hWaveOut = nullptr;

	this->Close();

	mmResult = ::waveOutOpen(&hWaveOut, DeviceId, &WaveFormat, (DWORD_PTR)this->StaticWaveOutCallBack, (DWORD_PTR)this, CALLBACK_FUNCTION);
	if(mmResult != MMSYSERR_NOERROR)
		return false;

	this->m_hWaveOut = hWaveOut;

	this->m_BitsPerSample = WaveFormat.wBitsPerSample;

	return true;
}

bool CWaveDeviceOut::Open(UInt32 DeviceId, const WAVEFORMATEX& WaveFormat, HWND hWindow)
{
	MMRESULT mmResult;
	HWAVEOUT hWaveOut = nullptr;

	if(hWindow == nullptr)
		return false;

	this->Close();

	mmResult = ::waveOutOpen(&hWaveOut, DeviceId, &WaveFormat, (DWORD_PTR)hWindow, (DWORD_PTR)0, CALLBACK_WINDOW);
	if(mmResult != MMSYSERR_NOERROR)
		return false;

	this->m_hWaveOut = hWaveOut;

	this->m_BitsPerSample = WaveFormat.wBitsPerSample;

	return true;
}

bool CWaveDeviceOut::Close()
{
	if(this->m_hWaveOut == nullptr)
		return true;

	::waveOutClose(this->m_hWaveOut);
	this->m_hWaveOut = nullptr;

	this->m_BitsPerSample = 0;

	this->m_fpCallback = nullptr;
	this->m_pvUserData = nullptr;

	return true;
}

bool CWaveDeviceOut::IsOpen()
{
	return (this->m_hWaveOut != nullptr);
}

bool CWaveDeviceOut::Reset()
{
	if(this->m_hWaveOut == nullptr)
		return true;

	::waveOutReset(this->m_hWaveOut);

	return true;
}

bool CWaveDeviceOut::Pause()
{
	if(this->m_hWaveOut == nullptr)
		return false;

	if(::waveOutPause(this->m_hWaveOut) != MMSYSERR_NOERROR)
		return false;

	return true;
}

bool CWaveDeviceOut::Restart()
{
	if(this->m_hWaveOut == nullptr)
		return false;

	if(::waveOutRestart(this->m_hWaveOut) != MMSYSERR_NOERROR)
		return false;

	return true;
}

bool CWaveDeviceOut::BreakLoop()
{
	if(this->m_hWaveOut == nullptr)
		return false;

	if(::waveOutBreakLoop(this->m_hWaveOut) != MMSYSERR_NOERROR)
		return false;

	return true;
}

bool CWaveDeviceOut::PrepareHeaders(UInt32 HeaderCount, DWORD BufferSize)
{
	bool bReturn;

	WAVEHDR *pHeaders = nullptr;

	if(HeaderCount == 0)
		return false;

	if(this->m_hWaveOut == nullptr)
		return true;

	this->UnprepareHeaders();

	pHeaders = new WAVEHDR[HeaderCount];
	if(pHeaders == nullptr)
		return false;

	memset(pHeaders, 0, sizeof(WAVEHDR) * HeaderCount);

	for(UInt32 i = 0; i < HeaderCount; i++)
	{
		pHeaders[i].dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
		pHeaders[i].dwLoops = 1;
		pHeaders[i].dwUser = (DWORD)i; // buffer index
		pHeaders[i].dwBufferLength = BufferSize;

		pHeaders[i].lpData = (LPSTR)new BYTE[BufferSize];
		if(pHeaders[i].lpData == nullptr)
		{
			bReturn = false;
			goto _Error;
		}

		if(this->m_BitsPerSample == 8)
		{
			for(UInt32 j = 0; j < BufferSize; j++)
			{
				((PBYTE)pHeaders[i].lpData)[j] = 128;
			}
		}
		else
		{
			memset(pHeaders[i].lpData, 0, BufferSize);
		}

		if(::waveOutPrepareHeader(this->m_hWaveOut, &pHeaders[i], sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
		{
			bReturn = false;
			goto _Error;
		}
	}

	this->m_pWaveOutHeaders = pHeaders;
	this->m_HeaderCount = HeaderCount;

	return true;

_Error:

	for(UInt32 i = 0; i < HeaderCount; i++)
	{
		if(pHeaders[i].lpData != nullptr)
		{
			delete[] (PBYTE)pHeaders[i].lpData;
			pHeaders[i].lpData = nullptr;
		}
	}

	delete[] pHeaders;
	pHeaders = nullptr;

	return bReturn;
}

bool CWaveDeviceOut::UnprepareHeaders()
{
	if(this->m_hWaveOut == nullptr || this->m_pWaveOutHeaders == nullptr)
		return true;

	for(UInt32 i = 0; i < this->m_HeaderCount; i++)
	{
		if(this->m_pWaveOutHeaders[i].lpData != nullptr)
		{
			delete[] (PBYTE)this->m_pWaveOutHeaders[i].lpData;
			this->m_pWaveOutHeaders[i].lpData = nullptr;
		}
	}

	delete[] this->m_pWaveOutHeaders;
	this->m_pWaveOutHeaders = nullptr;

	this->m_HeaderCount = 0;

	return true;
}

bool CWaveDeviceOut::GetWaveBuffer(UInt32 HeaderIndex, PVOID& pvBuffer, DWORD* pBufferSize)
{
	if(this->m_HeaderCount <= HeaderIndex || pvBuffer == nullptr)
		return false;

	if(this->m_hWaveOut == nullptr)
		return false;

	pvBuffer = this->m_pWaveOutHeaders[HeaderIndex].lpData;

	if(pBufferSize != nullptr)
	{
		*pBufferSize = this->m_pWaveOutHeaders[HeaderIndex].dwBufferLength;
	}

	return true;
}

bool CWaveDeviceOut::SetWaveBuffer(UInt32 HeaderIndex, PVOID pvBuffer, DWORD BufferSize)
{
	if(this->m_HeaderCount <= HeaderIndex || pvBuffer == nullptr || BufferSize == 0)
		return false;

	if(this->m_hWaveOut == nullptr)
		return false;

	{
		DWORD dwCopySize = this->m_pWaveOutHeaders[HeaderIndex].dwBufferLength;

		dwCopySize = PFMath::Min(dwCopySize, BufferSize);

		memcpy(this->m_pWaveOutHeaders[HeaderIndex].lpData, pvBuffer, dwCopySize);
	}

	return true;
}

bool CWaveDeviceOut::Write(UInt32 HeaderIndex, DWORD WriteSize)
{
	Int32 nReturn;
	DWORD dwDefaultSize;

	if(this->m_hWaveOut == nullptr || this->m_HeaderCount <= HeaderIndex)
		return true;

	dwDefaultSize = this->m_pWaveOutHeaders[HeaderIndex].dwBufferLength;
	if(WriteSize < dwDefaultSize)
	{
		this->m_pWaveOutHeaders[HeaderIndex].dwBufferLength = WriteSize;
	}

	nReturn = ::waveOutWrite(this->m_hWaveOut, &this->m_pWaveOutHeaders[HeaderIndex], sizeof(WAVEHDR));

	if(WriteSize < dwDefaultSize)
	{
		this->m_pWaveOutHeaders[HeaderIndex].dwBufferLength = dwDefaultSize;
	}

	if(nReturn != MMSYSERR_NOERROR)
		return false;

	return true;
}

void CALLBACK CWaveDeviceOut::StaticWaveOutCallBack(HWAVEOUT hWaveOut, UInt32 Message, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	CWaveDeviceOut *pThis = (CWaveDeviceOut *)dwInstance;

	if(pThis->m_fpCallback != nullptr)
	{
		pThis->m_fpCallback(Message, dwParam1, dwParam2, pThis->m_pvUserData);
	}
	else
	{
		pThis->WaveOutCallBack(Message, dwParam1, dwParam2);
	}
}

void CWaveDeviceOut::WaveOutCallBack(UInt32 Message, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	// overwrite to implement
}

bool CWaveDeviceOut::SetVolume(WORD Left, WORD Right)
{
	if(this->m_hWaveOut == nullptr)
		return false;

	if(::waveOutSetVolume(this->m_hWaveOut, MAKELONG(Left, Right)) != MMSYSERR_NOERROR)
		return false;

	return true;
}

bool CWaveDeviceOut::GetVolume(WORD *pLeft, WORD *pRight)
{
	DWORD dwVolume = 0;

	if(this->m_hWaveOut == nullptr)
		return false;

	if(::waveOutGetVolume(this->m_hWaveOut, &dwVolume) != MMSYSERR_NOERROR)
		return false;

	if(pLeft != nullptr)
	{
		*pLeft = LOWORD(dwVolume);
	}

	if(pRight != nullptr)
	{
		*pRight = HIWORD(dwVolume);
	}

	return true;
}

bool CWaveDeviceOut::GetDeviceId(UInt32* pDeviceId)
{
	if(pDeviceId == nullptr)
		return false;

	*pDeviceId = 0;

	if(this->m_hWaveOut == nullptr)
		return false;

	if(::waveOutGetID(this->m_hWaveOut, pDeviceId) != MMSYSERR_NOERROR)
		return false;

	return true;
}

bool CWaveDeviceOut::GetPosition(MMTIME& mmTime)
{
	if(this->m_hWaveOut == nullptr)
		return false;

	if(::waveOutGetPosition(this->m_hWaveOut, &mmTime, sizeof(mmTime)) != MMSYSERR_NOERROR)
		return false;

	return true;
}

bool CWaveDeviceOut::SendMessage(UInt32 Message, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	if(this->m_hWaveOut == nullptr)
		return false;

	return ::waveOutMessage(this->m_hWaveOut, Message, dwParam1, dwParam2) == MMSYSERR_NOERROR;
}

CWaveDeviceOut::operator HWAVEOUT()
{
	return this->m_hWaveOut;
}

UInt32 CWaveDeviceOut::GetDeviceCount()
{
	return ::waveOutGetNumDevs();
}

bool CWaveDeviceOut::GetDeviceCapabilities(UInt32 DeviceId, WAVEOUTCAPS& Capabilities)
{
	if(::waveOutGetDevCaps(DeviceId, &Capabilities, sizeof(Capabilities)) != MMSYSERR_NOERROR)
		return false;

	return true;
}
