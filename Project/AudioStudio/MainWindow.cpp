#include <Platform.h>

#include <AudioData.h>

#include <WaveAudioReader.h>
#include <WaveAudioWriter.h>

#include <AudioPlayer.h>

#include <MenuLibrary.h>

#include "Session.h"

#include "NewDlg.h"

#include "resource.h"

#define FILE_MENU_POS	0
#define AUDIO_MENU_POS	1

extern HINSTANCE g_hInstance;

static CAudioData gs_AudioData;
static CAudioData gs_ProcAudio;

static CAudioPlayer gs_AudioPlayer;

static void UpdateWaveform(HWND hWindow);
static void LoadAudio(HWND hWindow, LPCTSTR pszFilePath, bool bShowErrorMsg);
static void SaveAudio(HWND hWindow, LPCTSTR pszFilePath, const IAudioData* pAudioData);

static void UpdateAppTitle(HWND hWindow);

static void OnFileMenuPopup(HMENU hMenu);
static void OnAudioMenuPopup(HMENU hMenu);

static void StartStopAudioPlay();

static Int32 CALLBACK WavePlayerCallback(CAudioPlayer::EMessages Message, LPVOID pvParam1, LPVOID pvParam2, LPVOID pvUserData);

// ���C���E�B���h�E�쐬���̏���
INT OnCreate(HWND hWindow, CREATESTRUCT* pCreateStruct)
{
	// �E�B���h�E�ɃA�C�R����ݒ�
	{
		HICON hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_SYMBOL));
		SendMessage(hWindow, WM_SETICON, ICON_BIG, (WPARAM)hIcon);
		SendMessage(hWindow, WM_SETICON, ICON_SMALL, (WPARAM)hIcon);
	}

	// �t�@�C���h���b�v����
	DragAcceptFiles(hWindow, TRUE);
	
	// �O��̃I�[�f�B�I�f�[�^��ǂݍ���
	{
		TCHAR szFilePath[MAX_PATH] = { 0 };

		if(Session::RestoreSession(szFilePath, PF_ARRAY_LENGTH(szFilePath)))
		{
			LoadAudio(hWindow, szFilePath, false);
		}
	}

	gs_AudioPlayer.AttachCallback(WavePlayerCallback, hWindow);

	UpdateAppTitle(hWindow);

	return 0;
}

// ���C���E�B���h�E�́u����v�{�^���������̏���
INT OnClose(HWND hWindow)
{
	DestroyWindow(hWindow);

	Session::StoreSession(gs_AudioData.GetAudioName());

	return 0;
}

// ���C���E�B���h�E�̔j�����̏���
INT OnDestroy(HWND hWindow)
{
	// ���b�Z�[�W���[�v�𔲂���ׂ� WM_QUIT ���b�Z�[�W�𓊂���
	PostQuitMessage(0);

	return 0;
}

// ���C���E�B���h�E�̍ĕ`�掞�̏���
INT OnPaint(HWND hWindow)
{
	HDC hDC;
	PAINTSTRUCT PaintStruct = { 0 };

	RECT rcClient;
	SIZE ClientSize;

	GetClientRect(hWindow, &rcClient);

	ClientSize.cx = rcClient.right - rcClient.left;
	ClientSize.cy = rcClient.bottom - rcClient.top;

	hDC = BeginPaint(hWindow, &PaintStruct);

	// ��U��ʂ����h�肷��
	{
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(BLACK_BRUSH));

		Rectangle(hDC, 0, 0, ClientSize.cx, ClientSize.cy);

		SelectObject(hDC, hOldBrush);
	}

	// xxx TODO draw waveform

	EndPaint(hWindow, &PaintStruct);

	return 0;
}

