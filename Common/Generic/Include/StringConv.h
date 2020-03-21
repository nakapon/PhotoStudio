#pragma once

#if BUILD_IS_UNICODE
	#define ASTR_TO_WSTR(str) ((LPCWSTR)(FStringConv((LPCSTR)(str))))
	#define ASTR_TO_TSTR(str) ((LPCTSTR)(FStringConv((LPCSTR)(str))))

	#define WSTR_TO_ASTR(str) ((LPCSTR )(FStringConv((LPCWSTR)(str))))
	#define WSTR_TO_TSTR(str) ((LPCTSTR)(str))

	#define TSTR_TO_ASTR(str) ((LPCSTR )(FStringConv((LPCTSTR)(str))))
	#define TSTR_TO_WSTR(str) ((LPCWSTR)(str))
#else
	#define ASTR_TO_WSTR(str) ((LPCWSTR)(FStringConv((LPCSTR)(str))))
	#define ASTR_TO_TSTR(str) ((LPCSTR)(str))

	#define WSTR_TO_ASTR(str) ((LPCSTR )(FStringConv((LPCWSTR)(str))))
	#define WSTR_TO_TSTR(str) ((LPCTSTR)(FStringConv((LPCWSTR)(str))))

	#define TSTR_TO_ASTR(str) ((LPCSTR)(str))
	#define TSTR_TO_WSTR(str) ((LPCWSTR)(FStringConv((LPCTSTR)(str))))
#endif

class FStringConv
{
public:
	FStringConv();
	FStringConv(LPCSTR pszString);
	FStringConv(LPCWSTR pwszString);

	FStringConv(const FStringConv& String);

	FStringConv(FStringConv&& String);

	~FStringConv();

	void Clear();

	DWORD GetLength() const;

	// operator=
	FStringConv& operator=(const FStringConv& String);
	FStringConv& operator=(FStringConv&& String);
	FStringConv& operator=(LPCSTR pszString);
	FStringConv& operator=(LPCWSTR pwszString);

	// type operator
	operator LPCSTR();
	operator LPCWSTR();

protected:
	LPWSTR m_pwszString;
	LPSTR m_pszString;
	DWORD m_dwLength;

protected:
	void MoveFrom(FStringConv& String);
};
