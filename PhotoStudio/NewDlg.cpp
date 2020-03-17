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
		s_pImageData = (IImageData*)lParam; // DialogBoxParam の dwInitParam で渡したものが来る

		SetDlgItemText(hDialog, IDC_EDIT_NAME, TEXT("Untitled"));

		// TODO クリップボードに画像がある場合はクリップボード上の画像サイズを設定する (PhotoShop仕様)
		SetDlgItemInt(hDialog, IDC_EDIT_WIDTH, 256, FALSE);
		SetDlgItemInt(hDialog, IDC_EDIT_HEIGHT, 256, FALSE);
		SetDlgItemInt(hDialog, IDC_EDIT_CHANNELS, 3, FALSE);
		SetDlgItemInt(hDialog, IDC_EDIT_DEPTH, 8, FALSE);

		return 1; /* コントロールにフォーカスを与える場合は非0を返す */

	case WM_CLOSE:
		// キャンセルは 0 を返す
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

				// 白色にする（画素値を最大値にする）
				ImageProc::Fill(s_pImageData, MaxValue, MaxValue, MaxValue);

				// 成功した場合は 1 を返す
				EndDialog(hDialog, Result ? 1 : 0);
			}
			break;

		case IDCANCEL:
			// キャンセルは 0 を返す
			EndDialog(hDialog, 0);
			break;
		}
		break;
	}

	return 0;
}
