#include <Platform.h>

#include <vector>

#include <FontUtilities.h>

static int CALLBACK EnumFontProc(const LOGFONT* pLogicalFont, const TEXTMETRIC* pTextMetric, DWORD FontType, LPARAM lParam);

bool FontUtilities::IsSystemFont(LPCTSTR pszFaceName)
{
	Int32 nReturn;

	if(pszFaceName == nullptr)
		return false;

	{
		HDC hDC = ::GetDC(nullptr);

		LOGFONT LogicalFont = { 0 };

		LogicalFont.lfCharSet = DEFAULT_CHARSET;
		PFString::Copy(LogicalFont.lfFaceName, pszFaceName);

		nReturn = ::EnumFontFamiliesEx(hDC, &LogicalFont, EnumFontProc, (LPARAM)pszFaceName, 0);

		::ReleaseDC(nullptr, hDC);
	}

	return nReturn == 0;
}

static int CALLBACK EnumFontProc(const LOGFONT* pLogicalFont, const TEXTMETRIC* pTextMetric, DWORD FontType, LPARAM lParam)
{
	LPCTSTR pszTargetFaceName = (LPCTSTR)lParam;

	if(PFString::Compare(pszTargetFaceName, pLogicalFont->lfFaceName, PFStringCaseSens::Ignore) == 0)
		return 0;

	return 1;
}

bool FontUtilities::GetSystemFontPath(LPCTSTR pszFaceName, LPTSTR pszFontPath, UInt32 MaxLength)
{
	bool bReturn;

	LONG lResult;

	HKEY hKey;

	TCHAR szFileName[MAX_PATH] = { 0 };

	DWORD MaxValueNameSize, MaxValueDataSize;

	DWORD ValueIndex = 0;
	std::vector<TCHAR> ValueName;
	std::vector<BYTE> ValueData;

	if(pszFaceName == nullptr || pszFontPath == nullptr || MaxLength == 0)
		return false;

	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts"), 0, KEY_READ, &hKey);
	if(lResult != ERROR_SUCCESS)
		return false;

	lResult = RegQueryInfoKey(hKey, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &MaxValueNameSize, &MaxValueDataSize, nullptr, nullptr);
	if(lResult != ERROR_SUCCESS)
	{
		bReturn = false;
		goto _Return;
	}

	ValueName.resize(MaxValueNameSize);
	ValueData.resize(MaxValueDataSize);

	do
	{
		DWORD ValueNameSize = MaxValueNameSize;
		DWORD ValueDataSize = MaxValueDataSize;
		DWORD ValueType;

		memset(&ValueName[0], 0, sizeof(TCHAR) * MaxValueNameSize);
		memset(&ValueData[0], 0, sizeof(BYTE) * MaxValueDataSize);

		lResult = RegEnumValue(hKey, ValueIndex, &ValueName[0], &ValueNameSize, 0, &ValueType, &ValueData[0], &ValueDataSize);

		ValueIndex++;

		if(lResult != ERROR_SUCCESS || ValueType != REG_SZ)
		{
			continue;
		}

		if(PFString::Strstr(&ValueName[0], pszFaceName) != nullptr)
		{
			PFString::Copy(szFileName, (LPCTSTR)&ValueData[0]);
			break;
		}
	}
	while(lResult != ERROR_NO_MORE_ITEMS);

	bReturn = szFileName[0] != _T('\0') ? true : false;

_Return:

	RegCloseKey(hKey);

	if(bReturn)
	{
		TCHAR szFilePath[MAX_PATH] = { 0 };

		GetWindowsDirectory(szFilePath, sizeof(szFilePath) / sizeof(szFilePath[0]));
		PFString::Append(szFilePath, TEXT("\\Fonts\\"));
		PFString::Append(szFilePath, szFileName);

		PFString::Copy(pszFontPath, MaxLength, szFilePath);
	}

	return bReturn;
}
