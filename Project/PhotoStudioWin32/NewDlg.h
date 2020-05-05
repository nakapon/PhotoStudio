#pragma once

#include <IImageData.h>

class NewDlg
{
public:

	// 「新規作成」モーダルダイアログ
	static bool DoModal(HINSTANCE hInstance, HWND hWindow, IImageData* pImageData);

private:
	NewDlg() = delete;
	~NewDlg() = delete;
	NewDlg(const NewDlg&) = delete;
	NewDlg& operator=(const NewDlg&) = delete;
};
