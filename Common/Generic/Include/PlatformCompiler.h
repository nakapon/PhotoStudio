#pragma once

#if PLATFORM_WINDOWS
	#include <WindowsPlatformCompiler.h>
#elif PLATFORM_LINUX
	#include <LinuxPlatformCompiler.h>
#elif PLATFORM_MAC
	#include <MacPlatformCompiler.h>
#else
	#error Unknown Platform
#endif

// Alignment
#ifndef PRAGMA_PACK_PUSH
	#define PRAGMA_PACK_PUSH(n)
#endif
#ifndef PRAGMA_PACK_POP
	#define PRAGMA_PACK_POP()
#endif

#ifndef GCC_PACK
	#define GCC_PACK(n)
#endif
#ifndef GCC_ALIGN
	#define GCC_ALIGN(n)
#endif
#ifndef MS_ALIGN
	#define MS_ALIGN(n)
#endif

// DLL export and import
#ifndef DLLEXPORT
	#define DLLEXPORT
	#define DLLIMPORT
#endif

// Linker
#ifndef PRAGMA_LINK_LIBRARY
	#define PRAGMA_LINK_LIBRARY(name)
#endif

#ifndef PRAGMA_LINK_OPTION
	#define PRAGMA_LINK_OPTION(opt)
#endif

// Inline
#ifndef FORCEINLINE
	#define FORCEINLINE
#endif

#ifndef FORCENOINLINE
	#define FORCENOINLINE
#endif

// Branch prediction hints
#ifndef LIKELY
	#define LIKELY(x)		(x)
	#define IF_LIKELY(x)	if(x)
#endif

#ifndef UNLIKELY
	#define UNLIKELY(x)		(x)
	#define IF_UNLIKELY(x)	if(x)
#endif
