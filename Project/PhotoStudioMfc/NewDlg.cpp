#include "stdafx.h"

#include <ImageProc.h>

#include "NewDlg.h"

IMPLEMENT_DYNAMIC(CNewDlg, CDialog)

CNewDlg::CNewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewDlg::IDD, pParent)
	, m_pImageData(nullptr)
{
}

CNewDlg::~CNewDlg()
{
}

void CNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNewDlg, CDialog)
END_MESSAGE_MAP()

BOOL CNewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->SetDlgItemText(IDC_EDIT_NAME, TEXT("Untitled"));

	this->SetDlgItemInt(IDC_EDIT_WIDTH, 256, FALSE);
	this->SetDlgItemInt(IDC_EDIT_HEIGHT, 256, FALSE);
	this->SetDlgItemInt(IDC_EDIT_CHANNELS, 3, FALSE);
	this->SetDlgItemInt(IDC_EDIT_DEPTH, 8, FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CNewDlg::OnOK()
{
	bool Result;

	TCHAR szImageName[256] = { 0 };

	this->GetDlgItemText(IDC_EDIT_NAME, szImageName, PF_ARRAY_LENGTH(szImageName));

	UInt32 Width = this->GetDlgItemInt(IDC_EDIT_WIDTH, nullptr, FALSE);
	UInt32 Height = this->GetDlgItemInt(IDC_EDIT_HEIGHT, nullptr, FALSE);
	UInt32 Channels = this->GetDlgItemInt(IDC_EDIT_CHANNELS, nullptr, FALSE);
	UInt32 Depth = this->GetDlgItemInt(IDC_EDIT_DEPTH, nullptr, FALSE);

	Result = this->m_pImageData->Create(szImageName, IImageData::EDataTypes::UnsignedInt, Width, Height, Channels, Depth);

	UInt32 MaxValue = (1 << Depth) - 1;

	// 白色にする（画素値を最大値にする）
	ImageProc::Fill(this->m_pImageData, MaxValue, MaxValue, MaxValue);

	this->EndDialog(Result ? 1 : 0);
}

void CNewDlg::OnCancel()
{
	this->EndDialog(0);
}
