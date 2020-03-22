#pragma once

#include <GenericPlatformPath.h>

#if PLATFORM_WINDOWS
	#include <WindowsPlatformPath.h>
#elif PLATFORM_LINUX
	#include <LinuxPlatformPath.h>
#elif PLATFORM_MAC
	#include <MacPlatformPath.h>
#else
	#error Unknown Platform
#endif
