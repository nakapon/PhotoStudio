#include <Platform.h>

#include <imgui/imgui.h>

#include <imgui/StyleLibrary.h>

using namespace ImGui;

void StyleLibrary::PushWindowStyles()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,        ImVec2(4.0f, 4.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,       0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,         ImVec2(2.0f, 2.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,          ImVec2(2.0f, 2.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding,    0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_TabRounding,          0.0f);
}

void StyleLibrary::ApplyDarkColors()
{
	ImVec4* pColors = ImGui::GetStyle().Colors;

	pColors[ImGuiCol_Text]                   = ImColor(IM_COL32(225, 225, 225, 255));
	pColors[ImGuiCol_TextDisabled]           = ImColor(IM_COL32(128, 128, 128, 255));
	pColors[ImGuiCol_WindowBg]               = ImColor(IM_COL32( 30,  30,  30, 255));	// Background of normal windows
	pColors[ImGuiCol_ChildBg]                = ImColor(IM_COL32(  0,   0,   0,   0));	// Background of child windows (Disable)
	pColors[ImGuiCol_PopupBg]                = ImColor(IM_COL32( 27,  27,  28, 230));	// Background of popups, menus, tooltips windows
	pColors[ImGuiCol_Border]                 = ImColor(IM_COL32( 63,  63,  70, 255));
	pColors[ImGuiCol_BorderShadow]           = ImColor(IM_COL32(  0,   0,   0,   0));	// (Disable)
	pColors[ImGuiCol_FrameBg]                = ImColor(IM_COL32( 37,  37,  38, 255));	// Background of checkbox, radio button, plot, slider, text input
	pColors[ImGuiCol_FrameBgHovered]         = ImColor(IM_COL32( 63,  63,  70, 128));
	pColors[ImGuiCol_FrameBgActive]          = ImColor(IM_COL32( 66, 150, 250, 171));
#if 0
	pColors[ImGuiCol_TitleBg]                = ImColor(IM_COL32( 45,  45,  48, 255));
	pColors[ImGuiCol_TitleBgActive]          = ImColor(IM_COL32(  0, 122, 204, 255));
#else
	pColors[ImGuiCol_TitleBg]                = ImColor(IM_COL32( 63,  63,  70, 255));
	pColors[ImGuiCol_TitleBgActive]          = ImColor(IM_COL32( 80,  80,  88, 255));
#endif
	pColors[ImGuiCol_SliderGrab]             = ImColor(IM_COL32( 79,  79,  79, 255));
	pColors[ImGuiCol_SliderGrabActive]       = ImColor(IM_COL32(130, 130, 130, 255));
	pColors[ImGuiCol_Button]                 = ImColor(IM_COL32( 45,  45,  48, 255));
	pColors[ImGuiCol_ButtonHovered]          = ImColor(IM_COL32( 63,  63,  70, 255));
#if 0
	pColors[ImGuiCol_Header]                 = ImColor(IM_COL32( 63,  63,  70, 255));	// Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
	pColors[ImGuiCol_HeaderHovered]          = ImColor(IM_COL32( 70,  70,  80, 255));
	pColors[ImGuiCol_HeaderActive]           = ImColor(IM_COL32( 80,  80,  88, 255));
#else
	pColors[ImGuiCol_Header]                 = ImColor(IM_COL32( 45,  45,  48, 255));	// Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
	pColors[ImGuiCol_HeaderHovered]          = ImColor(IM_COL32( 63,  63,  70, 255));
	pColors[ImGuiCol_HeaderActive]           = ImColor(IM_COL32( 80,  80,  88, 255));
#endif
	pColors[ImGuiCol_Tab]                    = ImColor(IM_COL32( 45,  45,  48, 255));
	pColors[ImGuiCol_TabHovered]             = ImColor(IM_COL32( 28, 151, 234, 255));
	pColors[ImGuiCol_TabActive]              = ImColor(IM_COL32(  0, 122, 204, 255));
}
