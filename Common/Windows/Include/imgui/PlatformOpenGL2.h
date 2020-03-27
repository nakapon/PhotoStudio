#pragma once

namespace ImGui
{

class CPlatformOpenGL2
{
public:
	CPlatformOpenGL2();
	~CPlatformOpenGL2();

	void Create(HWND hWindow, LPCTSTR pszFontName = nullptr, UInt32 FontSize = 0, LPCTSTR pszIniFilePath = nullptr);
	void Destroy();

	// Note: OpenGL API���g���ꍇ�͏����� MakeCurrentGL / ReleaseCurrentGL �ň͂ނ���
	void MakeCurrentGL();
	void ReleaseCurrentGL();

	// Note: ImGui API���g���ꍇ�͎��O�ɌĂԂ���
	void MakeCurrentImGui();

	// Note: BeginRender �� true ��Ԃ����ꍇ�̂ݕ`�揈���y�� EndRender �̌Ăяo�������邱��
	bool BeginRender(RECT Viewport, COLORREF ClearColor);
	void EndRender();

	// Window Events
	void OnKeyDown(UInt32 Char);
	void OnKeyUp(UInt32 Char);
	void OnChar(UInt32 Char);

	void OnMouseButton(UInt32 Message);
	void OnMouseMove(POINT Point);
	void OnMouseWheel(Int32 Delta);

protected:
	HWND m_hWindow;
	HGLRC m_hRC;

	Int64 m_TicksPerSecond;
	Int64 m_Time;

	void* m_pContext;

	UInt32 m_FontTexture;

	bool m_IsRendering;

	RECT m_Viewport;
	COLORREF m_ClearColor;

	CHAR m_IniFilePath[MAX_PATH];

protected:
	bool InitializeGL();
	void UninitializeGL();

	void SwapBufferGL();

	void CreateFontTexture();
	void DestroyFontTexture();

	void UpdateKeyboardMods();
	void UpdateMouseCursor();

private:
	CPlatformOpenGL2(const CPlatformOpenGL2&);
	CPlatformOpenGL2& operator=(const CPlatformOpenGL2&);
};

} // namespace ImGui
