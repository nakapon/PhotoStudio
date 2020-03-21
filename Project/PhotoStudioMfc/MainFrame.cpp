#include "stdafx.h"

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

	// アイコンをセット
	this->SetIcon(this->m_hIcon, FALSE);
	this->SetIcon(this->m_hIcon, TRUE);


	return 0;
}

void CMainFrame::OnClose()
{

	CFrameWnd::OnClose();
}

void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

}

void CMainFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting

}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

}
