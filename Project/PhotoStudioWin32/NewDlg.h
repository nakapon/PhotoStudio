#pragma once

#include <IImageData.h>

class NewDlg
{
public:

	// �u�V�K�쐬�v���[�_���_�C�A���O
	static bool DoModal(HINSTANCE hInstance, HWND hWindow, IImageData* pImageData);

private:
	NewDlg() = delete;
	~NewDlg() = delete;
	NewDlg(const NewDlg&) = delete;
	NewDlg& operator=(const NewDlg&) = delete;
};
