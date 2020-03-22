#pragma once

#include <locale.h>

template <typename T>
struct TUnixPlatformString : public TGenericPlatformString<T>
{
	// MultiByteToWideChar
	static Int32 GetWideCharLength(LPCSTR pszSrc)
	{
		if(pszSrc == nullptr)
			return 0;

		CHAR szPrevLocale[128] = { 0 };
		PFStringA::Copy(szPrevLocale, sizeof(szPrevLocale) / sizeof(szPrevLocale[0]), ::setlocale(LC_CTYPE, nullptr));
		::setlocale(LC_CTYPE, "");

		SIZE_T Length = ::mbstowcs(nullptr, pszSrc, 0);

		::setlocale(LC_CTYPE, szPrevLocale);

		if(Length == (SIZE_T)-1)
			return 0;

		return (Int32)Length + 1;
	}

	static Int32 MultiByteToWideChar(LPWSTR pszDst, UInt32 MaxLength, LPCSTR pszSrc)
	{
		if(pszSrc == nullptr)
			return 0;

		CHAR szPrevLocale[128] = { 0 };
		PFStringA::Copy(szPrevLocale, sizeof(szPrevLocale) / sizeof(szPrevLocale[0]), ::setlocale(LC_CTYPE, nullptr));
		::setlocale(LC_CTYPE, "");

		SIZE_T Length = ::mbstowcs(pszDst, pszSrc, PFStringA::Length(pszSrc) + 1);

		::setlocale(LC_CTYPE, szPrevLocale);

		if(Length == (SIZE_T)-1)
			return 0;

		return (Int32)Length + 1;
	}

	// WideCharToMultiByte
	static Int32 GetMultiByteLength(LPCWSTR pszSrc)
	{
		if(pszSrc == nullptr)
			return 0;

		CHAR szPrevLocale[128] = { 0 };
		PFStringA::Copy(szPrevLocale, sizeof(szPrevLocale) / sizeof(szPrevLocale[0]), ::setlocale(LC_CTYPE, nullptr));
		::setlocale(LC_CTYPE, "");

		SIZE_T Length = ::wcstombs(nullptr, pszSrc, 0);

		::setlocale(LC_CTYPE, szPrevLocale);

		if(Length == (SIZE_T)-1)
			return 0;

		return (Int32)Length + 1;
	}

	static Int32 WideCharToMultiByte(LPSTR pszDst, UInt32 MaxLength, LPCWSTR pszSrc)
	{
		if(pszSrc == nullptr)
			return 0;

		CHAR szPrevLocale[128] = { 0 };
		PFStringA::Copy(szPrevLocale, sizeof(szPrevLocale) / sizeof(szPrevLocale[0]), ::setlocale(LC_CTYPE, nullptr));
		::setlocale(LC_CTYPE, "");

		SIZE_T Length = ::wcstombs(pszDst, pszSrc, MaxLength);

		::setlocale(LC_CTYPE, szPrevLocale);

		if(Length == (SIZE_T)-1)
			return 0;

		return (Int32)Length + 1;
	}
};
