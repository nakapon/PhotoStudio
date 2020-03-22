#pragma once

#include <Platform.h>

// to QString
#define PFQT_ASTR_TO_QSTR(str)	QString(str)
#define PFQT_WSTR_TO_QSTR(str)	QString::fromWCharArray(str)
#if BUILD_IS_UNICODE
#define PFQT_TSTR_TO_QSTR(str)	QString::fromWCharArray(str)
#else
#define PFQT_TSTR_TO_QSTR(str)	QString(str)
#endif

// from QString
#define PFQT_COPY_QSTR_TO_ASTR(dst, maxLen, src)	PFStringA::Copy(dst, (LPCSTR)src.toLocal8Bit().data())
#define PFQT_COPY_QSTR_TO_WSTR(dst, maxLen, src)	src.toWCharArray(dst)
#if BUILD_IS_UNICODE
#define PFQT_COPY_QSTR_TO_TSTR(dst, maxLen, src)	src.toWCharArray(dst)
#else
#define PFQT_COPY_QSTR_TO_TSTR(dst, maxLen, src)	PFStringA::Copy(dst, (LPCSTR)src.toLocal8Bit().data())
#endif
