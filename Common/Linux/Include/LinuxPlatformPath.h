#pragma once

#include <unistd.h>

#include <GenericPlatformPath.h>

template <typename T>
struct TLinuxPlatfornPath : public TGenericPlatformPath<T>
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
inline bool TLinuxPlatfornPath<CHAR>::GetModuleFilePath(HMODULE hModule, LPSTR pszPath, UInt32 MaxLength)
{
	INT nReturn;

	CHAR szPath[PATH_MAX] = { 0 };

	if(pszPath == nullptr || MaxLength == 0)
		return false;

	nReturn = readlink("/proc/self/exe", szPath, sizeof(szPath) / sizeof(szPath[0]) - 1);
	if(nReturn < 0)
		return false;

	PFStringA::Copy(pszPath, MaxLength, szPath);

	return true;
}

template <>
inline bool TLinuxPlatfornPath<WCHAR>::GetModuleFilePath(HMODULE hModule, LPWSTR pszPath, UInt32 MaxLength)
{
	INT nReturn;

	CHAR szPath[PATH_MAX] = { 0 };

	if(pszPath == nullptr || MaxLength == 0)
		return false;

	nReturn = readlink("/proc/self/exe", szPath, sizeof(szPath) / sizeof(szPath[0]) - 1);
	if(nReturn < 0)
		return false;

	PFStringW::Copy(pszPath, MaxLength, ASTR_TO_WSTR(szPath));

	return true;
}

// GetModuleDirPath
template <>
inline bool TLinuxPlatfornPath<CHAR>::GetModuleDirPath(HMODULE hModule, LPSTR pszPath, UInt32 MaxLength)
{
	LPSTR pszPointer = nullptr;

	if(pszPath == nullptr || MaxLength == 0)
		return false;

	pszPath[0] = '\0';

	if(TLinuxPlatfornPath<CHAR>::GetModuleFilePath(hModule, pszPath, MaxLength) == 0)
		return false;

	pszPointer = PFStringA::Strrchr(pszPath, '/');
	if(pszPointer == nullptr)
		return false;

	pszPointer[0] = '\0';

	return true;
}

template <>
inline bool TLinuxPlatfornPath<WCHAR>::GetModuleDirPath(HMODULE hModule, LPWSTR pszPath, UInt32 MaxLength)
{
	LPWSTR pszPointer = nullptr;

	if(pszPath == nullptr || MaxLength == 0)
		return false;

	pszPath[0] = L'\0';

	if(TLinuxPlatfornPath<WCHAR>::GetModuleFilePath(hModule, pszPath, MaxLength) == 0)
		return false;

	pszPointer = PFStringW::Strrchr(pszPath, L'/');
	if(pszPointer == nullptr)
		return false;

	pszPointer[0] = L'\0';

	return true;
}

template <typename T>
using PFPathT = TLinuxPlatfornPath<T>;

typedef TLinuxPlatfornPath< CHAR> PFPathA;
typedef TLinuxPlatfornPath<WCHAR> PFPathW;
typedef TLinuxPlatfornPath<TCHAR> PFPath;
