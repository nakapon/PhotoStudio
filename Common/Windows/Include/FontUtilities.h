#pragma once

class FontUtilities
{
public:

	static bool IsSystemFont(LPCTSTR pszFaceName);

	static bool GetSystemFontPath(LPCTSTR pszFaceName, LPTSTR pszFontPath, UInt32 MaxLength);

private:
	FontUtilities() = delete;
	~FontUtilities() = delete;
	FontUtilities(const FontUtilities&) = delete;
	FontUtilities& operator=(const FontUtilities&) = delete;
};