// ���C���E�B���h�E�̃R�}���h�i���j���[�����C�{�^�������Ȃǁj����
INT OnCommand(HWND hWindow, WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case ID_FILE_NEW:
		if(NewDlg::DoModal(g_hInstance, hWindow, &gs_AudioData))
		{
			gs_ProcAudio.Destroy();

			UpdateWaveform(hWindow);

			UpdateAppTitle(hWindow);
		}
		break;

	case ID_FILE_OPEN:
		{
			OPENFILENAME OpenFileName = { 0 };

			TCHAR szFilePath[MAX_PATH] = { 0 };

			// �u�t�@�C���I���_�C�A���O�v��\��
			OpenFileName.lStructSize     = sizeof(OPENFILENAME);
			OpenFileName.hwndOwner       = hWindow;
			OpenFileName.lpstrFilter     = TEXT("WAVE (*.wav)\0*.wav\0\0");
//			OpenFileName.lpstrFilter     = TEXT("All Files(*.*)\0*.*\0\0");
			OpenFileName.nFilterIndex    = 1;
			OpenFileName.lpstrFile       = szFilePath;
			OpenFileName.nMaxFile        = MAX_PATH;
			OpenFileName.Flags           = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			OpenFileName.lpstrDefExt     = TEXT("");
			OpenFileName.nMaxFileTitle   = MAX_PATH;
			OpenFileName.lpstrFileTitle  = nullptr;
			OpenFileName.lpstrTitle      = nullptr;

			if(GetOpenFileName(&OpenFileName))
			{
				LoadAudio(hWindow, szFilePath, true);

				UpdateAppTitle(hWindow);
			}
		}
		break;

	case ID_FILE_CLOSE:
		gs_AudioData.Destroy();
		gs_ProcAudio.Destroy();
		UpdateWaveform(hWindow);
		UpdateAppTitle(hWindow);
		break;

	case ID_FILE_SAVE:
		{
			OPENFILENAME SaveFileName = { 0 };

			TCHAR szFilePath[MAX_PATH] = { 0 };

			// �u�t�@�C���I���_�C�A���O�v��\��
			SaveFileName.lStructSize     = sizeof(OPENFILENAME);
			SaveFileName.hwndOwner       = hWindow;
			SaveFileName.lpstrFilter     = TEXT("WAVE (*.wav)\0*.wav\0\0");
//			SaveFileName.lpstrFilter     = TEXT("All Files(*.*)\0*.*\0\0");
			SaveFileName.nFilterIndex    = 1;
			SaveFileName.lpstrFile       = szFilePath;
			SaveFileName.nMaxFile        = MAX_PATH;
			SaveFileName.Flags           = OFN_OVERWRITEPROMPT;
			SaveFileName.lpstrDefExt     = TEXT("");
			SaveFileName.nMaxFileTitle   = MAX_PATH;
			SaveFileName.lpstrFileTitle  = nullptr;
			SaveFileName.lpstrTitle      = nullptr;

			if(GetSaveFileName(&SaveFileName))
			{
				IAudioData* pAudioData = gs_ProcAudio.IsCreated() ? &gs_ProcAudio : &gs_AudioData;

				SaveAudio(hWindow, szFilePath, pAudioData);
			}
		}
		break;

	case ID_FILE_EXIT:
		PostMessage(hWindow, WM_CLOSE, 0, 0);
		break;

	case ID_AUDIO_PLAY:
		StartStopAudioPlay();
		break;

	case ID_AUDIO_LOOP:
		gs_AudioPlayer.EnableLoopPlay(!gs_AudioPlayer.IsLoopPlayEnabled());
		break;
	}

	return 0;
}

void OnFileMenuPopup(HMENU hMenu)
{
	MenuLibrary::Enable(hMenu, ID_FILE_CLOSE, false, gs_AudioData.IsCreated());
	MenuLibrary::Enable(hMenu, ID_FILE_SAVE, false, gs_AudioData.IsCreated());
}

void OnAudioMenuPopup(HMENU hMenu)
{
	MenuLibrary::Enable(hMenu, ID_AUDIO_PLAY, false, gs_AudioData.IsCreated());
	MenuLibrary::SetString(hMenu, ID_AUDIO_PLAY, false, gs_AudioPlayer.IsPlaying() ? TEXT("Stop\tSpace") : TEXT("Play\tSpace"));

	MenuLibrary::Check(hMenu, ID_AUDIO_LOOP, false, gs_AudioPlayer.IsLoopPlayEnabled());
}

