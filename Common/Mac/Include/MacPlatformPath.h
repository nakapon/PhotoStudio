#pragma once

#include <mach-o/dyld.h>

#include <GenericPlatformPath.h>

template <typename T>
struct TMacPlatfornPath : public TGenericPlatformPath<T>
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
inline bool TMacPlatfornPath<CHAR>::GetModuleFilePath(HMODULE hModule, LPSTR pszPath, UInt32 MaxLength)
{
	INT nReturn;

	CHAR szPath[PATH_MAX] = { 0 };

	if(pszPath == nullptr || MaxLength == 0)
		return false;

	uint32_t Size = (uint32_t)sizeof(szPath);
	nReturn = _NSGetExecutablePath(szPath, &Size);
	if(nReturn != 0)
		return false;

	PFStringA::Copy(pszPath, MaxLength, szPath);

	LPSTR pszApp = PFStringA::Strstr(szPath, ".app");
	if(pszApp == nullptr)
		return false;

	LPSTR psz = PFStringA::Strchr(pszApp, '/');
	if(psz != nullptr)
	{
		*psz = 0;
	}

	PFStringA::Copy(pszPath, MaxLength, szPath);

	return true;
}

template <>
inline bool TMacPlatfornPath<WCHAR>::GetModuleFilePath(HMODULE hModule, LPWSTR pszPath, UInt32 MaxLength)
{
	INT nReturn;

	CHAR szPath[PATH_MAX] = { 0 };

	if(pszPath == nullptr || MaxLength == 0)
		return false;

//	nReturn = readlink("/proc/self/exe", szPath, sizeof(szPath) / sizeof(szPath[0]) - 1);
//	if(nReturn < 0)
//		return false;

	PFStringW::Copy(pszPath, MaxLength, ASTR_TO_WSTR(szPath));

	return true;
}

// GetModuleDirPath
template <>
inline bool TMacPlatfornPath<CHAR>::GetModuleDirPath(HMODULE hModule, LPSTR pszPath, UInt32 MaxLength)
{
	LPSTR pszPointer = nullptr;

	if(pszPath == nullptr || MaxLength == 0)
		return false;

	pszPath[0] = '\0';

	if(TMacPlatfornPath<CHAR>::GetModuleFilePath(hModule, pszPath, MaxLength) == 0)
		return false;

	pszPointer = PFStringA::Strrchr(pszPath, '\\');
	if(pszPointer == nullptr)
		return false;

	pszPointer[0] = '\0';

	return true;
}

template <>
inline bool TMacPlatfornPath<WCHAR>::GetModuleDirPath(HMODULE hModule, LPWSTR pszPath, UInt32 MaxLength)
{
	LPWSTR pszPointer = nullptr;

	if(pszPath == nullptr || MaxLength == 0)
		return false;

	pszPath[0] = L'\0';

	if(TMacPlatfornPath<WCHAR>::GetModuleFilePath(hModule, pszPath, MaxLength) == 0)
		return false;

	pszPointer = PFStringW::Strrchr(pszPath, L'\\');
	if(pszPointer == nullptr)
		return false;

	pszPointer[0] = L'\0';

	return true;
}

template <typename T>
using PFPathT = TMacPlatfornPath<T>;

typedef TMacPlatfornPath< CHAR> PFPathA;
typedef TMacPlatfornPath<WCHAR> PFPathW;
typedef TMacPlatfornPath<TCHAR> PFPath;
