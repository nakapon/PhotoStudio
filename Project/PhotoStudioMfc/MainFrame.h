#pragma once

#include <ImageData.h>

#include "resource.h"

class CMainFrame : public CFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame();
	virtual ~CMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	HICON m_hIcon;

	CImageData m_ImageData;
	CImageData m_ProcImage;

protected:
	void UpdateImage();
	void LoadImage(LPCTSTR pszFilePath, bool bShowErrorMsg);

	void UpdateAppTitle();

protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDropFiles(HDROP hDropInfo);

	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileClose();
	afx_msg void OnFileClose(CCmdUI *pCmdUI);
	afx_msg void OnFileSave();
	afx_msg void OnFileSave(CCmdUI *pCmdUI);

	afx_msg void OnImageMono();
	afx_msg void OnImageMono(CCmdUI *pCmdUI);
};
