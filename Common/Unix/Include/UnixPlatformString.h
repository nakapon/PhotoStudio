#pragma once

#include <locale.h>

template <typename T>
struct TUnixPlatformString /* : public TGenericPlatformString<T> */
{
	// MultiByteToWideChar
	static Int32 GetWideCharLength(LPCSTR pszSrc)
	{
		if(pszSrc == nullptr)
			return 0;

		CHAR szPrevLocale[128] = { 0 };
		strcpy_s(szPrevLocale, sizeof(szPrevLocale) / sizeof(szPrevLocale[0]), ::setlocale(LC_CTYPE, nullptr));
		::setlocale(LC_CTYPE, "");

		SIZE_T nLength = ::mbstowcs(nullptr, pszSrc, 0);

		::setlocale(LC_CTYPE, szPrevLocale);

		if(nLength == (SIZE_T)-1)
			return 0;

		return (Int32)nLength + 1;
	}

	static Int32 MultiByteToWideChar(LPWSTR pszDst, UInt32 uiMaxLength, LPCSTR pszSrc)
	{
		if(pszSrc == nullptr)
			return 0;

		CHAR szPrevLocale[128] = { 0 };
		strcpy_s(szPrevLocale, sizeof(szPrevLocale) / sizeof(szPrevLocale[0]), ::setlocale(LC_CTYPE, nullptr));
		::setlocale(LC_CTYPE, "");

		SIZE_T nLength = ::mbstowcs(pszDst, pszSrc, ::strlen(pszSrc) + 1);

		::setlocale(LC_CTYPE, szPrevLocale);

		if(nLength == (SIZE_T)-1)
			return 0;

		return (Int32)nLength + 1;
	}

	// WideCharToMultiByte
	static Int32 GetMultiByteLength(LPCWSTR pszSrc)
	{
		if(pszSrc == nullptr)
			return 0;

		CHAR szPrevLocale[128] = { 0 };
		strcpy_s(szPrevLocale, sizeof(szPrevLocale) / sizeof(szPrevLocale[0]), ::setlocale(LC_CTYPE, nullptr));
		::setlocale(LC_CTYPE, "");

		SIZE_T nLength = ::wcstombs(nullptr, pszSrc, 0);

		::setlocale(LC_CTYPE, szPrevLocale);

		if(nLength == (SIZE_T)-1)
			return 0;

		return (Int32)nLength + 1;
	}

	static Int32 WideCharToMultiByte(LPSTR pszDst, UInt32 uiMaxLength, LPCWSTR pszSrc)
	{
		if(pszSrc == nullptr)
			return 0;

		CHAR szPrevLocale[128] = { 0 };
		strcpy_s(szPrevLocale, sizeof(szPrevLocale) / sizeof(szPrevLocale[0]), ::setlocale(LC_CTYPE, nullptr));
		::setlocale(LC_CTYPE, "");

		SIZE_T nLength = ::wcstombs(pszDst, pszSrc, uiMaxLength);

		::setlocale(LC_CTYPE, szPrevLocale);

		if(nLength == (SIZE_T)-1)
			return 0;

		return (Int32)nLength + 1;
	}
};
