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

#include <Math/Vec2.h>
#include <Math/Vec3.h>

#include <Math/Mat22.h>
#include <Math/Mat33.h>

#include <Math/Size2.h>
#include <Math/Rect2.h>
#include <Math/Region2.h>
