#pragma once

template <typename T> struct TRect2;
template <typename T> struct TRegion2;

template <typename T> struct TSize2;

typedef TSize2<int> Size2i;
typedef TSize2<float> Size2f;
typedef TSize2<double> Size2d;

template <typename T>
struct TSize2
{
public:
	T w, h;

public:
	TSize2()
		: w((T)0)
		, h((T)0)
	{ }

	TSize2(T wi, T hi)
		: w(wi)
		, h(hi)
	{ }

	TSize2(const TSize2 &s)
		: w(s.w)
		, h(s.h)
	{ }

	TSize2(const TRect2<T> &rc)
		: w(rc.right - rc.left)
		, h(rc.bottom - rc.top)
	{ }

	TSize2(const TRegion2<T> &rgn)
		: w(rgn.w)
		, h(rgn.h)
	{ }

	void Set(T wi, T hi)
	{
		w = wi;
		h = hi;
	}

	// operator=
	TSize2& operator=(const TSize2& s)
	{
		w = s.w;
		h = s.h;
		return *this;
	}

	TSize2& operator=(const T& s)
	{
		w = h = s;
		return *this;
	}

	// operator+, operator+=
	friend TSize2 operator+(const TSize2& sz1, const TSize2& sz2)
	{
		return TSize2(sz1.w + sz2.w, sz1.h + sz2.h);
	}

	friend TSize2 operator+(const TSize2& sz, const T& s)
	{
		return TSize2(sz.w + s, sz.h + s);
	}

	friend TSize2 operator+(const T& s, const TSize2& sz)
	{
		return TSize2(s + sz.w, s + sz.h);
	}

	TSize2& operator+=(const TSize2& sz)
	{
		w += sz.w;
		h += sz.h;
		return *this;
	}

	TSize2& operator+=(const T& s)
	{
		w += s;
		h += s;
		return *this;
	}

	// operator-, operator-=
	friend TSize2 operator-(const TSize2& sz1, const TSize2& sz2)
	{
		return TSize2(sz1.w - sz2.w, sz1.h - sz2.h);
	}

	friend TSize2 operator-(const TSize2& sz, const T& s)
	{
		return TSize2(sz.w - s, sz.h - s);
	}

	friend TSize2 operator-(const T& s, const TSize2& sz)
	{
		return TSize2(s - sz.w, s - sz.h);
	}

	TSize2& operator-=(const TSize2& sz)
	{
		w -= sz.w;
		h -= sz.h;
		return *this;
	}

	TSize2& operator-=(const T& s)
	{
		w -= s;
		h -= s;
		return *this;
	}

	// operator*, operator*=
	friend TSize2 operator*(const TSize2& sz1, const TSize2& sz2)
	{
		return TSize2(sz1.w * sz2.w, sz1.h * sz2.h);
	}

	friend TSize2 operator*(const TSize2& sz, const T& s)
	{
		return TSize2(sz.w * s, sz.h * s);
	}

	friend TSize2 operator*(const T& s, const TSize2& sz)
	{
		return TSize2(sz.w * s, sz.h * s);
	}

	TSize2& operator*=(const TSize2& sz)
	{
		w *= sz.w;
		h *= sz.h;
		return *this;
	}

	TSize2& operator*=(T s)
	{
		w *= s;
		h *= s;
		return *this;
	}

	// operator/, operator/=
	friend TSize2 operator/(const TSize2& sz1, const TSize2& sz2)
	{
		return TSize2(sz1.w / sz2.w, sz1.h / sz2.h);
	}

	friend TSize2 operator/(const TSize2& sz, const T& s)
	{
		return TSize2(sz.w / s, sz.h / s);
	}

	friend TSize2 operator/(const T& s, const TSize2& sz)
	{
		return TSize2(s / sz.w, s / sz.h);
	}

	TSize2& operator/=(const TSize2& sz)
	{
		w /= sz.w;
		h /= sz.h;
		return *this;
	}

	TSize2& operator/=(T s)
	{
		w /= s;
		h /= s;
		return *this;
	}

	// Methods
	T Area() const
	{
		return w * h;
	}

	// WINDEF SIZE
	TSize2(const SIZE &s)
	{
		w = s.cx;
		h = s.cy;
	}

	operator SIZE() const
	{
		SIZE s = {w, h};
		return s;
	}

	TSize2& operator=(const SIZE& s)
	{
		w = s.cx;
		h = s.cy;
		return *this;
	}
};

#include <Math/Rect2.h>
#include <Math/Region2.h>

static_assert(sizeof(Size2i) == 8, "Size2i size must be 8 bytes.");
static_assert(sizeof(Size2f) == 8, "Size2f size must be 8 bytes.");
static_assert(sizeof(Size2d) == 16, "Size2d size must be 16 bytes.");
