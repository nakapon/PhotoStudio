#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"

class CPhotoStudioApp : public CWinApp
{
public:
	CPhotoStudioApp();
	~CPhotoStudioApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	DECLARE_MESSAGE_MAP()
};

extern CPhotoStudioApp theApp;
