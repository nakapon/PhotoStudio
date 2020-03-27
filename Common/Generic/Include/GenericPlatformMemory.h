#pragma once

struct GenericPlatformMemory
{
	// Aligned Memory
	static inline void* Malloc(SIZE_T Size, SIZE_T Alignment)
	{
		if(Size == 0)
			return nullptr;

		return malloc(Size);
	}

	static inline void* Realloc(void* ptr, SIZE_T NewSize, SIZE_T Alignment)
	{
		if(NewSize == 0)
			return nullptr;

		if(ptr == nullptr)
		{
			return malloc(NewSize);
		}
		else
		{
			return realloc(ptr, NewSize);
		}
	}

	static inline void Free(void* ptr)
	{
		if(ptr != nullptr)
		{
			free(ptr);
		}
	}

	static inline void* Move(void* pDst, const void* pSrc, SIZE_T Size)
	{
		return memmove(pDst, pSrc, Size);
	}

	static inline Int32 Compare(const void* pBuf1, const void* pBuf2, SIZE_T Size)
	{
		return memcmp(pBuf1, pBuf2, Size);
	}

	static inline void* Fill(void* pDst, UInt8 Value, SIZE_T Size)
	{
		return memset(pDst, Value, Size);
	}

	static inline void* Zero(void* pDst, SIZE_T Size)
	{
		return memset(pDst, 0, Size);
	}

	static inline void* Copy(void* pDst, const void* pSrc, SIZE_T Size)
	{
		return memcpy(pDst, pSrc, Size);
	}

	static inline void Swap(void* pDst, const void* pSrc, SIZE_T Size)
	{
		if(pDst == nullptr || pSrc == nullptr || pDst == pSrc || Size == 0)
			return;

		UInt8* pbySrc = (UInt8 *)pSrc;
		UInt8* pbyDst = (UInt8 *)pDst;

		while(Size--)
		{
			UInt8 byData = *pbySrc;
			*pbySrc++ = *pbyDst;
			*pbyDst++ = byData;
		}
	}

	static inline void Reverse(void* ptr, SIZE_T ElementCount, SIZE_T ElementSize)
	{
		if(ptr == nullptr || ElementCount < 1 || ElementSize == 0)
			return;

		UInt8* pbyHead = (UInt8 *)ptr;
		UInt8* pbyTail = (UInt8 *)ptr + (ElementCount - 1) * ElementSize;

		for(; pbyHead < pbyTail; pbyHead += ElementSize, pbyTail -= ElementSize)
		{
			GenericPlatformMemory::Swap(pbyHead, pbyTail, ElementSize);
		}
	}

	static inline void ShiftL(void* ptr, SIZE_T ElementCount, SIZE_T ElementSize, SIZE_T ShiftCount)
	{
		if(ptr == nullptr || ElementCount < 1 || ElementSize == 0 || ShiftCount == 0 || ElementCount <= ShiftCount)
			return;

		GenericPlatformMemory::Reverse(ptr, ShiftCount, ElementSize);
		GenericPlatformMemory::Reverse((UInt8 *)ptr + (ShiftCount * ElementSize), ElementCount - ShiftCount, ElementSize);
		GenericPlatformMemory::Reverse(ptr, ElementCount, ElementSize);
	}

	static inline void ShiftR(void* ptr, SIZE_T ElementCount, SIZE_T ElementSize, SIZE_T ShiftCount)
	{
		if(ptr == nullptr || ElementCount < 1 || ElementSize == 0 || ShiftCount == 0 || ElementCount <= ShiftCount)
			return;

		GenericPlatformMemory::Reverse(ptr, ElementCount, ElementSize);
		GenericPlatformMemory::Reverse(ptr, ShiftCount, ElementSize);
		GenericPlatformMemory::Reverse((UInt8 *)ptr + (ShiftCount * ElementSize), ElementCount - ShiftCount, ElementSize);
	}
};
