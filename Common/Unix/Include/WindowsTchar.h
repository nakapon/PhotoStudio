#pragma once

#include <string.h>
#include <ctype.h>

inline char* strcpy_s(char* pszDst, UInt32 MaxLength, const char* pszSrc)
{
	return strcpy(pszDst, pszSrc);
}

template <UInt32 MaxLength>
inline char* strcpy_s(char (&pszDst)[MaxLength], const char* pszSrc)
{
	return strcpy(pszDst, pszSrc);
}

inline char* strcat_s(char* pszDst, UInt32 MaxLength, const char* pszSrc)
{
	return strcat(pszDst, pszSrc);
}

template <UInt32 MaxLength>
inline char* strcat_s(char (&pszDst)[MaxLength], const char* pszSrc)
{
	return strcat(pszDst, pszSrc);
}

inline wchar_t* wcscpy_s(wchar_t* pszDst, UInt32 MaxLength, const wchar_t* pszSrc)
{
	return wcscpy(pszDst, pszSrc);
}

template <UInt32 MaxLength>
inline wchar_t* wcscpy_s(wchar_t (&pszDst)[MaxLength], const wchar_t* pszSrc)
{
	return wcscpy(pszDst, pszSrc);
}

inline wchar_t* wcscat_s(wchar_t* pszDst, UInt32 MaxLength, const wchar_t* pszSrc)
{
	return wcscat(pszDst, pszSrc);
}

template <UInt32 MaxLength>
inline wchar_t* wcscat_s(wchar_t (&pszDst)[MaxLength], const wchar_t* pszSrc)
{
	return wcscat_s(pszDst, pszSrc);
}

#if BUILD_IS_UNICODE
	#define _tcscpy wcscpy
	#define _tcscat wcscat
	#define _tcscpy_s wcscpy
	#define _tcscat_s wcscat
	#define _tcscmp wcscmp
	#define _tcsclen wcslen
	#define _tcslen wcslen
	#define _tcschr wcschr
	#define _tcsrchr wcsrchr
	#define _tcsstr wcsstr
	#define _totlower towlower

	#define _stscanf swscanf
	#define _stprintf swprintf

	#define wsprintf swprintf
#else
	#define _tcscpy strcpy
	#define _tcscat strcat
	#define _tcscpy_s strcpy
	#define _tcscat_s strcat
	#define _tcscmp strcmp
	#define _tcsclen strlen
	#define _tcslen strlen
	#define _tcschr strchr
	#define _tcsrchr strrchr
	#define _tcsstr strstr
	#define _totlower tolower

	#define _stscanf sscanf
	#define _stprintf sprintf

	#define wsprintf sprintf
#endif

#define _tfopen fopen
