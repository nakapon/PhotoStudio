#include <Platform.h>

// メモリリーク検知有効
#define __DETECT_MEMORY_LEAKS__

#ifdef __DETECT_MEMORY_LEAKS__
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif // __DETECT_MEMORY_LEAKS__

#include "resource.h"

// ウィンドウタイトル
#define APP_WINDOW_NAME	TEXT("Photo Studio")

// ウィンドウクラス
#define APP_CLASS_NAME	TEXT("PhotoStudioClass")

// ウィンドウサイズ
#define WINDOW_WIDTH	1200
#define WINDOW_HEIGHT	800

HINSTANCE g_hInstance;

INT OnCreate(HWND hWindow, CREATESTRUCT* pCreateStruct);
INT OnClose(HWND hWindow);
INT OnDestroy(HWND hWindow);
INT OnPaint(HWND hWindow);
INT OnCommand(HWND hWindow, WPARAM wParam, LPARAM lParam);
INT OnNotify(HWND hWindow, WPARAM wParam, LPARAM lParam);
INT OnScrollEvent(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam);
INT OnMouseEvent(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam);
INT OnKeyEvent(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam);
INT OnInitMenuPopup(HWND hWindow, WPARAM wParam, LPARAM lParam);
INT OnDropFiles(HWND hWindow, WPARAM wParam, LPARAM lParam);

static LRESULT CALLBACK WindowProcedure(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
	case WM_CREATE:
		return OnCreate(hWindow, (CREATESTRUCT*)lParam);

	case WM_CLOSE:
		return OnClose(hWindow);

	case WM_DESTROY:
		return OnDestroy(hWindow);

	case WM_PAINT:
		return OnPaint(hWindow);

	case WM_COMMAND:
		return OnCommand(hWindow, wParam, lParam);

	case WM_NOTIFY:
		return OnNotify(hWindow, wParam, lParam);

	case WM_HSCROLL:	case WM_VSCROLL:
		return OnScrollEvent(hWindow, Message, wParam, lParam);

	case WM_LBUTTONDOWN: case WM_LBUTTONUP:
	case WM_RBUTTONDOWN: case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		return OnMouseEvent(hWindow, Message, wParam, lParam);

	case WM_KEYDOWN:	case WM_KEYUP:
	case WM_CHAR:
		return OnKeyEvent(hWindow, Message, wParam, lParam);

	case WM_INITMENUPOPUP:
		return OnInitMenuPopup(hWindow, wParam, lParam);

	case WM_DROPFILES:
		return OnDropFiles(hWindow, wParam, lParam);

	default:
		return DefWindowProc(hWindow, Message, wParam, lParam);
	}

	return 0;
}

static BOOL InitializeApplication(HINSTANCE hInstance)
{
	WNDCLASSEX WindowClassEx = { 0 };

	WindowClassEx.cbSize = sizeof(WindowClassEx);
	WindowClassEx.style = CS_HREDRAW | CS_VREDRAW;
	WindowClassEx.lpfnWndProc = WindowProcedure;
	WindowClassEx.cbClsExtra = 0;
	WindowClassEx.cbWndExtra = 0;
	WindowClassEx.hInstance = hInstance;
	WindowClassEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WindowClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	WindowClassEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WindowClassEx.lpszMenuName = NULL;
	WindowClassEx.lpszClassName = APP_CLASS_NAME;
	WindowClassEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	return (RegisterClassEx(&WindowClassEx) == 0) ? FALSE : TRUE;
}

static BOOL CreateMainWindow(HINSTANCE hInstance, INT nShowCommand, HWND &hWindow)
{
	HMENU hMainMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MAIN_MENU));

	hWindow = CreateWindowEx(0, APP_CLASS_NAME, APP_WINDOW_NAME,
							 WS_OVERLAPPEDWINDOW, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
							 NULL, hMainMenu, hInstance, NULL);
	if(hWindow == NULL)
		return FALSE;

	ShowWindow(hWindow, nShowCommand);

	UpdateWindow(hWindow);

	return TRUE;
}

// WinMain - Windowsアプリの場合はエントリポイントは main ではなく WinMain
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pszCommandLine, INT nShowCommand)
{
	INT nReturn;

	HWND hWindow;

	g_hInstance = hInstance;

	CoInitialize(NULL);

#ifdef __DETECT_MEMORY_LEAKS__
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_WNDW | _CRTDBG_MODE_DEBUG);
	//_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_WNDW | _CRTDBG_MODE_DEBUG);
	//_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_WNDW | _CRTDBG_MODE_DEBUG);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // __DETECT_MEMORY_LEAKS__

	// アプリケーション初期化 - ウィンドウクラスの登録
	if(!InitializeApplication(hInstance))
		return -1;

	// メインウィンドウの作成
	if(!CreateMainWindow(hInstance, nShowCommand, hWindow))
		return -1;

	// メッセージループ．メインウィンドウを閉じる(WM_QUIT メッセージを受信する)まで無限ループ．
	{
		INT nResult = 0;

		MSG Message;
		HACCEL hAccel;

		hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));

		// GetMessage は WM_QUIT を受信すると FALSE を返す
		while((nResult = GetMessage(&Message, NULL, 0, 0)) != 0)
		{
			if(nResult == -1)
			{
				MessageBox(hWindow, TEXT("Fatal error occurred at message loop!!"), TEXT("Error"), MB_ICONERROR | MB_TOPMOST | MB_OK);
			}
			else
			{
				if(!TranslateAccelerator(hWindow, hAccel, &Message))
				{
					TranslateMessage(&Message);
					DispatchMessage(&Message);
				}
			}
		}

		nReturn = (INT)Message.wParam;
	}

	CoUninitialize();

	return nReturn;
}
