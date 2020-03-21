#pragma once

#if PLATFORM_WINDOWS
	#include <WindowsPlatformString.h>
#elif PLATFORM_LINUX
	#include <LinuxPlatformString.h>
#elif PLATFORM_MAC
	#include <MacPlatformString.h>
#else
	#error Unknown Platform
#endif

#include <StringConv.h>
