﻿#pragma once

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
};