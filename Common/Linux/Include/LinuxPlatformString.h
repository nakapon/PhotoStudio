#pragma once

#include <UnixPlatformString.h>

template <typename T>
struct TLinuxPlatformString : public TUnixPlatformString<T>
{
};

template <typename T>
using PFStringT = TLinuxPlatformString<T>;

typedef TLinuxPlatformString< CHAR> PFStringA;
typedef TLinuxPlatformString<WCHAR> PFStringW;
typedef TLinuxPlatformString<TCHAR> PFString;
