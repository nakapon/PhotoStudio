#pragma once

#include <IImageData.h>

#include "resource.h"

class CNewDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewDlg)

public:
	CNewDlg(CWnd* pParent = NULL);
	virtual ~CNewDlg();

	enum { IDD = IDD_NEW };

	IImageData* m_pImageData;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();

	virtual void OnOK();
	virtual void OnCancel();
};
