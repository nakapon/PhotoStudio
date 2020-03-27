#pragma once

struct GenericPlatformMath
{
	// Abs
	template <typename T>
	static inline T Abs(T x)
	{
		return (0 <= x) ? x : -x;
	}

	// Square
	template <typename T>
	static inline T Square(T x)
	{
		return x * x;
	}

	// Cube
	template <typename T>
	static inline T Cube(T x)
	{
		return x * x * x;
	}

	// Max / Min
	template <typename T>
	static inline T Max(T a, T b)
	{
		return (a < b) ? b : a;
	}

	template <typename T>
	static inline T Min(T a, T b)
	{
		return (a < b) ? a : b;
	}

	template <typename T>
	static inline T Max(T a, T b, T c)
	{
		return Max(a, Max(b, c));
	}

	template <typename T>
	static inline T Min(T a, T b, T c)
	{
		return Min(a, Min(b, c));
	}

	// Clamp
	template <typename T>
	static inline T Clamp(T x, T min, T max)
	{
		return Max(min, Min(max, x));
	}

	static inline Int32 MulDiv(Int32 Number, Int32 Numerator, Int32 Denominator)
	{
		Int64 Result;

		if(Denominator == 0)
			return -1;

		Result = Number;
		Result *= Numerator;
		Result += (Denominator >> 1);
		Result /= Denominator;

		return (Int32)Result;
	}

	// Floor Log2
	static UInt32 FloorLog2U32(UInt32 value)
	{
		UInt32 pos = 0;
		if((1 << 16) <= value){ value >>= 16; pos += 16; }
		if((1 <<  8) <= value){ value >>=  8; pos +=  8; }
		if((1 <<  4) <= value){ value >>=  4; pos +=  4; }
		if((1 <<  2) <= value){ value >>=  2; pos +=  2; }
		if((1 <<  1) <= value){               pos +=  1; }
		return (value == 0) ? 0 : pos;
	}

	static UInt64 FloorLog2U64(UInt64 value)
	{
		UInt64 pos = 0;
		if((1ull << 32) <= value){ value >>= 32; pos += 32; }
		if((1ull << 16) <= value){ value >>= 16; pos += 16; }
		if((1ull <<  8) <= value){ value >>=  8; pos +=  8; }
		if((1ull <<  4) <= value){ value >>=  4; pos +=  4; }
		if((1ull <<  2) <= value){ value >>=  2; pos +=  2; }
		if((1ull <<  1) <= value){               pos +=  1; }
		return (value == 0) ? 0 : pos;
	}

	// CeilLog2
	static UInt32 CeilLog2U32(UInt32 arg)
	{
		return FloorLog2U32(arg) + 1;
	}

	static UInt64 CeilLog2U64(UInt64 arg)
	{
		return FloorLog2U64(arg) + 1;
	}

	// RoundUpPowerOfTow
	static UInt32 RoundUpPowerOfTowU32(UInt32 arg)
	{
		return 1 << CeilLog2U32(arg);
	}

	static UInt64 RoundUpPowerOfTowU64(UInt64 arg)
	{
		return 1ull << CeilLog2U64(arg);
	}

};
