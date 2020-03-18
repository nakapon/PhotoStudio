#pragma once

#include <Templates/Conditional.h>

struct GenericPlatformTypes
{
	typedef unsigned char       UInt8;  //  8-bit unsigned
	typedef unsigned short int  UInt16; // 16-bit unsigned
	typedef unsigned int        UInt32; // 32-bit unsigned
	typedef unsigned long long  UInt64; // 64-bit unsigned

	typedef signed char         Int8;   //  8-bit signed
	typedef signed short int    Int16;  // 16-bit signed
	typedef signed int          Int32;  // 32-bit signed
	typedef signed long long    Int64;  // 64-bit signed

	typedef float               Float32;// 32-bit float
	typedef double              Float64;// 64-bit float

	typedef UInt8               Char8;  //  8-bit char
	typedef UInt16              Char16; // 16-bit char
	typedef UInt32              Char32; // 32-bit char

	typedef TConditional<sizeof(void*) == 8, UInt64, UInt32>::Type UIntPtr;
	typedef TConditional<sizeof(void*) == 8,  Int64,  Int32>::Type  IntPtr;

	typedef UIntPtr SIZE_T;
	typedef IntPtr SSIZE_T;
};
