#include <Platform.h>

#include <windowsx.h>

#include <imgui/imgui.h>

#include <imgui/PlatformOpenGL2.h>
#include <imgui/StyleLibrary.h>

#include <ImageData.h>
#include <ImageReader.h>
#include <ImageWriter.h>

#include <ImageProc.h>

#include "ImageView.h"

#include "Session.h"

#include "resource.h"

#define TIMER_ID_RENDER		1

#define TARGET_FRAME_RATE	30	// FPS

extern HINSTANCE g_hInstance;

static ImGui::CPlatformOpenGL2 gs_Platform;

static CImageView gs_ImageView;

static CImageData gs_ImageData; /* 読み込んだ画像データ */
static CImageData gs_ProcImage; /* 処理結果画像データ */

static bool gs_IsRenderingRateVisible = true;

static bool gs_ShowNew = false;

static bool gs_ShowImGuiMetrics = false;
static bool gs_ShowImGuiStyleEditor = false;
static bool gs_ShowImGuiAbout = false;
static bool gs_ShowImGuiDemo = false;

static void LoadImage(HWND hWindow, LPCTSTR pszFilePath, bool bShowErrorMsg);

static void UpdateImageView(IImageData* pImageData);

static void UpdateAppTitle(HWND hWindow);

static void Render(HWND hWindow);
static ImVec2 RenderMainMenu(HWND hWindow);
static void RenderCore(HWND hWindow);

static void OnFileNew(HWND hWindow);
static void OnFileOpen(HWND hWindow);
static void OnFileClose(HWND hWindow);
static void OnFileSave(HWND hWindow);

static void OnImageGray(HWND hWindow);

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

	// ImGui生成
	{
		static TCHAR s_szFilePath[MAX_PATH] = { 0 };

		PFPath::GetModuleDirPath(nullptr, s_szFilePath);
		PFString::Append(s_szFilePath, TEXT("\\PhotoStudioImu.ini"));

		gs_Platform.Create(hWindow, TEXT("Meiryo UI"), 18, s_szFilePath);

		ImGui::StyleLibrary::PushWindowStyles();

		ImGui::StyleLibrary::ApplyDarkColors();
	}

	// レンダリング用タイマー起動
	SetTimer(hWindow, TIMER_ID_RENDER, 1000 / TARGET_FRAME_RATE, nullptr);

	// 前回の画像データを読み込む
	{
		TCHAR szFilePath[MAX_PATH] = { 0 };

		if(Session::RestoreSession(szFilePath, PF_ARRAY_LENGTH(szFilePath)))
		{
			LoadImage(hWindow, szFilePath, false);

			UpdateImageView(&gs_ImageData);
		}
	}

	UpdateAppTitle(hWindow);

	// 1フレーム目だけここでレンダリングする
	Render(hWindow);

	return 0;
}

// メインウィンドウの「閉じる」ボタン押下時の処理
INT OnClose(HWND hWindow)
{
	DestroyWindow(hWindow);

	Session::StoreSession(gs_ImageData.GetImageName());

	KillTimer(hWindow, TIMER_ID_RENDER);

	return 0;
}

// メインウィンドウの破棄時の処理
INT OnDestroy(HWND hWindow)
{
	gs_Platform.Destroy();

	// メッセージループを抜ける為に WM_QUIT メッセージを投げる
	PostQuitMessage(0);

	return 0;
}

// メインウィンドウのサイズ変更時の処理
INT OnSize(HWND hWindow, WPARAM wParam, LPARAM lParam)
{
	Render(hWindow);

	return 0;
}

// メインウィンドウのコマンド処理
INT OnCommand(HWND hWindow, WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case ID_FILE_NEW:
		OnFileNew(hWindow);
		break;

	case ID_FILE_OPEN:
		OnFileOpen(hWindow);
		break;

	case ID_FILE_CLOSE:
		OnFileClose(hWindow);
		break;

	case ID_FILE_SAVE:
		OnFileSave(hWindow);
		break;
	}

	return 0;
}

// メインウィンドウのタイマー処理
INT OnTimer(HWND hWindow, WPARAM wParam, LPARAM lParam)
{
	Render(hWindow);

	return 0;
}

// メインウィンドウのマウスイベント処理
INT OnMouseEvent(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if(Message == WM_LBUTTONDOWN || Message == WM_LBUTTONUP
	|| Message == WM_RBUTTONDOWN || Message == WM_RBUTTONUP)
	{
		gs_Platform.OnMouseButton(Message);
	}
	else if(Message == WM_MOUSEMOVE)
	{
		POINT Point;

		Point.x = GET_X_LPARAM(lParam);
		Point.y = GET_Y_LPARAM(lParam);

		gs_Platform.OnMouseMove(Point);
	}
	else if(Message == WM_MOUSEWHEEL)
	{
		Int32 Delta = GET_WHEEL_DELTA_WPARAM(wParam);

		gs_Platform.OnMouseWheel(Delta);
	}

	return 0;
}

