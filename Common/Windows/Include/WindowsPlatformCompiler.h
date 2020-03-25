#pragma once

// Pragma
#define PRAGMA(x) __pragma(x)

// Alignment
#define PRAGMA_PACK_PUSH(n) PRAGMA(pack(push, n))
#define PRAGMA_PACK_POP()   PRAGMA(pack(pop))

#define MS_ALIGN(n) __declspec(align(n))

// DLL export and import
#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)

// Linker
#define PRAGMA_LINK_LIBRARY(name) __pragma(comment(lib, name))
#define PRAGMA_LINK_OPTION(opt)   __pragma(comment(linker, opt))

// Inline
#ifndef FORCEINLINE
	#define FORCEINLINE   __forceinline
#endif

#ifndef FORCENOINLINE
	#define FORCENOINLINE __declspec(noinline)
#endif
