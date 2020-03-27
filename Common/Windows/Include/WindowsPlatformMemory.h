#pragma once

#include <GenericPlatformMemory.h>

struct WindowsPlatformMemory : public GenericPlatformMemory
{
	// Aligned Memory
	static inline void* Malloc(SIZE_T Size, SIZE_T Alignment)
	{
		if(Size == 0)
			return nullptr;

		return ::_aligned_malloc(Size, Alignment);
	}

	static inline void* Realloc(void* ptr, SIZE_T NewSize, SIZE_T Alignment)
	{
		if(NewSize == 0)
			return nullptr;

		if(ptr == nullptr)
		{
			return ::_aligned_malloc(NewSize, Alignment);
		}
		else
		{
			return ::_aligned_realloc(ptr, NewSize, Alignment);
		}
	}

	static inline void Free(void* ptr)
	{
		if(ptr != nullptr)
		{
			::_aligned_free(ptr);
		}
	}
};

typedef WindowsPlatformMemory PFMemory;