// メインウィンドウのキー処理
INT OnKeyEvent(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if(Message == WM_KEYDOWN)
	{
		gs_Platform.OnKeyDown(wParam);
	}
	else if(Message == WM_KEYUP)
	{
		gs_Platform.OnKeyUp(wParam);
	}
	else if(Message == WM_CHAR)
	{
		gs_Platform.OnChar(wParam);
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

	UpdateImageView(&gs_ImageData);

	UpdateAppTitle(hWindow);

	Render(hWindow);

	return 0;
}

static void OnFileNew(HWND hWindow)
{
	gs_ShowNew = true;
}

static void OnFileOpen(HWND hWindow)
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

		UpdateImageView(&gs_ImageData);

		UpdateAppTitle(hWindow);

		Render(hWindow);
	}
}

static void OnFileClose(HWND hWindow)
{
	gs_ImageData.Destroy();
	gs_ProcImage.Destroy();

	UpdateImageView(nullptr);

	UpdateAppTitle(hWindow);

	Render(hWindow);
}

static void OnFileSave(HWND hWindow)
{
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
}

static void OnImageGray(HWND hWindow)
{
	if(gs_ImageData.IsCreated())
	{
		ImageProc::GrayScale(&gs_ProcImage, &gs_ImageData);

		UpdateImageView(&gs_ProcImage);

		Render(hWindow);
	}
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
		Render(hWindow);
	}
}

static void UpdateImageView(IImageData* pImageData)
{
	gs_Platform.MakeCurrentGL();

	gs_ImageView.SetImage(pImageData);

	gs_Platform.ReleaseCurrentGL();
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

static void Render(HWND hWindow)
{
	static bool s_IsRendering;

	COLORREF CLEAR_COLOR = RGB(45, 45, 48);

	RECT rcClient;

	if(s_IsRendering)
		return;

	s_IsRendering = true;

	GetClientRect(hWindow, &rcClient);

	if(gs_Platform.BeginRender(rcClient, CLEAR_COLOR))
	{
		RenderCore(hWindow);

		gs_Platform.EndRender();
	}

	s_IsRendering = false;
}

static ImVec2 RenderMainMenu(HWND hWindow)
{
	ImVec2 MenuBarSize;

	ImGuiIO& io = ImGui::GetIO();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 2.0f));

	ImGui::PushID("MainMenu");

	if(ImGui::BeginMainMenuBar())
	{
		MenuBarSize = ImGui::GetWindowSize();

		if(ImGui::BeginMenu("File"))
		{
			if(ImGui::MenuItem("New...", "Ctrl+N"))
			{
				OnFileNew(hWindow);
			}

			if(ImGui::MenuItem("Open...", "Ctrl+O"))
			{
				OnFileOpen(hWindow);
			}

			ImGui::Separator();

			if(ImGui::MenuItem("Close...", "Ctrl+W", nullptr, gs_ImageData.IsCreated()))
			{
				OnFileClose(hWindow);
			}

			ImGui::Separator();

			if(ImGui::MenuItem("Save...", "Ctrl+S", nullptr, gs_ImageData.IsCreated()))
			{
				OnFileSave(hWindow);
			}

			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("Image"))
		{
			if(ImGui::MenuItem("Monochrome", nullptr, nullptr, gs_ImageData.IsCreated()))
			{
				OnImageGray(hWindow);
			}

			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("Tool"))
		{
			if(ImGui::MenuItem("Metrics", nullptr, &gs_ShowImGuiMetrics))
			{
			}

			if(ImGui::MenuItem("Style Editor", nullptr, &gs_ShowImGuiStyleEditor))
			{
			}

			if(ImGui::MenuItem("Demo", nullptr, &gs_ShowImGuiDemo))
			{
			}

			ImGui::Separator();

			if(ImGui::MenuItem("About ImGui", nullptr, &gs_ShowImGuiAbout))
			{
			}

			ImGui::EndMenu();
		}

		if(gs_IsRenderingRateVisible)
		{
			CHAR szInfo[256] = { 0 };

			sprintf_s(szInfo, "%.3f [ms] (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

			ImVec2 TextSize = ImGui::CalcTextSize(szInfo);
			float CursorX = ImGui::GetCursorPosX();
			float Space = ImGui::GetStyle().ItemSpacing.x;

			ImGui::Dummy(ImVec2(ImGui::GetWindowWidth() - CursorX - TextSize.x - 2.0f * Space, 0.0f));
			ImGui::Text(szInfo);
		}

		ImGui::EndMainMenuBar();
	}

	ImGui::PopID();

	ImGui::PopStyleVar(2);

	return MenuBarSize;
}

