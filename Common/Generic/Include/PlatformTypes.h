#pragma once

#include <GenericPlatformTypes.h>

#if PLATFORM_WINDOWS
	#include <WindowsPlatformTypes.h>
#elif PLATFORM_LINUX
	#include <LinuxPlatformTypes.h>
#elif PLATFORM_MAC
	#include <MacPlatformTypes.h>
#else
	#error Unknown Platform
#endif

#ifndef UNUSED_PLATFORM_TYPES
#define PLATFORM_TYPES
typedef PlatformTypes::UInt8    UInt8;      // 8-bit  unsigned
typedef PlatformTypes::UInt16   UInt16;     // 16-bit unsigned
typedef PlatformTypes::UInt32   UInt32;     // 32-bit unsigned
typedef PlatformTypes::UInt64   UInt64;     // 64-bit unsigned

typedef PlatformTypes::Int8     SInt8;      // 8-bit  signed
typedef PlatformTypes::Int16    SInt16;     // 16-bit signed
typedef PlatformTypes::Int32    SInt32;     // 32-bit signed
typedef PlatformTypes::Int64    SInt64;     // 64-bit signed

typedef PlatformTypes::Int8     Int8;       // 8-bit  signed
typedef PlatformTypes::Int16    Int16;      // 16-bit signed
typedef PlatformTypes::Int32    Int32;      // 32-bit signed
typedef PlatformTypes::Int64    Int64;      // 64-bit signed

typedef PlatformTypes::Float32  FSingle;    // 32-bit float
typedef PlatformTypes::Float64  FDouble;    // 64-bit float

typedef PlatformTypes::Char8    Char8;      // 8-bit char
typedef PlatformTypes::Char16   Char16;     // 16-bit char
typedef PlatformTypes::Char32   Char32;     // 32-bit char

typedef PlatformTypes::Char8    CHAR8;      // 8-bit char
typedef PlatformTypes::Char16   CHAR16;     // 16-bit char
typedef PlatformTypes::Char32   CHAR32;     // 32-bit char
#endif

typedef PlatformTypes::Char8    UTF8CHAR;   // UTF-8  (Unicode,  8-bit, variable-width)
typedef PlatformTypes::Char16   UTF16CHAR;  // UTF-16 (Unicode, 16-bit, variable-width)
typedef PlatformTypes::Char32   UTF32CHAR;  // UTF-32 (Unicode, 32-bit, fixed-width)

typedef PlatformTypes::IntPtr   INTPTR;
typedef PlatformTypes::UIntPtr  UINTPTR;

typedef PlatformTypes::IntPtr   IntPtr;
typedef PlatformTypes::UIntPtr  UIntPtr;

typedef PlatformTypes::SIZE_T   SIZE_T;
typedef PlatformTypes::SSIZE_T  SSIZE_T;

enum class PFEndians
{
	Little,
	Big,
};

#ifndef QWORD
typedef PlatformTypes::UInt64  QWORD;
typedef PlatformTypes::UInt64 *PQWORD;
typedef PlatformTypes::UInt64 *LPQWORD;
#endif // !QWORD

// Check basic type size
static_assert(sizeof(PlatformTypes::Int8)  == 1, "Int8 size must be 1 byte.");
static_assert(sizeof(PlatformTypes::Int16) == 2, "Int16 size must be 2 bytes.");
static_assert(sizeof(PlatformTypes::Int32) == 4, "Int32 size must be 4 bytes.");
static_assert(sizeof(PlatformTypes::Int64) == 8, "Int64 size must be 8 bytes.");

static_assert(sizeof(PlatformTypes::UInt8)  == 1, "UInt8 size must be 1 byte.");
static_assert(sizeof(PlatformTypes::UInt16) == 2, "UInt16 size must be 2 bytes.");
static_assert(sizeof(PlatformTypes::UInt32) == 4, "UInt32 size must be 4 bytes.");
static_assert(sizeof(PlatformTypes::UInt64) == 8, "UInt64 size must be 8 bytes.");

static_assert(sizeof(PlatformTypes::Float32) == 4, "Float32 size must be 4 bytes.");
static_assert(sizeof(PlatformTypes::Float64) == 8, "Float64 size must be 8 bytes.");
