#include <windows.h>

#include <stdio.h>
#include <tchar.h>

#include "Session.h"

static void GetFilePath(LPTSTR pszFilePath, UINT MaxLength);

bool Session::StoreSession(LPCTSTR pszFilePath)
{
	TCHAR szSessionPath[MAX_PATH] = { 0 };

	GetFilePath(szSessionPath, sizeof(szSessionPath) / sizeof(szSessionPath[0]));

	{
		FILE* fp;

		fp = _tfopen(szSessionPath, TEXT("w"));
		if(fp == nullptr)
			return false;

		// TODO TSTR_TO_ASTR() マクロを導入
		CHAR szFilePathA[MAX_PATH] = { 0 };
		WideCharToMultiByte(CP_ACP, 0, pszFilePath, -1, szFilePathA, sizeof(szFilePathA) / sizeof(szFilePathA[0]), nullptr, nullptr);

		fprintf(fp, szFilePathA);

		fclose(fp);
	}

	return true;
}

bool Session::RestoreSession(LPTSTR pszFilePath, UINT MaxLength)
{
	TCHAR szSessionPath[MAX_PATH] = { 0 };

	CHAR szFilePathA[MAX_PATH] = { 0 };

	GetFilePath(szSessionPath, sizeof(szSessionPath) / sizeof(szSessionPath[0]));

	{
		FILE* fp;

		fp = _tfopen(szSessionPath, TEXT("r"));
		if(fp == nullptr)
			return false;

		fread(&szFilePathA[0], sizeof(szFilePathA[0]), sizeof(szFilePathA) / sizeof(szFilePathA[0]), fp);

		fclose(fp);
	}

	// TODO ASTR_TO_TSTR() マクロを導入
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFilePathA, sizeof(szFilePathA), pszFilePath, MaxLength);

	return true;
}

static void GetFilePath(LPTSTR pszFilePath, UINT MaxLength)
{
	TCHAR szFilePath[MAX_PATH] = { 0 };

	GetModuleFileName(nullptr, szFilePath, sizeof(szFilePath) / sizeof(szFilePath[0]));

	LPTSTR pszExtension = _tcsrchr(szFilePath, _T('\\'));
	if(pszExtension != nullptr)
	{
		pszExtension[1] = _T('\0');
	}

	_tcscat_s(szFilePath, TEXT("PhotoStudio.ssn"));

	_tcscpy_s(pszFilePath, MaxLength, szFilePath);
}
