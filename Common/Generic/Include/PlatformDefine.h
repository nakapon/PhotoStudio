#pragma once

/*
	The following macros are defined as '1' if the condition is met, and '0' otherwise.

	[CPU]
		PLATFORM_CPU_INTEL_X86
		PLATFORM_CPU_INTEL_X64

	[Platform]
		PLATFORM_WINDOWS
		PLATFORM_LINUX
		PLATFORM_MAC
		PLATFORM_IOS

	PLATFORM_32BIT
	PLATFORM_64BIT

	PLATFORM_LITTLE_ENDIAN

	[Compiler]
		PLATFORM_COMPILER_CLANG
		PLATFORM_COMPILER_GCC
		PLATFORM_COMPILER_MSVC
		PLATFORM_COMPILER_INTEL
		PLATFORM_COMPILER_CYGWIN
		PLATFORM_COMPILER_MINGW
		PLATFORM_COMPILER_NVCC
		PLATFORM_COMPILER_QT_MOC

	[Build Condition]
		BUILD_IS_64BIT
		BUILD_IS_DEBUG
		BUILD_IS_UNICODE
		BUILD_IS_CPP
*/


/*-----------------------------------------------------------------------------
 * CPU
 *----------------------------------------------------------------------------*/
#if (defined(_M_X64) || defined(__x86_64__))
	#define PLATFORM_CPU_INTEL_X64 1
	#define PLATFORM_64BIT 1
#elif (defined(_M_IX86) || defined(__i386))
	#define PLATFORM_CPU_INTEL_X86 1
	#define PLATFORM_32BIT 1
#else
	#error Unsupported CPU
#endif


#if !defined(PLATFORM_CPU_X86_FAMILY)
	#define PLATFORM_CPU_X86_FAMILY 0
#endif

#if !defined(PLATFORM_CPU_INTEL_X86)
	#define PLATFORM_CPU_INTEL_X86 0
#endif

#if !defined(PLATFORM_CPU_INTEL_X64)
	#define PLATFORM_CPU_INTEL_X64 0
#endif


/*-----------------------------------------------------------------------------
 * Platform
 *----------------------------------------------------------------------------*/
#if defined(_WIN32)
	#define PLATFORM_WINDOWS 1
	#define PLATFORM_LITTLE_ENDIAN 1
#elif (defined(__linux) || defined(__linux__))
	#define PLATFORM_LINUX 1
	#define PLATFORM_LITTLE_ENDIAN 1
#elif defined(__APPLE__)
	#define PLATFORM_APPLE 1
	#define PLATFORM_LITTLE_ENDIAN 1
	#if defined(TARGET_OS_IPHONE)
		#define PLATFORM_IOS 1
		#define PLATFORM_64BIT 1
	#elif defined(TARGET_OS_OSX)
		#define PLATFORM_MAC 1
	#else
		#error Unknown Apple Platform
	#endif
#else
	#error Unknown Platform
#endif


#if !defined(PLATFORM_WINDOWS)
	#define PLATFORM_WINDOWS 0
#endif

#if !defined(PLATFORM_LINUX)
	#define PLATFORM_LINUX 0
#endif

#if !defined(PLATFORM_APPLE)
	#define PLATFORM_APPLE 0
#endif

#if !defined(PLATFORM_MAC)
	#define PLATFORM_MAC 0
#endif

#if !defined(PLATFORM_IOS)
	#define PLATFORM_IOS 0
#endif


#if !defined(PLATFORM_32BIT)
	#define PLATFORM_32BIT 0
#endif

#if !defined(PLATFORM_64BIT)
	#define PLATFORM_64BIT 0
#endif


#if !defined(PLATFORM_LITTLE_ENDIAN)
	#define PLATFORM_LITTLE_ENDIAN 0
#endif


/*-----------------------------------------------------------------------------
 * Compiler
 *----------------------------------------------------------------------------*/

#if defined(__clang__)
	#define PLATFORM_COMPILER_CLANG 1
#elif defined(__GNUC__)
	#define PLATFORM_COMPILER_GCC 1
#elif defined(_MSC_VER)
	#define PLATFORM_COMPILER_MSVC 1
#else
	#error Unknown Compiler
#endif

#if defined(__INTEL_COMPILER)
	#define PLATFORM_COMPILER_INTEL 1
#endif

#if (defined(__CYGWIN__) || defined(__CYGWIN32__))
	#define PLATFORM_COMPILER_CYGWIN 1
#endif

#if (defined(__MINGW32__) || defined(__MINGW64__))
	#define PLATFORM_COMPILER_MINGW 1
#endif

#if (defined(__NVCC__) || defined(__CUDACC__))
	#define PLATFORM_COMPILER_NVCC 1
#endif

#if defined(Q_MOC_RUN)
	#define PLATFORM_COMPILER_QT_MOC 1
#endif


#if !defined(PLATFORM_COMPILER_CLANG)
	#define PLATFORM_COMPILER_CLANG 0
#endif

#if !defined(PLATFORM_COMPILER_GCC)
	#define PLATFORM_COMPILER_GCC 0
#endif

#if !defined(PLATFORM_COMPILER_MSVC)
	#define PLATFORM_COMPILER_MSVC 0
#endif

#if !defined(PLATFORM_COMPILER_INTEL)
	#define PLATFORM_COMPILER_INTEL 0
#endif

#if !defined(PLATFORM_COMPILER_CYGWIN)
	#define PLATFORM_COMPILER_CYGWIN 0
#endif

#if !defined(PLATFORM_COMPILER_MINGW)
	#define PLATFORM_COMPILER_MINGW 0
#endif

#if !defined(PLATFORM_COMPILER_NVCC)
	#define PLATFORM_COMPILER_NVCC 0
#endif

#if !defined(PLATFORM_COMPILER_QT_MOC)
	#define PLATFORM_COMPILER_QT_MOC 0
#endif


/*-----------------------------------------------------------------------------
 * Build Condition
 *----------------------------------------------------------------------------*/
#if PLATFORM_64BIT
	#define BUILD_IS_64BIT 1
#else
	#define BUILD_IS_64BIT 0
#endif

#if (defined(DEBUG) || defined(_DEBUG))
	#define BUILD_IS_DEBUG 1
#else
	#define BUILD_IS_DEBUG 0
#endif

#if (defined(UNICODE) || defined(_UNICODE))
	#define BUILD_IS_UNICODE 1
#else
	#define BUILD_IS_UNICODE 0
#endif

#ifdef __cplusplus
	#define BUILD_IS_CPP 1
#else
	#define BUILD_IS_CPP 0
#endif
