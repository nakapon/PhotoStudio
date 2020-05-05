#include <Platform.h>

#include <AudioProc.h>

#include "NewDlg.h"

#include "resource.h"

static DWORD RATE_TBL[] =
{
	11025,
	22050,
	44100,
	48000,
	96000,
};

static INT_PTR CALLBACK DialogProcedure(HWND hDialog, UINT unMessage, WPARAM wParam, LPARAM lParam);

bool NewDlg::DoModal(HINSTANCE hInstance, HWND hWindow, IAudioData* pAudioData)
{
	Int32 Result;

	Result = DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_NEW), hWindow, DialogProcedure, (LPARAM)pAudioData);

	return (Result != 0) ? true : false;
}

static INT_PTR CALLBACK DialogProcedure(HWND hDialog, UINT unMessage, WPARAM wParam, LPARAM lParam)
{
	static IAudioData* s_pAudioData;

	switch(unMessage)
	{
	case WM_INITDIALOG:
		s_pAudioData = (IAudioData*)lParam; // DialogBoxParam の dwInitParam で渡したものが来る

		SetDlgItemText(hDialog, IDC_EDIT_NAME, TEXT("Untitled"));

		// TODO クリップボードに音声ファイルがある場合はクリップボード上の形式を設定する

		CheckRadioButton(hDialog, IDC_RADIO_8BIT, IDC_RADIO_16BIT, IDC_RADIO_16BIT);
		CheckRadioButton(hDialog, IDC_RADIO_MONO, IDC_RADIO_STEREO, IDC_RADIO_STEREO);

		for(UInt32 i = 0; i < PF_ARRAY_LENGTH(RATE_TBL); i++)
		{
			TCHAR szLabel[256] = { 0 };

			_stprintf_s(szLabel, TEXT("%d"), RATE_TBL[i]);

			SendMessage(GetDlgItem(hDialog, IDC_COMBO_RATE), CB_ADDSTRING, 0, (LPARAM)szLabel);
		}

		SendMessage(GetDlgItem(hDialog, IDC_COMBO_RATE), CB_SETCURSEL, 2, 0);

		SetDlgItemInt(hDialog, IDC_EDIT_SAMPLE, 5000, FALSE);

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

				TCHAR szAudioName[256] = { 0 };

				GetDlgItemText(hDialog, IDC_EDIT_NAME, szAudioName, PF_ARRAY_LENGTH(szAudioName));

				IAudioData::EDataTypes DataType = IsDlgButtonChecked(hDialog, IDC_RADIO_8BIT) ? IAudioData::EDataTypes::UInt8 : IAudioData::EDataTypes::Int16;
				UInt32 ChannelCount = IsDlgButtonChecked(hDialog, IDC_RADIO_MONO) ? 1 : 2;

				UInt32 RateIndex = SendMessage(GetDlgItem(hDialog, IDC_COMBO_RATE), CB_GETCURSEL, 0, 0);
				UInt32 SamplesPerSec = RATE_TBL[RateIndex];

				UInt32 SampleCountInMs = GetDlgItemInt(hDialog, IDC_EDIT_SAMPLE, nullptr, FALSE);

				UInt32 SampleCount = (SamplesPerSec * SampleCountInMs) / 1000;
				if(SampleCount == 0)
				{
					MessageBox(hDialog, TEXT("Sample count is zero"), TEXT("Error"), MB_OK | MB_ICONERROR | MB_TOPMOST);
					Result = false;
				}
				else
				{
					Result = s_pAudioData->Create(szAudioName, DataType, ChannelCount, SamplesPerSec, SampleCount);
				}

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
