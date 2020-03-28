#pragma once

#include <IAudioData.h>

class NewDlg
{
public:

	// 「新規作成」モーダルダイアログ
	static bool DoModal(HINSTANCE hInstance, HWND hWindow, IAudioData* pAudioData);

private:
	NewDlg() = delete;
	~NewDlg() = delete;
	NewDlg(const NewDlg&) = delete;
	NewDlg& operator=(const NewDlg&) = delete;
};
