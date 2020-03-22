#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
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