static void RenderCore(HWND hWindow)
{
	ImVec2 DisplaySize;

	ImVec2 MainMenuBarSize;

	ImVec2 MainPos, MainSize;

	ImGuiIO& io = ImGui::GetIO();

	DisplaySize = io.DisplaySize;

	// Main menu
	MainMenuBarSize = RenderMainMenu(hWindow);

	MainPos.x = 0;
	MainPos.y = MainMenuBarSize.y - 1.0f;

	MainSize.x = DisplaySize.x;
	MainSize.y = DisplaySize.y - MainMenuBarSize.y + 1.0f;

	// Main Image View
	{
		ImGui::SetNextWindowPos(MainPos, ImGuiCond_Always);
		ImGui::SetNextWindowSize(MainSize, ImGuiCond_Always);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);

		ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoTitleBar
								 | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus
								 | ImGuiWindowFlags_NoSavedSettings;

		ImGui::Begin("Document", nullptr, WindowFlags);

		gs_ImageView.Render(hWindow);

		ImGui::End();

		ImGui::PopStyleVar();
	}

	// ImGui Utilities
	if(gs_ShowImGuiMetrics)
	{
		ImGui::ShowMetricsWindow(&gs_ShowImGuiMetrics);
	}

	if(gs_ShowImGuiStyleEditor)
	{
		ImGui::Begin("Style Editor", &gs_ShowImGuiStyleEditor);

		ImGui::ShowStyleEditor();

		ImGui::End();
	}

	if(gs_ShowImGuiAbout)
	{
		ImGui::ShowAboutWindow(&gs_ShowImGuiAbout);
	}

	if(gs_ShowImGuiDemo)
	{
		ImGui::ShowDemoWindow(&gs_ShowImGuiDemo);
	}

	// Dialogs
	if(gs_ShowNew)
	{
		ImGui::OpenPopup("New");

		if(ImGui::BeginPopupModal("New", &gs_ShowNew, ImGuiWindowFlags_AlwaysAutoResize))
		{
			static CHAR s_szImageName[MAX_PATH] = "Untitiled";
			static IImageData::SImageInfo s_ImageInfo = {256, 256, 3, 8};
			static float s_Color[3] = {1.0f, 1.0f, 1.0f};

			static const auto InitParam = [&](){
				PFStringA::Copy(s_szImageName, "Untitiled");

				s_ImageInfo.Width = 256;
				s_ImageInfo.Height = 256;
				s_ImageInfo.ChannelCount = 3;
				s_ImageInfo.BitsPerChannel = 8;
			};

			ImGui::InputText("Image Name", s_szImageName, IM_ARRAYSIZE(s_szImageName));

			ImGui::Dummy(ImVec2(0.0f, 8.0f));

			ImGui::InputInt("Width", (int*)&s_ImageInfo.Width);
			ImGui::InputInt("Height", (int*)&s_ImageInfo.Height);
			ImGui::InputInt("Channels", (int*)&s_ImageInfo.ChannelCount);
			ImGui::InputInt("Depth", (int*)&s_ImageInfo.BitsPerChannel);

			ImGui::Dummy(ImVec2(0.0f, 8.0f));

			ImGui::ColorEdit3("Background", s_Color);

			ImGui::Dummy(ImVec2(0.0f, 8.0f));

			ImGui::Separator();

			ImGui::Dummy(ImVec2(0.0f, 8.0f));

			if(ImGui::Button("OK", ImVec2(120, 0)))
			{
				if(gs_ImageData.Create(ASTR_TO_TSTR(s_szImageName), IImageData::EDataTypes::UnsignedInt, s_ImageInfo))
				{
					UInt32 MaxValue;
					UInt32 R, G, B;

					MaxValue = (1 << s_ImageInfo.BitsPerChannel) - 1;

					R = (UInt32)(MaxValue * s_Color[0] + 0.5f);
					G = (UInt32)(MaxValue * s_Color[1] + 0.5f);
					B = (UInt32)(MaxValue * s_Color[2] + 0.5f);

					ImageProc::Fill(&gs_ImageData, R, G, B);

					gs_ProcImage.Destroy();

					UpdateImageView(&gs_ImageData);

					UpdateAppTitle(hWindow);
				}

				gs_ShowNew = false;
				InitParam();

				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if(ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				gs_ShowNew = false;
				InitParam();

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}
