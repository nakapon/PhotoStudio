#include <windows.h>

#include "ImageProc.h"

#include "NewDlg.h"

#include "resource.h"

static INT_PTR CALLBACK DialogProcedure(HWND hDialog, UINT unMessage, WPARAM wParam, LPARAM lParam);

bool NewDlg::DoModal(HINSTANCE hInstance, HWND hWindow, IImageData* pImageData)
{
	INT Result;

	Result = DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_NEW), hWindow, DialogProcedure, (LPARAM)pImageData);

	return (Result != 0) ? true : false;
}

static INT_PTR CALLBACK DialogProcedure(HWND hDialog, UINT unMessage, WPARAM wParam, LPARAM lParam)
{
	static IImageData* s_pImageData;

	switch(unMessage)
	{
	case WM_INITDIALOG:
		s_pImageData = (IImageData*)lParam; // DialogBoxParam �� dwInitParam �œn�������̂�����

		SetDlgItemText(hDialog, IDC_EDIT_NAME, TEXT("Untitled"));

		// TODO �N���b�v�{�[�h�ɉ摜������ꍇ�̓N���b�v�{�[�h��̉摜�T�C�Y��ݒ肷�� (PhotoShop�d�l)
		SetDlgItemInt(hDialog, IDC_EDIT_WIDTH, 256, FALSE);
		SetDlgItemInt(hDialog, IDC_EDIT_HEIGHT, 256, FALSE);
		SetDlgItemInt(hDialog, IDC_EDIT_CHANNELS, 3, FALSE);
		SetDlgItemInt(hDialog, IDC_EDIT_DEPTH, 8, FALSE);

		return 1; /* �R���g���[���Ƀt�H�[�J�X��^����ꍇ�͔�0��Ԃ� */

	case WM_CLOSE:
		// �L�����Z���� 0 ��Ԃ�
		EndDialog(hDialog, 0);
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				bool Result;

				TCHAR szImageName[256] = { 0 };

				GetDlgItemText(hDialog, IDC_EDIT_NAME, szImageName, sizeof(szImageName) / sizeof(szImageName[0]));

				UINT Width = GetDlgItemInt(hDialog, IDC_EDIT_WIDTH, nullptr, FALSE);
				UINT Height = GetDlgItemInt(hDialog, IDC_EDIT_HEIGHT, nullptr, FALSE);
				UINT Channels = GetDlgItemInt(hDialog, IDC_EDIT_CHANNELS, nullptr, FALSE);
				UINT Depth = GetDlgItemInt(hDialog, IDC_EDIT_DEPTH, nullptr, FALSE);

				Result = s_pImageData->Create(szImageName, Width, Height, Channels, Depth);

				UINT MaxValue = (1 << Depth) - 1;

				// ���F�ɂ���i��f�l���ő�l�ɂ���j
				ImageProc::Fill(s_pImageData, MaxValue, MaxValue, MaxValue);

				// ���������ꍇ�� 1 ��Ԃ�
				EndDialog(hDialog, Result ? 1 : 0);
			}
			break;

		case IDCANCEL:
			// �L�����Z���� 0 ��Ԃ�
			EndDialog(hDialog, 0);
			break;
		}
		break;
	}

	return 0;
}
