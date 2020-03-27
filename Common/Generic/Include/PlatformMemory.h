#pragma once

#include <GenericPlatformMemory.h>

#if PLATFORM_WINDOWS
	#include <WindowsPlatformMemory.h>
#elif PLATFORM_LINUX
	#include <LinuxPlatformMemory.h>
#elif PLATFORM_MAC
	#include <MacPlatformMemory.h>
#else
	#error Unknown Platform
#endif