// ���C���E�B���h�E�̒ʒm����
INT OnNotify(HWND hWindow, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

// ���C���E�B���h�E�̃X�N���[���o�[�̏���
INT OnScrollEvent(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

// ���C���E�B���h�E�̃}�E�X�C�x���g����
INT OnMouseEvent(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

// ���C���E�B���h�E�̃L�[����
INT OnKeyEvent(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if(Message == WM_KEYDOWN)
	{
		if(wParam == VK_SPACE)
		{
			// SHIFT�L�[���� => �Đ��E��~����
			StartStopAudioPlay();
			UpdateAppTitle(hWindow);
		}
	}

	return 0;
}

// ���j���[��W�J����Ƃ��̏���
INT OnInitMenuPopup(HWND hWindow, WPARAM wParam, LPARAM lParam)
{
	HMENU hTargetMenu = (HMENU)wParam;

	UInt32 Position = LOWORD(lParam);
	BOOL bWindowMenu = (BOOL)HIWORD(lParam);

	HMENU hParentMenu = ::GetMenu(hWindow);

	HMENU hMenu = ::GetSubMenu(hParentMenu, FILE_MENU_POS);
	if(hMenu == hTargetMenu)
	{
		OnFileMenuPopup(hMenu);
	}

	hMenu = ::GetSubMenu(hParentMenu, AUDIO_MENU_POS);
	if(hMenu == hTargetMenu)
	{
		OnAudioMenuPopup(hMenu);
	}

	return 0;
}

// �t�@�C�����E�B���h�E�Ƀh���b�v�����Ƃ��̏���
INT OnDropFiles(HWND hWindow, WPARAM wParam, LPARAM lParam)
{
	HDROP hDrop = (HDROP)wParam;

	TCHAR szFilePath[MAX_PATH] = { 0 };

	UInt32 ItemCount = DragQueryFile(hDrop, (UInt32)-1, nullptr, 0);

	DragQueryFile(hDrop, 0, szFilePath, PF_ARRAY_LENGTH(szFilePath));

	LoadAudio(hWindow, szFilePath, true);

	UpdateAppTitle(hWindow);

	return 0;
}

static void UpdateWaveform(HWND hWindow)
{
	RedrawWindow(hWindow, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASENOW | RDW_UPDATENOW);
}

static void LoadAudio(HWND hWindow, LPCTSTR pszFilePath, bool bShowErrorMsg)
{
	// �������ʂ�j��
	gs_ProcAudio.Destroy();

	if(!WaveAudioReader::ReadAudio(pszFilePath, &gs_AudioData))
	{
		// �ǂݍ��ݎ��s
		if(bShowErrorMsg)
		{
			MessageBox(hWindow, TEXT("Failed to load the specified file."), TEXT("Error"), MB_ICONERROR | MB_OK | MB_TOPMOST);
		}
	}
	else
	{
		// �ǂݍ��ݐ��� - ���C���E�B���h�E���ĕ`��
		UpdateWaveform(hWindow);
	}
}

static void SaveAudio(HWND hWindow, LPCTSTR pszFilePath, const IAudioData* pAudioData)
{
	WaveAudioWriter::WriteAudio(pszFilePath, pAudioData);
}

static void UpdateAppTitle(HWND hWindow)
{
	TCHAR szTitle[256] = { 0 };

	PFString::Copy(szTitle, TEXT("Audio Studio"));

	if(gs_AudioData.IsCreated())
	{
		PFString::Append(szTitle, TEXT(" - "));

		// �I�[�f�B�I�� (�t�@�C���p�X�̏ꍇ�̓t�@�C��������������\���j
		{
			LPCTSTR pszAudioName;

			pszAudioName = PFString::Strrchr(gs_AudioData.GetAudioName(), PF_DIR_SEP_CHAR);
			if(pszAudioName != nullptr)
			{
				pszAudioName++;
			}
			else
			{
				pszAudioName =gs_AudioData.GetAudioName();
			}

			PFString::Append(szTitle, pszAudioName);
		}

		if(gs_AudioPlayer.IsPlaying())
		{
			PFString::Append(szTitle, TEXT(" (Playing)"));
		}

		// �I�[�f�B�I���
		{
			TCHAR szInfo[128] = { 0 };

			IAudioData::SAudioInfo AudioInfo = gs_AudioData.GetAudioInfo();

			_stprintf_s(szInfo, TEXT(" [%dch %dbit %dHz %dsec]"), AudioInfo.ChannelCount, AudioData::GetBitsPerChannel(AudioInfo.DataType), AudioInfo.SamplesPerSec, AudioInfo.SampleCount / AudioInfo.SamplesPerSec);
			PFString::Append(szTitle, szInfo);
		}
	}

	SetWindowText(hWindow, szTitle);
}

static void StartStopAudioPlay()
{
	if(!gs_AudioData.IsCreated())
		return;

	if(gs_AudioPlayer.IsPlaying())
	{
		gs_AudioPlayer.Stop();
	}
	else
	{
		IAudioData* pAudioData = gs_ProcAudio.IsCreated() ? &gs_ProcAudio : &gs_AudioData;

		gs_AudioPlayer.SetAudioData(pAudioData);
		gs_AudioPlayer.Play();
	}
}

static Int32 CALLBACK WavePlayerCallback(CAudioPlayer::EMessages Message, LPVOID pvParam1, LPVOID pvParam2, LPVOID pvUserData)
{
	HWND hWindow = (HWND)pvUserData;

	switch(Message)
	{
	case CAudioPlayer::EMessages::MSG_START:
		UpdateAppTitle(hWindow);
		break;

	case CAudioPlayer::EMessages::MSG_PLAYING:
		break;

	case CAudioPlayer::EMessages::MSG_END:
		UpdateAppTitle(hWindow);
		break;

	case CAudioPlayer::EMessages::MSG_STOP:
		UpdateAppTitle(hWindow);
		break;

	case CAudioPlayer::EMessages::MSG_PAUSE:
		break;
	}

	return 0;
}
