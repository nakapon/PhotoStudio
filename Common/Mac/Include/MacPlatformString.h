#pragma once

#include <UnixPlatformString.h>

template <typename T>
struct TMacPlatformString : public TUnixPlatformString<T>
{
};

template <typename T>
using PFStringT = TMacPlatformString<T>;

typedef TMacPlatformString< CHAR> PFStringA;
typedef TMacPlatformString<WCHAR> PFStringW;
typedef TMacPlatformString<TCHAR> PFString;
