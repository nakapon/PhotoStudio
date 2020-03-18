#pragma once

#include <stdint.h>
#include <string.h>

typedef short				SHORT, *PSHORT;
typedef int					INT, *PINT;
typedef unsigned int		UINT, *PUINT;
typedef int					LONG, *PLONG;
typedef long long			INT64, *PINT64;
typedef unsigned long long	UINT64, *PUINT64;
typedef unsigned int		ULONG, *PULONG;
typedef unsigned char		BYTE, *PBYTE;
typedef unsigned short		WORD, *PWORD;
typedef unsigned int		DWORD, *PDWORD;
typedef unsigned long long	QWORD, *PQWORD;
typedef int					BOOL, *PBOOL;
typedef float				FLOAT, *PFLOAT;
typedef double				DOUBLE, *PDOUBLE;

typedef intptr_t			INT_PTR, *PINT_PTR;
typedef uintptr_t			UINT_PTR, *PUINT_PTR;

typedef intptr_t			LONG_PTR, *PLONG_PTR;
typedef uintptr_t			ULONG_PTR, *PULONG_PTR;

typedef uintptr_t			DWORD_PTR, *PDWORD_PTR;

typedef long long			LONGLONG;
typedef unsigned long long	ULONGLONG;

typedef char				CHAR, *PCHAR, *LPSTR, *PSTR;
typedef const char			*LPCSTR, *PCSTR;

typedef wchar_t				WCHAR, *PWCHAR, *LPWSTR, *PWSTR;
typedef const wchar_t		*LPCWSTR, *PCWSTR;

#if BUILD_IS_UNICODE
	typedef wchar_t			TCHAR, *PTCHAR, *LPTSTR;
	typedef const wchar_t	*LPCTSTR;
#else
	typedef char			TCHAR, *PTCHAR, *LPTSTR;
	typedef const char		*LPCTSTR;
#endif

typedef UINT_PTR			WPARAM;
typedef LONG_PTR			LPARAM;
typedef LONG_PTR			LRESULT;

typedef DWORD				COLORREF;

typedef void				*HINSTANCE;

typedef void				*HBITMAP;

typedef void				*HICON;
typedef void				*HCURSOR;

#if BUILD_IS_UNICODE
#define __T(x)				L##x
#define _T(x)				__T(x)
#define __TEXT(x)			L##x
#define TEXT(x)				__TEXT(x)
#else
#define TEXT(x)				x
#define _T(x)				x
#endif

#define CONST				const

#define VOID				void
typedef void				*PVOID, *LPVOID;

#ifndef NULL
	#ifdef __cplusplus
	#define NULL			0
	#else
	#define NULL			((void *)0)
	#endif
#endif

#ifndef FALSE
#define FALSE				0
#endif

#ifndef TRUE
#define TRUE				1
#endif

typedef void				*HANDLE;
#define INVALID_HANDLE_VALUE ((HANDLE)-1)

typedef void				*HWND;

typedef void				*HMODULE;

typedef union _LARGE_INTEGER
{
	struct
	{
		DWORD LowPart;
		LONG HighPart;
	} u;
	LONGLONG QuadPart;
} LARGE_INTEGER;

typedef LARGE_INTEGER *PLARGE_INTEGER;

typedef union _ULARGE_INTEGER
{
	struct
	{
		DWORD LowPart;
		DWORD HighPart;
	} u;
	ULONGLONG QuadPart;
} ULARGE_INTEGER;

typedef ULARGE_INTEGER *PULARGE_INTEGER;

#define MAX_PATH			256

#define WINAPI

#define CALLBACK

#define STDMETHODCALLTYPE

#ifndef NEAR
#define NEAR
#endif

#ifndef FAR
#define FAR
#endif

#ifndef OUT
#define OUT
#endif

#ifndef IN
#define IN
#endif

#define INFINITE			0xFFFFFFFF  // Infinite timeout

/*
 * HRESULT
 */
typedef long				HRESULT;

#define S_OK				((HRESULT)0L)
#define S_FALSE				((HRESULT)1L)

#define _HRESULT_TYPEDEF_(_sc) ((HRESULT)_sc)

#define E_UNEXPECTED		_HRESULT_TYPEDEF_(0x8000FFFFL)
#define E_NOTIMPL			_HRESULT_TYPEDEF_(0x80004001L)
#define E_OUTOFMEMORY		_HRESULT_TYPEDEF_(0x8007000EL)
#define E_INVALIDARG		_HRESULT_TYPEDEF_(0x80070057L)
#define E_NOINTERFACE		_HRESULT_TYPEDEF_(0x80004002L)
#define E_POINTER			_HRESULT_TYPEDEF_(0x80004003L)
#define E_HANDLE			_HRESULT_TYPEDEF_(0x80070006L)
#define E_ABORT				_HRESULT_TYPEDEF_(0x80004004L)
#define E_FAIL				_HRESULT_TYPEDEF_(0x80004005L)
#define E_ACCESSDENIED		_HRESULT_TYPEDEF_(0x80070005L)
