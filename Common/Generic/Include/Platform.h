#pragma once

#include <PlatformDefine.h>

#include <PlatformTypes.h>

#include <PlatformMacro.h>

#include <PlatformCompiler.h>

#if PLATFORM_WINDOWS
	#ifdef PLATFORM_WINDOWS_AFX
		#include <WindowsPlatformAfx.h>
	#else
		#include <WindowsPlatform.h>
	#endif
#elif PLATFORM_LINUX
	#include <LinuxPlatform.h>
#elif PLATFORM_MAC
	#include <MacPlatform.h>
#else
	#error Unknown Platform
#endif

#include <PlatformCrt.h>

#include <PlatformMath.h>

#include <PlatformMemory.h>

#include <PlatformString.h>

#include <PlatformPath.h>

#include <ComplieTimeCheck.h>
