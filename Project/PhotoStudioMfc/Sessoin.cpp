#include <Platform.h>

#include <stdio.h>
#include <tchar.h>

#include "Session.h"

static void GetFilePath(LPTSTR pszFilePath, UInt32 MaxLength);

bool Session::StoreSession(LPCTSTR pszFilePath)
{
	TCHAR szSessionPath[MAX_PATH] = { 0 };

	::GetFilePath(szSessionPath, PF_ARRAY_LENGTH(szSessionPath));

	{
		FILE* fp;

		fp = _tfopen(szSessionPath, TEXT("w"));
		if(fp == nullptr)
			return false;

		fprintf(fp, TSTR_TO_ASTR(pszFilePath));

		fclose(fp);
	}

	return true;
}

bool Session::RestoreSession(LPTSTR pszFilePath, UInt32 MaxLength)
{
	TCHAR szSessionPath[MAX_PATH] = { 0 };

	CHAR szFilePathA[MAX_PATH] = { 0 };

	::GetFilePath(szSessionPath, PF_ARRAY_LENGTH(szSessionPath));

	{
		FILE* fp;

		fp = _tfopen(szSessionPath, TEXT("r"));
		if(fp == nullptr)
			return false;

		fread(&szFilePathA[0], sizeof(szFilePathA[0]), PF_ARRAY_LENGTH(szFilePathA), fp);

		fclose(fp);
	}

	_tcscpy_s(pszFilePath, MaxLength, ASTR_TO_TSTR(szFilePathA));

	return true;
}

static void GetFilePath(LPTSTR pszFilePath, UInt32 MaxLength)
{
	TCHAR szFilePath[MAX_PATH] = { 0 };

	GetModuleFileName(nullptr, szFilePath, PF_ARRAY_LENGTH(szFilePath));

	LPTSTR pszExtension = _tcsrchr(szFilePath, _T('\\'));
	if(pszExtension != nullptr)
	{
		pszExtension[1] = _T('\0');
	}

	_tcscat_s(szFilePath, TEXT("PhotoStudioMfc.ssn"));

	_tcscpy_s(pszFilePath, MaxLength, szFilePath);
}
