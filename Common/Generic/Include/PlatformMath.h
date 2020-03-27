#pragma once

#include <GenericPlatformMath.h>

#if PLATFORM_WINDOWS
	#include <WindowsPlatformMath.h>
#elif PLATFORM_LINUX
	#include <LinuxPlatformMath.h>
#elif PLATFORM_MAC
	#include <MacPlatformMath.h>
#else
	#error Unknown Platform
#endif
