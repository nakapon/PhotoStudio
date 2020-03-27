#include <Platform.h>

#include <GL/gl.h>
PRAGMA_LINK_LIBRARY("opengl32.lib")

#include <imgui/imgui.h>

#include <imgui/PlatformOpenGL2.h>

#include <FontUtilities.h>

using namespace ImGui;

static bool SetupPixelFormat(HDC hDC);

static void SetupRenderState(ImDrawData* draw_data, int fb_width, int fb_height);
static void RenderDrawData(ImDrawData* draw_data);

CPlatformOpenGL2::CPlatformOpenGL2()
	: m_hWindow(nullptr)
	, m_hRC(nullptr)
	, m_TicksPerSecond(0)
	, m_Time(0)
	, m_pContext(nullptr)
	, m_FontTexture(0)
	, m_IsRendering(false)
{
}

CPlatformOpenGL2::~CPlatformOpenGL2()
{
	this->Destroy();
}

void CPlatformOpenGL2::Create(HWND hWindow, LPCTSTR pszFontName, UInt32 FontSize, LPCTSTR pszIniFilePath)
{
	this->m_hWindow = hWindow;

	::QueryPerformanceFrequency((LARGE_INTEGER *)&this->m_TicksPerSecond);
	::QueryPerformanceCounter((LARGE_INTEGER *)&this->m_Time);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();

	this->m_pContext = ImGui::CreateContext();

	this->MakeCurrentImGui();

	ImGuiIO& io = ImGui::GetIO();
//	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
//	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	this->InitializeGL();

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImplXXXX_NewFrame below will call.
	// - Read 'docs/FONTS.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);
	if(pszFontName != nullptr && FontSize != 0)
	{
		TCHAR szFontPath[MAX_PATH] = { 0 };

		if(FontUtilities::GetSystemFontPath(pszFontName, szFontPath, sizeof(szFontPath) / sizeof(szFontPath[0])))
		{
			io.Fonts->AddFontFromFileTTF(TSTR_TO_ASTR(szFontPath), (float)FontSize, nullptr, io.Fonts->GetGlyphRangesJapanese());
		}
	}

	// Setup back-end capabilities flags
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // We can honor GetMouseCursor() values (optional)
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;  // We can honor io.WantSetMousePos requests (optional, rarely used)
	io.BackendPlatformName = "Win32/OpenGL2";
	io.ImeWindowHandle = this->m_hWindow;

	// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array that we will update during the application lifetime.
	io.KeyMap[ImGuiKey_Tab]         = VK_TAB;
	io.KeyMap[ImGuiKey_LeftArrow]   = VK_LEFT;
	io.KeyMap[ImGuiKey_RightArrow]  = VK_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow]     = VK_UP;
	io.KeyMap[ImGuiKey_DownArrow]   = VK_DOWN;
	io.KeyMap[ImGuiKey_PageUp]      = VK_PRIOR;
	io.KeyMap[ImGuiKey_PageDown]    = VK_NEXT;
	io.KeyMap[ImGuiKey_Home]        = VK_HOME;
	io.KeyMap[ImGuiKey_End]         = VK_END;
	io.KeyMap[ImGuiKey_Insert]      = VK_INSERT;
	io.KeyMap[ImGuiKey_Delete]      = VK_DELETE;
	io.KeyMap[ImGuiKey_Backspace]   = VK_BACK;
	io.KeyMap[ImGuiKey_Space]       = VK_SPACE;
	io.KeyMap[ImGuiKey_Enter]       = VK_RETURN;
	io.KeyMap[ImGuiKey_Escape]      = VK_ESCAPE;
	io.KeyMap[ImGuiKey_KeyPadEnter] = VK_RETURN;
	io.KeyMap[ImGuiKey_A]           = 'A';
	io.KeyMap[ImGuiKey_C]           = 'C';
	io.KeyMap[ImGuiKey_V]           = 'V';
	io.KeyMap[ImGuiKey_X]           = 'X';
	io.KeyMap[ImGuiKey_Y]           = 'Y';
	io.KeyMap[ImGuiKey_Z]           = 'Z';

	// Ini file path
	if(pszIniFilePath != nullptr)
	{
		PFStringA::Copy(this->m_IniFilePath, TSTR_TO_ASTR(pszIniFilePath));
		io.IniFilename = (LPCSTR)this->m_IniFilePath;
	}
}

void CPlatformOpenGL2::Destroy()
{
	if(this->m_hRC == nullptr)
		return;

	this->MakeCurrentGL();

	this->MakeCurrentImGui();

	this->DestroyFontTexture();

	ImGui::DestroyContext((ImGuiContext*)this->m_pContext);
	this->m_pContext = nullptr;

	this->UninitializeGL();

	this->m_hWindow = nullptr;
}

