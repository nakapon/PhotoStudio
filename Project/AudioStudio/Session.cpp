#include <Platform.h>

#include "Session.h"

#define SESSION_FILE_NAME	TEXT("AudioStudio.ssn")

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

	PFString::Copy(pszFilePath, MaxLength, ASTR_TO_TSTR(szFilePathA));

	return true;
}

static void GetFilePath(LPTSTR pszFilePath, UInt32 MaxLength)
{
	TCHAR szFilePath[MAX_PATH] = { 0 };

	PFPath::GetModuleFilePath(nullptr, szFilePath);

	LPTSTR pszExtension = PFString::Strrchr(szFilePath, PF_DIR_SEP_CHAR);
	if(pszExtension != nullptr)
	{
		pszExtension[1] = _T('\0');
	}

	PFString::Append(szFilePath, SESSION_FILE_NAME);

	PFString::Copy(pszFilePath, MaxLength, szFilePath);
}
