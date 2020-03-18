#pragma once

#include <string.h>
#include <ctype.h>

#define strcpy_s(d, n, s) strcpy((d), (s))
#define strcat_s(d, n, s) strcat((d), (s))

#define wcscpy_s(d, n, s) wcscpy((d), (s))
#define wcscat_s(d, n, s) wcscat((d), (s))

#if BUILD_IS_UNICODE
	#define _tcscpy(d, s) wcscpy((d), (s))
	#define _tcscat(d, s) wcscat((d), (s))
	#define _tcscpy_s(d, n, s) wcscpy((d), (s))
	#define _tcscat_s(d, n, s) wcscat((d), (s))
	#define _tcscmp(s1, s2) wcscmp((s1), (s2))
	#define _tcsclen(s) wcslen((s))
	#define _tcslen(s) wcslen((s))
	#define _tcschr(s, c) wcschr((s), (c))
	#define _tcsrchr(s, c) wcsrchr((s), (c))
	#define _tcsstr(s1, s2) wcsstr((s1), (s2))
	#define _totlower(s) towlower((s))

	#define _stscanf swscanf
	#define _stprintf swprintf

	#define wsprintf swprintf
#else
	#define _tcscpy(d, s) strcpy((d), (s))
	#define _tcscat(d, s) strcat((d), (s))
	#define _tcscpy_s(d, n, s) strcpy((d), (s))
	#define _tcscat_s(d, n, s) strcat((d), (s))
	#define _tcscmp(s1, s2) strcmp((s1), (s2))
	#define _tcsclen(s) strlen((s))
	#define _tcslen(s) strlen((s))
	#define _tcschr(s, c) strchr((s), (c))
	#define _tcsrchr(s, c) strrchr((s), (c))
	#define _tcsstr(s1, s2) strstr((s1), (s2))
	#define _totlower(s) tolower((s))

	#define _stscanf sscanf
	#define _stprintf sprintf

	#define wsprintf sprintf
#endif

#define _tfopen fopen
