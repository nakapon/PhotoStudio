#pragma once

namespace ImGui
{

class StyleLibrary
{
public:

	static void PushWindowStyles();

	static void ApplyDarkColors();

private:
	StyleLibrary() = delete;
	~StyleLibrary() = delete;
	StyleLibrary(const StyleLibrary&) = delete;
	StyleLibrary& operator=(const StyleLibrary&) = delete;
};

} // namespace ImGui
