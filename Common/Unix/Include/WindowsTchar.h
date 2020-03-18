#pragma once

#include <string.h>
#include <ctype.h>

#define strcpy_s(d, s) strcpy((d), (s))
#define strcpy_s(d, n, s) strcpy((d), (s))
#define strcat_s(d, s) strcat((d), (s))
#define strcat_s(d, n, s) strcat((d), (s))

#define wcscpy_s(d, s) wcscpy((d), (s))
#define wcscpy_s(d, n, s) wcscpy((d), (s))
#define wcscat_s(d, s) wcscat((d), (s))
#define wcscat_s(d, n, s) wcscat((d), (s))

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
