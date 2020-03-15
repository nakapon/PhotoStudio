#include <windows.h>

#include "ImageData.h"
#include "ImageReader.h"
#include "ImageRenderer.h"

#include "ImageProc.h"

#include "resource.h"

extern HINSTANCE g_hInstance;

static CImageData gs_ImageData; /* �ǂݍ��񂾉摜�f�[�^ */
static CImageData gs_ProcImage; /* �������ʉ摜�f�[�^ */

// ���C���E�B���h�E�쐬���̏���
INT OnCreate(HWND hWindow, CREATESTRUCT* pCreateStruct)
{
	// �E�B���h�E�ɃA�C�R����ݒ�
	{
		HICON hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_SYMBOL));
		SendMessage(hWindow, WM_SETICON, ICON_BIG, (WPARAM)hIcon);
		SendMessage(hWindow, WM_SETICON, ICON_SMALL, (WPARAM)hIcon);
	}

	return 0;
}

// ���C���E�B���h�E�́u����v�{�^���������̏���
INT OnClose(HWND hWindow)
{
	DestroyWindow(hWindow);

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
		if(gs_ProcImage.IsCreated())
		{
			// �������ʂ�\��
			ImageRenderer::Render(hDC, ClientSize, gs_ProcImage);
		}
		else
		{
			// �I���W�i����\��
			ImageRenderer::Render(hDC, ClientSize, gs_ImageData);
		}
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
		break;

	case ID_FILE_OPEN:
		// MENU �uFile�v���uOpen�v�̏���
		{
			OPENFILENAME OpenFileName = { 0 };

			TCHAR szFileName[MAX_PATH] = { 0 };

			// �u�t�@�C���I���_�C�A���O�v��\��
			OpenFileName.lStructSize     = sizeof(OPENFILENAME);
			OpenFileName.hwndOwner       = hWindow;
			OpenFileName.lpstrFilter     = TEXT("All Files(*.*)\0*.*\0\0");
			OpenFileName.nFilterIndex    = 1;
			OpenFileName.lpstrFile       = szFileName;
			OpenFileName.nMaxFile        = MAX_PATH;
			OpenFileName.Flags           = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			OpenFileName.lpstrDefExt     = TEXT("");
			OpenFileName.nMaxFileTitle   = MAX_PATH;
			OpenFileName.lpstrFileTitle  = nullptr;
			OpenFileName.lpstrTitle      = nullptr;

			if(GetOpenFileName(&OpenFileName))
			{
				// �������ʉ摜��j��
				gs_ProcImage.Destroy();

				if(!ImageReader::ReadImage(szFileName, gs_ImageData))
				{
					// �ǂݍ��ݎ��s
					MessageBox(hWindow, TEXT("Failed to load the specified file."), TEXT("Error"), MB_ICONERROR | MB_OK | MB_TOPMOST);
				}
				else
				{
					// �ǂݍ��ݐ��� - ���C���E�B���h�E���ĕ`�悵�ĉ�ʂɓǂݍ��񂾉摜��\������
					RedrawWindow(hWindow, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASENOW | RDW_UPDATENOW);
				}
			}
		}
		break;

	case ID_FILE_CLOSE:
		// MENU �uFile�v���uClose�v�̏���
		// �摜�f�[�^��j�����ĉ�ʂ��ĕ`��
		gs_ImageData.Destroy();
		gs_ProcImage.Destroy();
		RedrawWindow(hWindow, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASENOW | RDW_UPDATENOW);
		break;

	case ID_FILE_SAVE:
		// MENU �uFile�v���uSave�v�̏���
		break;


	case ID_IMAGE_MONO:
		if(gs_ImageData.IsCreated())
		{
			ImageProc::GrayScale(gs_ProcImage, gs_ImageData);
			RedrawWindow(hWindow, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASENOW | RDW_UPDATENOW);
		}
		break;
	}

	return 0;
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

