#pragma once

#include <GenericPlatformMemory.h>

struct LinuxPlatformMemory : public GenericPlatformMemory
{
	// Alignmented
	static inline void* Malloc(SIZE_T Size, SIZE_T Alignment)
	{
		if(Size == 0)
			return nullptr;

		return ::aligned_alloc(Alignment, Size);
	}

	static inline void* Realloc(void* ptr, SIZE_T NewSize, SIZE_T Alignment)
	{
		if(ptr == nullptr || NewSize == 0)
			return nullptr;

		return realloc(ptr, NewSize);
	}
};

typedef LinuxPlatformMemory PFMemory;
