#pragma once

template <typename T>
struct TWindowsPlatformPath : public TGenericPlatformPath<T>
{
	static inline bool GetModuleFilePath(HMODULE hModule, T* pszPath, UInt32 MaxLength);

	template <UInt32 MaxLength>
	static bool GetModuleFilePath(HMODULE hModule, T (&pszPath)[MaxLength])
	{
		return GetModuleFilePath(hModule, pszPath, MaxLength);
	}

	static inline bool GetModuleDirPath(HMODULE hModule, T* pszPath, UInt32 MaxLength);

	template <UInt32 MaxLength>
	static bool GetModuleDirPath(HMODULE hModule, T (&pszPath)[MaxLength])
	{
		return GetModuleDirPath(hModule, pszPath, MaxLength);
	}
};

// GetModuleFilePath
template <>
inline bool TWindowsPlatformPath<CHAR>::GetModuleFilePath(HMODULE hModule, LPSTR pszPath, UInt32 MaxLength)
{
	if(pszPath == nullptr || MaxLength == 0)
		return false;

	pszPath[0] = '\0';

	if(::GetModuleFileNameA(hModule, pszPath, MaxLength) == 0)
		return false;

//	TWindowsPlatformPath<CHAR>::Canonicalize(pszPath);

	return true;
}

template <>
inline bool TWindowsPlatformPath<WCHAR>::GetModuleFilePath(HMODULE hModule, LPWSTR pszPath, UInt32 MaxLength)
{
	if(pszPath == nullptr || MaxLength == 0)
		return false;

	pszPath[0] = L'\0';

	if(::GetModuleFileNameW(hModule, pszPath, MaxLength) == 0)
		return false;

//	TWindowsPlatformPath<WCHAR>::Canonicalize(pszPath);

	return true;
}

// GetModuleDirPath
template <>
inline bool TWindowsPlatformPath<CHAR>::GetModuleDirPath(HMODULE hModule, LPSTR pszPath, UInt32 MaxLength)
{
	LPSTR pszPointer = nullptr;

	if(pszPath == nullptr || MaxLength == 0)
		return false;

	pszPath[0] = '\0';

	if(TWindowsPlatformPath<CHAR>::GetModuleFilePath(hModule, pszPath, MaxLength) == 0)
		return false;

	pszPointer = PFStringA::Strrchr(pszPath, '\\');
	if(pszPointer == nullptr)
		return false;

	pszPointer[0] = '\0';

	return true;
}

template <>
inline bool TWindowsPlatformPath<WCHAR>::GetModuleDirPath(HMODULE hModule, LPWSTR pszPath, UInt32 MaxLength)
{
	LPWSTR pszPointer = nullptr;

	if(pszPath == nullptr || MaxLength == 0)
		return false;

	pszPath[0] = L'\0';

	if(TWindowsPlatformPath<WCHAR>::GetModuleFilePath(hModule, pszPath, MaxLength) == 0)
		return false;

	pszPointer = PFStringW::Strrchr(pszPath, L'\\');
	if(pszPointer == nullptr)
		return false;

	pszPointer[0] = L'\0';

	return true;
}

template <typename T>
using PFPathT = TWindowsPlatformPath<T>;

typedef TWindowsPlatformPath< CHAR> PFPathA;
typedef TWindowsPlatformPath<WCHAR> PFPathW;
typedef TWindowsPlatformPath<TCHAR> PFPath;
