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

static CImageData gs_ImageData; /* 読み込んだ画像データ */
static CImageData gs_ProcImage; /* 処理結果画像データ */

static void UpdateImage(HWND hWindow);
static void LoadImage(HWND hWindow, LPCTSTR pszFilePath, bool bShowErrorMsg);

static void UpdateAppTitle(HWND hWindow);

static void OnFileMenuPopup(HMENU hMenu);
static void OnImageMenuPopup(HMENU hMenu);

// メインウィンドウ作成時の処理
INT OnCreate(HWND hWindow, CREATESTRUCT* pCreateStruct)
{
	// ウィンドウにアイコンを設定
	{
		HICON hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_SYMBOL));
		SendMessage(hWindow, WM_SETICON, ICON_BIG, (WPARAM)hIcon);
		SendMessage(hWindow, WM_SETICON, ICON_SMALL, (WPARAM)hIcon);
	}

	// ファイルドロップ許可
	DragAcceptFiles(hWindow, TRUE);

	// 前回の画像データを読み込む
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

// メインウィンドウの「閉じる」ボタン押下時の処理
INT OnClose(HWND hWindow)
{
	DestroyWindow(hWindow);

	Session::StoreSession(gs_ImageData.GetImageName());

	return 0;
}

// メインウィンドウの破棄時の処理
INT OnDestroy(HWND hWindow)
{
	// メッセージループを抜ける為に WM_QUIT メッセージを投げる
	PostQuitMessage(0);

	return 0;
}

// メインウィンドウの再描画時の処理
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

	// 一旦画面を黒塗りする
	{
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(BLACK_BRUSH));

		Rectangle(hDC, 0, 0, ClientSize.cx, ClientSize.cy);

		SelectObject(hDC, hOldBrush);
	}

	if(gs_ImageData.IsCreated() || gs_ProcImage.IsCreated())
	{
		// 処理結果を優先して表示
		IImageData* pImageData = gs_ProcImage.IsCreated() ? &gs_ProcImage : &gs_ImageData;

		ImageRenderer::Render(hDC, ClientSize, pImageData);
	}

	EndPaint(hWindow, &PaintStruct);

	return 0;
}

// メインウィンドウのコマンド（メニュー押下，ボタン押下など）処理
INT OnCommand(HWND hWindow, WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case ID_FILE_NEW:
		// MENU 「File」→「New」の処理
		if(NewDlg::DoModal(g_hInstance, hWindow, &gs_ImageData))
		{
			gs_ProcImage.Destroy();

			UpdateImage(hWindow);

			UpdateAppTitle(hWindow);
		}
		break;

	case ID_FILE_OPEN:
		// MENU 「File」→「Open」の処理
		{
			OPENFILENAME OpenFileName = { 0 };

			TCHAR szFilePath[MAX_PATH] = { 0 };

			// 「ファイル選択ダイアログ」を表示
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
		// MENU 「File」→「Close」の処理
		// 画像データを破棄して画面を再描画
		gs_ImageData.Destroy();
		gs_ProcImage.Destroy();
		UpdateImage(hWindow);
		UpdateAppTitle(hWindow);
		break;

	case ID_FILE_SAVE:
		// MENU 「File」→「Save」の処理
		if(gs_ImageData.IsCreated() || gs_ProcImage.IsCreated())
		{
			OPENFILENAME SaveFileName = { 0 };

			TCHAR szFilePath[MAX_PATH] = { 0 };

			// 「ファイル選択ダイアログ」を表示
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

// メインウィンドウの通知処理
INT OnNotify(HWND hWindow, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

// メインウィンドウのスクロールバーの処理
INT OnScrollEvent(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

// メインウィンドウのマウスイベント処理
INT OnMouseEvent(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

// メインウィンドウのキー処理
INT OnKeyEvent(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

// メニューを展開するときの処理
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

// ファイルをウィンドウにドロップしたときの処理
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
	// 処理結果画像を破棄
	gs_ProcImage.Destroy();

	if(!ImageReader::ReadImage(pszFilePath, &gs_ImageData))
	{
		// 読み込み失敗
		if(bShowErrorMsg)
		{
			MessageBox(hWindow, TEXT("Failed to load the specified file."), TEXT("Error"), MB_ICONERROR | MB_OK | MB_TOPMOST);
		}
	}
	else
	{
		// 読み込み成功 - メインウィンドウを再描画して画面に読み込んだ画像を表示する
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

		// 画像名 (ファイルパスの場合はファイル名部分だけを表示）
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

		// 画像情報
		{
			TCHAR szInfo[128] = { 0 };

			IImageData::SImageInfo ImageInfo = gs_ImageData.GetImageInfo();

			_stprintf_s(szInfo, TEXT(" [%dx%d %dch %dbit]"), ImageInfo.Width, ImageInfo.Height, ImageInfo.ChannelCount, ImageInfo.BitsPerChannel);
			PFString::Append(szTitle, szInfo);
		}
	}

	SetWindowText(hWindow, szTitle);
}
