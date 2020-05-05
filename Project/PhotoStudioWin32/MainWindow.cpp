#include <Platform.h>

#include <ImageData.h>
#include <ImageReader.h>
#include <ImageWriter.h>

#include "ImageRenderer.h"

#include <ImageProc.h>

#include <MenuLibrary.h>

#include "Session.h"

#include "NewDlg.h"

#include "resource.h"

#define FILE_MENU_POS	0
#define IMAGE_MENU_POS	1

extern HINSTANCE g_hInstance;

static CImageData gs_ImageData; /* �ǂݍ��񂾉摜�f�[�^ */
static CImageData gs_ProcImage; /* �������ʉ摜�f�[�^ */

static void UpdateImage(HWND hWindow);
static void LoadImage(HWND hWindow, LPCTSTR pszFilePath, bool bShowErrorMsg);

static void UpdateAppTitle(HWND hWindow);

static void OnFileMenuPopup(HMENU hMenu);
static void OnImageMenuPopup(HMENU hMenu);

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

	// �O��̉摜�f�[�^��ǂݍ���
	{
		TCHAR szFilePath[MAX_PATH] = { 0 };

		if(Session::RestoreSession(szFilePath, PF_ARRAY_LENGTH(szFilePath)))
		{
			LoadImage(hWindow, szFilePath, false);
		}
	}

	UpdateAppTitle(hWindow);

	return 0;
}

// ���C���E�B���h�E�́u����v�{�^���������̏���
INT OnClose(HWND hWindow)
{
	DestroyWindow(hWindow);

	Session::StoreSession(gs_ImageData.GetImageName());

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

	if(gs_ImageData.IsCreated() || gs_ProcImage.IsCreated())
	{
		// �������ʂ�D�悵�ĕ\��
		IImageData* pImageData = gs_ProcImage.IsCreated() ? &gs_ProcImage : &gs_ImageData;

		ImageRenderer::Render(hDC, ClientSize, pImageData);
	}

	EndPaint(hWindow, &PaintStruct);

	return 0;
}

// ���C���E�B���h�E�̃R�}���h�i���j���[�����C�{�^�������Ȃǁj����
INT OnCommand(HWND hWindow, WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case ID_FILE_NEW:
		// MENU �uFile�v���uNew�v�̏���
		if(NewDlg::DoModal(g_hInstance, hWindow, &gs_ImageData))
		{
			gs_ProcImage.Destroy();

			UpdateImage(hWindow);

			UpdateAppTitle(hWindow);
		}
		break;

	case ID_FILE_OPEN:
		// MENU �uFile�v���uOpen�v�̏���
		{
			OPENFILENAME OpenFileName = { 0 };

			TCHAR szFilePath[MAX_PATH] = { 0 };

			// �u�t�@�C���I���_�C�A���O�v��\��
			OpenFileName.lStructSize     = sizeof(OPENFILENAME);
			OpenFileName.hwndOwner       = hWindow;
			OpenFileName.lpstrFilter     = TEXT("All Files(*.*)\0*.*\0\0");
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
				LoadImage(hWindow, szFilePath, true);

				UpdateAppTitle(hWindow);
			}
		}
		break;

	case ID_FILE_CLOSE:
		// MENU �uFile�v���uClose�v�̏���
		// �摜�f�[�^��j�����ĉ�ʂ��ĕ`��
		gs_ImageData.Destroy();
		gs_ProcImage.Destroy();
		UpdateImage(hWindow);
		UpdateAppTitle(hWindow);
		break;

	case ID_FILE_SAVE:
		// MENU �uFile�v���uSave�v�̏���
		if(gs_ImageData.IsCreated() || gs_ProcImage.IsCreated())
		{
			OPENFILENAME SaveFileName = { 0 };

			TCHAR szFilePath[MAX_PATH] = { 0 };

			// �u�t�@�C���I���_�C�A���O�v��\��
			SaveFileName.lStructSize     = sizeof(OPENFILENAME);
			SaveFileName.hwndOwner       = hWindow;
			SaveFileName.lpstrFilter     = TEXT("All Files(*.*)\0*.*\0\0");
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
				IImageData* pImageData = gs_ProcImage.IsCreated() ? &gs_ProcImage : &gs_ImageData;

				ImageWriter::WriteImage(szFilePath, pImageData);
			}
		}
		break;

	case ID_FILE_EXIT:
		PostMessage(hWindow, WM_CLOSE, 0, 0);
		break;

	case ID_IMAGE_MONO:
		if(gs_ImageData.IsCreated())
		{
			ImageProc::GrayScale(&gs_ProcImage, &gs_ImageData);
			UpdateImage(hWindow);
		}
		break;
	}

	return 0;
}

void OnFileMenuPopup(HMENU hMenu)
{
	MenuLibrary::Enable(hMenu, ID_FILE_CLOSE, false, gs_ImageData.IsCreated());
	MenuLibrary::Enable(hMenu, ID_FILE_SAVE, false, gs_ImageData.IsCreated());
}

void OnImageMenuPopup(HMENU hMenu)
{
	MenuLibrary::Enable(hMenu, ID_IMAGE_MONO, false, gs_ImageData.IsCreated());
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

	hMenu = ::GetSubMenu(hParentMenu, IMAGE_MENU_POS);
	if(hMenu == hTargetMenu)
	{
		OnImageMenuPopup(hMenu);
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

	LoadImage(hWindow, szFilePath, true);

	UpdateAppTitle(hWindow);

	return 0;
}

static void UpdateImage(HWND hWindow)
{
	RedrawWindow(hWindow, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASENOW | RDW_UPDATENOW);
}

static void LoadImage(HWND hWindow, LPCTSTR pszFilePath, bool bShowErrorMsg)
{
	// �������ʉ摜��j��
	gs_ProcImage.Destroy();

	if(!ImageReader::ReadImage(pszFilePath, &gs_ImageData))
	{
		// �ǂݍ��ݎ��s
		if(bShowErrorMsg)
		{
			MessageBox(hWindow, TEXT("Failed to load the specified file."), TEXT("Error"), MB_ICONERROR | MB_OK | MB_TOPMOST);
		}
	}
	else
	{
		// �ǂݍ��ݐ��� - ���C���E�B���h�E���ĕ`�悵�ĉ�ʂɓǂݍ��񂾉摜��\������
		UpdateImage(hWindow);
	}
}

static void UpdateAppTitle(HWND hWindow)
{
	TCHAR szTitle[256] = { 0 };

	PFString::Copy(szTitle, TEXT("Photo Studio"));

	if(gs_ImageData.IsCreated())
	{
		PFString::Append(szTitle, TEXT(" - "));

		// �摜�� (�t�@�C���p�X�̏ꍇ�̓t�@�C��������������\���j
		{
			LPCTSTR pszImageName;

			pszImageName = PFString::Strrchr(gs_ImageData.GetImageName(), PF_DIR_SEP_CHAR);
			if(pszImageName != nullptr)
			{
				pszImageName++;
			}
			else
			{
				pszImageName =gs_ImageData.GetImageName();
			}

			PFString::Append(szTitle, pszImageName);
		}

		// �摜���
		{
			TCHAR szInfo[128] = { 0 };

			IImageData::SImageInfo ImageInfo = gs_ImageData.GetImageInfo();

			_stprintf_s(szInfo, TEXT(" [%dx%d %dch %dbit]"), ImageInfo.Width, ImageInfo.Height, ImageInfo.ChannelCount, ImageInfo.BitsPerChannel);
			PFString::Append(szTitle, szInfo);
		}
	}

	SetWindowText(hWindow, szTitle);
}
