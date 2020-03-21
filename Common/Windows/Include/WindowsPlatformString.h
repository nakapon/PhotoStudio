#pragma once

template <typename T>
struct TWindowsPlatformString /* : public TGenericPlatformString<T> */
{
	// MultiByteToWideChar
	static Int32 GetWideCharLength(LPCSTR pszSrc)
	{
		if(pszSrc == nullptr)
			return 0;

		return ::MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, nullptr, 0);
	}

	static Int32 MultiByteToWideChar(LPWSTR pszDst, UInt32 uiMaxLength, LPCSTR pszSrc)
	{
		if(pszDst == nullptr || uiMaxLength == 0 || pszSrc == nullptr)
			return 0;

		return ::MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pszDst, uiMaxLength);
	}

	// WideCharToMultiByte
	static Int32 GetMultiByteLength(LPCWSTR pszSrc)
	{
		if(pszSrc == nullptr)
			return 0;

		return ::WideCharToMultiByte(CP_ACP, 0, pszSrc, -1, nullptr, 0, nullptr, nullptr);
	}

	static Int32 WideCharToMultiByte(LPSTR pszDst, UInt32 uiMaxLength, LPCWSTR pszSrc)
	{
		if(pszDst == nullptr || uiMaxLength == 0 || pszSrc == nullptr)
			return 0;

		return ::WideCharToMultiByte(CP_ACP, 0, pszSrc, -1, pszDst, uiMaxLength, nullptr, nullptr);
	}

	static inline T* Strtok(T* pszString, const T* pszDelim, T** ppszContext);
};

template <typename T>
using PFStringT = TWindowsPlatformString<T>;

typedef TWindowsPlatformString< CHAR> PFStringA;
typedef TWindowsPlatformString<WCHAR> PFStringW;
typedef TWindowsPlatformString<TCHAR> PFString;
