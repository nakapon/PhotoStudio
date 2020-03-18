#pragma once

#include <PlatformDefine.h>

#include <PlatformTypes.h>

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

#include <ComplieTimeCheck.h>