void CPlatformOpenGL2::MakeCurrentGL()
{
	if(this->m_hWindow != nullptr)
	{
		HDC hDC = GetDC(this->m_hWindow);

		::wglMakeCurrent(hDC, this->m_hRC);

		ReleaseDC(this->m_hWindow, hDC);
	}
}

void CPlatformOpenGL2::ReleaseCurrentGL()
{
	if(this->m_hWindow != nullptr)
	{
		HDC hDC = GetDC(this->m_hWindow);

		::wglMakeCurrent(hDC, nullptr);

		ReleaseDC(this->m_hWindow, hDC);
	}
}

void CPlatformOpenGL2::MakeCurrentImGui()
{
	if(this->m_pContext != nullptr)
	{
		ImGui::SetCurrentContext((ImGuiContext*)this->m_pContext);
	}
}

bool CPlatformOpenGL2::BeginRender(RECT Viewport, COLORREF ClearColor)
{
	if(this->m_hRC == nullptr)
		return false;

	if(this->m_IsRendering)
		return false;

	this->m_IsRendering = true;

	this->m_Viewport = Viewport;
	this->m_ClearColor = ClearColor;

	// Make current (ImGui)
	this->MakeCurrentImGui();

	ImGuiIO& io = ImGui::GetIO();

	// Setup display size
	io.DisplaySize.x = (float)(Viewport.right - Viewport.left);
	io.DisplaySize.y = (float)(Viewport.bottom - Viewport.top);

	// Setup modifier key states
	this->UpdateKeyboardMods();

	// Setup time step
	{
		Int64 CurrentTime;
		::QueryPerformanceCounter((LARGE_INTEGER *)&CurrentTime);
		io.DeltaTime = (float)(CurrentTime - this->m_Time) / this->m_TicksPerSecond;
		this->m_Time = CurrentTime;
	}

	// Make current (OpenGL)
	this->MakeCurrentGL();

	// Start the ImGui frame
	if(this->m_FontTexture == 0)
	{
		this->CreateFontTexture();
	}

	ImGui::NewFrame();

	return true;
}

void CPlatformOpenGL2::EndRender()
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::Render();

	glViewport(this->m_Viewport.left, this->m_Viewport.top, this->m_Viewport.right - this->m_Viewport.left, this->m_Viewport.bottom - this->m_Viewport.top);

	glClearColor(GetRValue(this->m_ClearColor) / 255.0f, GetRValue(this->m_ClearColor) / 255.0f, GetRValue(this->m_ClearColor) / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

#if 0
	// You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
	glUseProgram(0);
#endif
	RenderDrawData(ImGui::GetDrawData());

	// Sync
	glFinish();

	this->SwapBufferGL();

	this->ReleaseCurrentGL();

	this->m_IsRendering = false;
}

bool CPlatformOpenGL2::InitializeGL()
{
	bool bReturn;

	Int32 nReturn;

	HGLRC hRC = nullptr;

	HDC hDC = GetDC(this->m_hWindow);

	nReturn = ::SetupPixelFormat(hDC);
	if(nReturn < 1)
		goto _Error;

	hRC = ::wglCreateContext(hDC);
	if(hRC == nullptr)
	{
		bReturn = false;
		goto _Error;
	}

	if(!::wglMakeCurrent(hDC, hRC))
	{
		bReturn = false;
		goto _Error;
	}

	this->m_hRC = hRC;

	::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	::glClearDepth(1.0f);
	::glEnable(GL_DEPTH_TEST);

	ReleaseDC(this->m_hWindow, hDC);

	return true;

_Error:

	if(hRC != nullptr)
	{
		::wglDeleteContext(hRC);
		hRC = nullptr;
	}

	ReleaseDC(this->m_hWindow, hDC);

	return false;
}

void CPlatformOpenGL2::UninitializeGL()
{
	wglMakeCurrent(nullptr, nullptr);

	if(this->m_hRC != nullptr)
	{
		::wglDeleteContext(this->m_hRC);
		this->m_hRC = nullptr;
	}
}

