#pragma once

namespace Endian
{
	// WORD
	static inline WORD SwapWord(WORD wValue)
	{
		return ((0x00FFu & wValue) << 8)
			 | ((0xFF00u & wValue) >> 8);
	}

	static inline void SwapWordPtr(WORD *pwValue)
	{
		*pwValue = SwapWord(*pwValue);
	}

	// DWORD
	static inline DWORD SwapDword(DWORD dwValue)
	{
		return ((0x000000FFu & dwValue) << 24)
			 | ((0x0000FF00u & dwValue) <<  8)
			 | ((0x00FF0000u & dwValue) >>  8)
			 | ((0xFF000000u & dwValue) >> 24);
	}

	static inline void SwapDwordPtr(DWORD *pdwValue)
	{
		*pdwValue = SwapDword(*pdwValue);
	}

	// QWORD
	static inline QWORD SwapQword(QWORD qwValue)
	{
		return ((0x00000000000000FFull & qwValue) << 56)
			 | ((0x000000000000FF00ull & qwValue) << 40)
			 | ((0x0000000000FF0000ull & qwValue) << 24)
			 | ((0x00000000FF000000ull & qwValue) <<  8)
			 | ((0x000000FF00000000ull & qwValue) >>  8)
			 | ((0x0000FF0000000000ull & qwValue) >> 24)
			 | ((0x00FF000000000000ull & qwValue) >> 40)
			 | ((0xFF00000000000000ull & qwValue) >> 56);
	}

	static inline void SwapQwordPtr(QWORD *pqwValue)
	{
		*pqwValue = SwapQword(*pqwValue);
	}

} // namespace Endian
