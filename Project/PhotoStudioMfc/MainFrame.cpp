#include "stdafx.h"

#include <ImageReader.h>
#include <ImageWriter.h>
#include <ImageProc.h>

#include "ImageRenderer.h"

#include "Session.h"

#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DROPFILES()
	ON_COMMAND(ID_FILE_NEW, &CMainFrame::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
	ON_COMMAND(ID_FILE_CLOSE, &CMainFrame::OnFileClose)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, &CMainFrame::OnFileClose)
	ON_COMMAND(ID_FILE_SAVE, &CMainFrame::OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CMainFrame::OnFileSave)
	ON_COMMAND(ID_IMAGE_MONO, &CMainFrame::OnImageMono)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_MONO, &CMainFrame::OnImageMono)
END_MESSAGE_MAP()

CMainFrame::CMainFrame()
	: m_hIcon(nullptr)
{
	this->m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMainFrame::~CMainFrame()
{
}

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

	this->m_bAutoMenuEnable = FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
//	cs.lpszClass = AfxRegisterWndClass(0);
	if(cs.lpszName != nullptr)
	{
		WNDCLASS WindowClass = { 0 };

		::GetClassInfo(::AfxGetInstanceHandle(), cs.lpszClass, &WindowClass);
		WindowClass.lpszClassName = TEXT("PhotoStudio.MainWindow");
		WindowClass.hbrBackground = (HBRUSH)::GetStockObject(NULL_BRUSH);
		::AfxRegisterClass(&WindowClass);
		cs.lpszClass = WindowClass.lpszClassName;
	}

	return TRUE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ウィンドウにアイコンを設定
	this->SetIcon(this->m_hIcon, FALSE);
	this->SetIcon(this->m_hIcon, TRUE);

	// ファイルドロップ許可
	this->DragAcceptFiles(TRUE);

	// 前回の画像データを読み込む
	{
		TCHAR szFilePath[MAX_PATH] = { 0 };

		if(Session::RestoreSession(szFilePath, PF_ARRAY_LENGTH(szFilePath)))
		{
			this->LoadImage(szFilePath, false);
		}
	}

	this->UpdateAppTitle();

	return 0;
}

void CMainFrame::OnClose()
{
	Session::StoreSession(this->m_ImageData.GetImageName());

	CFrameWnd::OnClose();
}

void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

}

void CMainFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rcClient;
	SIZE ClientSize;

	this->GetClientRect(&rcClient);

	ClientSize.cx = rcClient.right - rcClient.left;
	ClientSize.cy = rcClient.bottom - rcClient.top;

	// 一旦画面を黒塗りする
	{
		HBRUSH hOldBrush = (HBRUSH)dc.SelectObject(GetStockObject(BLACK_BRUSH));

		dc.Rectangle(0, 0, ClientSize.cx, ClientSize.cy);

		dc.SelectObject(hOldBrush);
	}

	if(this->m_ImageData.IsCreated() || this->m_ProcImage.IsCreated())
	{
		// 処理結果を優先して表示
		IImageData* pImageData = this->m_ProcImage.IsCreated() ? &this->m_ProcImage : &this->m_ImageData;

		ImageRenderer::Render(dc.GetSafeHdc(), ClientSize, pImageData);
	}
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

}

void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
	TCHAR szFilePath[MAX_PATH] = { 0 };

	UINT ItemCount = DragQueryFile(hDropInfo, (UINT)-1, nullptr, 0);

	DragQueryFile(hDropInfo, 0, szFilePath, PF_ARRAY_LENGTH(szFilePath));

	this->LoadImage(szFilePath, true);

	this->UpdateAppTitle();

	CFrameWnd::OnDropFiles(hDropInfo);
}

void CMainFrame::UpdateImage()
{
	this->RedrawWindow(nullptr, nullptr, RDW_INVALIDATE | RDW_ERASENOW | RDW_UPDATENOW);
}

void CMainFrame::LoadImage(LPCTSTR pszFilePath, bool bShowErrorMsg)
{
	// 処理結果画像を破棄
	this->m_ProcImage.Destroy();

	if(!ImageReader::ReadImage(pszFilePath, &this->m_ImageData))
	{
		// 読み込み失敗
		if(bShowErrorMsg)
		{
			this->MessageBox(TEXT("Failed to load the specified file."), TEXT("Error"), MB_ICONERROR | MB_OK | MB_TOPMOST);
		}
	}
	else
	{
		// 読み込み成功 - メインウィンドウを再描画して画面に読み込んだ画像を表示する
		this->UpdateImage();
	}
}

void CMainFrame::UpdateAppTitle()
{
	TCHAR szTitle[256] = { 0 };

	_tcscpy_s(szTitle, TEXT("Photo Studio"));

	if(this->m_ImageData.IsCreated())
	{
		_tcscat_s(szTitle, TEXT(" - "));

		// 画像名 (ファイルパスの場合はファイル名部分だけを表示）
		{
			LPCTSTR pszImageName;

			pszImageName = _tcsrchr(this->m_ImageData.GetImageName(), _T('\\'));
			if(pszImageName != nullptr)
			{
				pszImageName++;
			}
			else
			{
				pszImageName =this->m_ImageData.GetImageName();
			}

			_tcscat_s(szTitle, pszImageName);
		}

		// 画像情報
		{
			TCHAR szInfo[128] = { 0 };

			IImageData::IMAGEINFO ImageInfo = this->m_ImageData.GetImageInfo();

			_stprintf_s(szInfo, TEXT(" [%dx%d %dch %dbit]"), ImageInfo.Width, ImageInfo.Height, ImageInfo.ChannelCount, ImageInfo.BitsPerChannel);
			_tcscat_s(szTitle, szInfo);
		}
	}

	this->SetWindowText(szTitle);
}

void CMainFrame::OnFileNew()
{
	// TODO
}

void CMainFrame::OnFileOpen()
{
	CFileDialog dlg(TRUE, nullptr, nullptr, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					TEXT("All files (*.*)|*.*||"));

	if(dlg.DoModal() == IDOK)
	{
		this->LoadImage(dlg.GetPathName(), true);

		this->UpdateAppTitle();
	}
}

void CMainFrame::OnFileClose()
{
	// 画像データを破棄して画面を再描画
	this->m_ImageData.Destroy();
	this->m_ProcImage.Destroy();

	this->UpdateImage();
}

void CMainFrame::OnFileClose(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(this->m_ImageData.IsCreated());
}

void CMainFrame::OnFileSave()
{
	CFileDialog dlg(FALSE, nullptr, nullptr, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					TEXT("All files (*.*)|*.*||"));

	if(dlg.DoModal() == IDOK)
	{
		IImageData* pImageData = this->m_ProcImage.IsCreated() ? &this->m_ProcImage : &this->m_ImageData;

		ImageWriter::WriteImage(dlg.GetPathName(), pImageData);
	}
}

void CMainFrame::OnFileSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(this->m_ImageData.IsCreated());
}

void CMainFrame::OnImageMono()
{
	if(this->m_ImageData.IsCreated())
	{
		ImageProc::GrayScale(&this->m_ProcImage, &this->m_ImageData);
		this->UpdateImage();
	}
}

void CMainFrame::OnImageMono(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(this->m_ImageData.IsCreated());
}
