#include <Platform.h>

#include <ImageProc.h>

#include "NewDlg.h"

#include "resource.h"

static INT_PTR CALLBACK DialogProcedure(HWND hDialog, UINT unMessage, WPARAM wParam, LPARAM lParam);

bool NewDlg::DoModal(HINSTANCE hInstance, HWND hWindow, IImageData* pImageData)
{
	Int32 Result;

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

				GetDlgItemText(hDialog, IDC_EDIT_NAME, szImageName, PF_ARRAY_LENGTH(szImageName));

				IImageData::EDataTypes DataType = IImageData::EDataTypes::UInt;
				UInt32 Width = GetDlgItemInt(hDialog, IDC_EDIT_WIDTH, nullptr, FALSE);
				UInt32 Height = GetDlgItemInt(hDialog, IDC_EDIT_HEIGHT, nullptr, FALSE);
				UInt32 Channels = GetDlgItemInt(hDialog, IDC_EDIT_CHANNELS, nullptr, FALSE);
				UInt32 BitsPerChannel = GetDlgItemInt(hDialog, IDC_EDIT_DEPTH, nullptr, FALSE);

				Result = s_pImageData->Create(szImageName, DataType, Width, Height, Channels, BitsPerChannel);

				UInt32 MaxValue = (1 << BitsPerChannel) - 1;

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