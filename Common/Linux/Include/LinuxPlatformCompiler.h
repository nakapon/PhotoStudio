#pragma once

// Pragma
#define PRAGMA(x) _Pragma(#x)

// Alignment
#define PRAGMA_PACK_PUSH(n) PRAGMA(pack(n))
#define PRAGMA_PACK_POP()   PRAGMA(pack(0))

#define GCC_PACK(n)  __attribute__((packed, aligned(n)))

#define GCC_ALIGN(n) __attribute__((aligned(n)))

// DLL export and import
#define DLLEXPORT __attribute__((visibility("default")))
#define DLLIMPORT __attribute__((visibility("default")))

// Inline
#ifndef FORCEINLINE
	#define FORCEINLINE   inline __attribute__((always_inline))
#endif

#ifndef FORCENOINLINE
	#define FORCENOINLINE __attribute__((noinline))
#endif

// Branch prediction hints
#define LIKELY(x)		__builtin_expect(!!(x), 1)
#define IF_LIKELY(x)	if(__builtin_expect(!!(x), 1))

#define UNLIKELY(x)		__builtin_expect(!!(x), 0)
#define IF_UNLIKELY(x)	if(__builtin_expect(!!(x), 0))
