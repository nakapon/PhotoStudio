#pragma once

#if PLATFORM_WINDOWS
	#include <WindowsPlatformCrt.h>
#elif PLATFORM_LINUX
	#include <LinuxPlatformCrt.h>
#elif PLATFORM_MAC
	#include <MacPlatformCrt.h>
#else
	#error Unknown Platform
#endif