void CPlatformOpenGL2::CreateFontTexture()
{
	// Build texture atlas
	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bit (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

	// Upload texture to graphics system
	GLint last_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	glGenTextures(1, &this->m_FontTexture);
	glBindTexture(GL_TEXTURE_2D, this->m_FontTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	// Store our identifier
	io.Fonts->TexID = (ImTextureID)(intptr_t)this->m_FontTexture;

	// Restore state
	glBindTexture(GL_TEXTURE_2D, last_texture);
}

void CPlatformOpenGL2::DestroyFontTexture()
{
	if(this->m_FontTexture != 0)
	{
		ImGuiIO& io = ImGui::GetIO();
		glDeleteTextures(1, &this->m_FontTexture);
		io.Fonts->TexID = 0;
		this->m_FontTexture = 0;
	}
}

void CPlatformOpenGL2::SwapBufferGL()
{
	HDC hDC = GetDC(this->m_hWindow);

	::SwapBuffers(hDC);

	ReleaseDC(this->m_hWindow, hDC);
}

void CPlatformOpenGL2::UpdateKeyboardMods()
{
	ImGuiIO& io = ImGui::GetIO();

	io.KeyCtrl = (::GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
	io.KeyShift = (::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
	io.KeyAlt = (::GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
}

void CPlatformOpenGL2::UpdateMouseCursor()
{
	ImGuiIO& io = ImGui::GetIO();

	if(io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
		return;

	ImGuiMouseCursor MouseCursor = ImGui::GetMouseCursor();
	if(MouseCursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
	{
		// Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
		::SetCursor(nullptr);
	}
	else
	{
		LPTSTR Cursor = IDC_ARROW;

		switch(MouseCursor)
		{
		case ImGuiMouseCursor_Arrow:      Cursor = IDC_ARROW; break;
		case ImGuiMouseCursor_TextInput:  Cursor = IDC_IBEAM; break;
		case ImGuiMouseCursor_ResizeAll:  Cursor = IDC_SIZEALL; break;
		case ImGuiMouseCursor_ResizeEW:   Cursor = IDC_SIZEWE; break;
		case ImGuiMouseCursor_ResizeNS:   Cursor = IDC_SIZENS; break;
		case ImGuiMouseCursor_ResizeNESW: Cursor = IDC_SIZENESW; break;
		case ImGuiMouseCursor_ResizeNWSE: Cursor = IDC_SIZENWSE; break;
		case ImGuiMouseCursor_Hand:       Cursor = IDC_HAND; break;
		}

		::SetCursor(::LoadCursor(nullptr, Cursor));
	}
}

void CPlatformOpenGL2::OnKeyDown(UInt32 Char)
{
	this->MakeCurrentImGui();

	ImGuiIO& io = ImGui::GetIO();

	if(Char < 256)
	{
		io.KeysDown[Char] = true;
	}
}

void CPlatformOpenGL2::OnKeyUp(UInt32 Char)
{
	this->MakeCurrentImGui();

	ImGuiIO& io = ImGui::GetIO();

	if(Char < 256)
	{
		io.KeysDown[Char] = false;
	}
}

void CPlatformOpenGL2::OnChar(UInt32 Char)
{
	this->MakeCurrentImGui();

	ImGuiIO& io = ImGui::GetIO();

	io.AddInputCharacter(Char);
}

void CPlatformOpenGL2::OnMouseButton(UInt32 Message)
{
	bool IsDown = true;

	Int32 Button = -1;

	if(Message == WM_LBUTTONDOWN || Message == WM_LBUTTONDBLCLK){ Button = 0; IsDown = true; }
	if(Message == WM_RBUTTONDOWN || Message == WM_RBUTTONDBLCLK){ Button = 1; IsDown = true; }

	if(Message == WM_LBUTTONUP){ Button = 0; IsDown = false; }
	if(Message == WM_RBUTTONUP){ Button = 1; IsDown = false; }

	if(Button < 0)
		return;

	this->MakeCurrentImGui();

	ImGuiIO& io = ImGui::GetIO();

	if(IsDown)
	{
		if(!ImGui::IsAnyMouseDown() && ::GetCapture() == NULL)
		{
			::SetCapture(this->m_hWindow);
		}

		io.MouseDown[Button] = true;
	}
	else
	{
		io.MouseDown[Button] = false;

		if(!ImGui::IsAnyMouseDown() && ::GetCapture() == this->m_hWindow)
		{
			::ReleaseCapture();
		}
	}
}

void CPlatformOpenGL2::OnMouseMove(POINT Point)
{
	this->MakeCurrentImGui();

	ImGuiIO& io = ImGui::GetIO();

	io.MousePos = ImVec2((float)Point.x, (float)Point.y);

	this->UpdateMouseCursor();
}

void CPlatformOpenGL2::OnMouseWheel(Int32 Delta)
{
	this->MakeCurrentImGui();

	ImGuiIO& io = ImGui::GetIO();

	io.MouseWheel += (float)Delta / WHEEL_DELTA;
}

static bool SetupPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR PixelFormatDescriptor = { };

	Int32 nPixelFormat;

	PixelFormatDescriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	PixelFormatDescriptor.nVersion = 1;
	PixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	PixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
	PixelFormatDescriptor.cColorBits = 32;
	PixelFormatDescriptor.cRedBits = 0;
	PixelFormatDescriptor.cRedShift = 0;
	PixelFormatDescriptor.cGreenBits = 0;
	PixelFormatDescriptor.cGreenShift = 0;
	PixelFormatDescriptor.cBlueBits = 0;
	PixelFormatDescriptor.cBlueShift = 0;
	PixelFormatDescriptor.cAlphaBits = 0;
	PixelFormatDescriptor.cAlphaShift = 0;
	PixelFormatDescriptor.cAccumBits = 0;
	PixelFormatDescriptor.cAccumRedBits = 0;
	PixelFormatDescriptor.cAccumGreenBits = 0;
	PixelFormatDescriptor.cAccumBlueBits = 0;
	PixelFormatDescriptor.cAccumAlphaBits = 0;
	PixelFormatDescriptor.cDepthBits = 0;
	PixelFormatDescriptor.cStencilBits = 0;
	PixelFormatDescriptor.cAuxBuffers = 0;
	PixelFormatDescriptor.iLayerType = PFD_MAIN_PLANE;
	PixelFormatDescriptor.bReserved = 0;
	PixelFormatDescriptor.dwLayerMask = 0;
	PixelFormatDescriptor.dwVisibleMask = 0;
	PixelFormatDescriptor.dwDamageMask = 0;

	nPixelFormat = ::ChoosePixelFormat(hDC, &PixelFormatDescriptor);
	if(0 == nPixelFormat)
		return false;

	if(!::SetPixelFormat(hDC, nPixelFormat, &PixelFormatDescriptor))
		return false;

	return true;
}

static void SetupRenderState(ImDrawData* draw_data, int fb_width, int fb_height)
{
	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, vertex/texcoord/color pointers, polygon fill.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_SCISSOR_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// If you are using this code with non-legacy OpenGL header/contexts (which you should not, prefer using imgui_impl_opengl3.cpp!!),
	// you may need to backup/reset/restore current shader using the lines below. DO NOT MODIFY THIS FILE! Add the code in your calling function:
	//  GLint last_program;
	//  glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
	//  glUseProgram(0);
	//  ImplOpenGL2_RenderDrawData(...);
	//  glUseProgram(last_program)

	// Setup viewport, orthographic projection matrix
	// Our visible imgui space lies from draw_data->DisplayPos (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayPos is (0,0) for single viewport apps.
	glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(draw_data->DisplayPos.x, draw_data->DisplayPos.x + draw_data->DisplaySize.x, draw_data->DisplayPos.y + draw_data->DisplaySize.y, draw_data->DisplayPos.y, -1.0f, +1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

// OpenGL2 Render function.
// (this used to be set in io.RenderDrawListsFn and called by ImGui::Render(), but you can now call this directly from your main loop)
// Note that this implementation is little overcomplicated because we are saving/setting up/restoring every OpenGL state explicitly, in order to be able to run within any OpenGL engine that doesn't do so.
static void RenderDrawData(ImDrawData* draw_data)
{
	// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
	int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
	int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
	if (fb_width == 0 || fb_height == 0)
		return;

	// Backup GL state
	GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	GLint last_polygon_mode[2]; glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
	GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
	GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);

	// Setup desired GL state
	SetupRenderState(draw_data, fb_width, fb_height);

	// Will project scissor/clipping rectangles into framebuffer space
	ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
	ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

	// Render command lists
	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;
		const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;
		glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + IM_OFFSETOF(ImDrawVert, pos)));
		glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + IM_OFFSETOF(ImDrawVert, uv)));
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + IM_OFFSETOF(ImDrawVert, col)));

		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback)
			{
				// User callback, registered via ImDrawList::AddCallback()
				// (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
				if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
					SetupRenderState(draw_data, fb_width, fb_height);
				else
					pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				// Project scissor/clipping rectangles into framebuffer space
				ImVec4 clip_rect;
				clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
				clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
				clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
				clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

				if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
				{
					// Apply scissor/clipping rectangle
					glScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));

					// Bind texture, Draw
					glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
					glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer);
				}
			}
			idx_buffer += pcmd->ElemCount;
		}
	}

	// Restore modified GL state
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindTexture(GL_TEXTURE_2D, (GLuint)last_texture);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
	glPolygonMode(GL_FRONT, (GLenum)last_polygon_mode[0]); glPolygonMode(GL_BACK, (GLenum)last_polygon_mode[1]);
	glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
	glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}
