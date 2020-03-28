#pragma once

template <typename T> struct TVec2;
template <typename T> struct TSize2;
template <typename T> struct TRect2;

template <typename T> struct TRegion2;

typedef TRegion2<int> Region2i;
typedef TRegion2<float> Region2f;
typedef TRegion2<double> Region2d;

template <typename T>
struct TRegion2
{
public:
	T x, y, w, h;

public:
	TRegion2()
		: x((T)0)
		, y((T)0)
		, w((T)0)
		, h((T)0)
	{ }

	TRegion2(T nx, T ny, T nw, T nh)
		: x(nx)
		, y(ny)
		, w(nw)
		, h(nh)
	{ }

	TRegion2(const TRegion2 &rgn)
		: x(rgn.x)
		, y(rgn.y)
		, w(rgn.w)
		, h(rgn.h)
	{ }

	TRegion2(const TSize2<T> &sz)
		: x((T)0)
		, y((T)0)
		, w(sz.w)
		, h(sz.h)
	{ }

	TRegion2(const TRect2<T> &rc)
		: x(rc.left)
		, y(rc.top)
		, w(rc.right - rc.left)
		, h(rc.bottom - rc.top)
	{ }

	void SetRegion(T rx, T ry, T rw, T rh)
	{
		x = rx;
		y = ry;
		w = rw;
		h = rh;
	}

	void SetRect(T l, T t, T r, T b)
	{
		x = l;
		y = t;
		w = r - l;
		h = b - t;
	}

	// operator=
	TRegion2& operator=(const TRegion2& rgn)
	{
		x = rgn.x;
		y = rgn.y;
		w = rgn.w;
		h = rgn.h;
		return *this;
	}

	TRegion2& operator=(const TRect2<T>& rc)
	{
		x = rc.left;
		y = rc.top;
		w = rc.right - rc.left;
		h = rc.bottom - rc.top;
		return *this;
	}

	TRegion2& operator=(const T& s)
	{
		x = 0;
		y = 0;
		w = s;
		h = s;
		return *this;
	}

	// operator+, operator+=
	TRegion2& operator+(const TRegion2& rgn)
	{
		T l, t, r, b;
		l = PFMath::Min<T>(x, rgn.x);
		t = PFMath::Min<T>(y, rgn.y);
		r = PFMath::Max<T>(x + w, rgn.x + w);
		b = PFMath::Max<T>(y + h, rgn.y + h);
		x = l;
		y = t;
		w = r - l;
		h = b - t;
		return *this;
	}

	TRegion2& operator+=(const TRegion2& rgn)
	{
		T l, t, r, b;
		l = PFMath::Min<T>(x, rgn.x);
		t = PFMath::Min<T>(y, rgn.y);
		r = PFMath::Max<T>(x + w, rgn.x + w);
		b = PFMath::Max<T>(y + h, rgn.y + h);
		x = l;
		y = t;
		w = r - l;
		h = b - t;
		return *this;
	}

	// Methods
	TVec2<T> Point() const
	{
		return TVec2<T>(x, y);
	}

	TSize2<T> Size() const
	{
		return TSize2<T>(w, h);
	}

	bool Contains(T tx, T ty) const
	{
		return (x <= tx && tx < (x + w) && y <= ty && ty <= (y + h));
	}

	bool Contains(const TVec2<T> &pt) const
	{
		return (x <= pt.x && pt.x < (x + w) && y <= pt.y && pt.y <= (y + h));
	}

	bool Overlaps(const TRegion2<T> &rgn) const
	{
		TVec2<T> tl, br;
		tl.x = PFMath::Max<T>(x, rgn.x);
		tl.y = PFMath::Max<T>(y, rgn.y);
		br.x = PFMath::Min<T>(x + w, rgn.x + rgn.w);
		br.y = PFMath::Min<T>(y + h, rgn.y + rgn.h);
		return ((T)0 < (br.x - tl.x)) && ((T)0 < (br.y - tl.y));
	}

	T Area() const
	{
		return w * h;
	}

	// WinDef POINT
	bool Contains(const POINT &p) const
	{
		return (x <= p.x && p.x < (x + w) && y <= p.y && p.y < (y + h));
	}

	void MoveXY(POINT &p)
	{
		x = p.x;
		y = p.y;
	};

	void Offset(const POINT &p)
	{
		x += p.x;
		y += p.y;
	}

	void Offset(const SIZE &s)
	{
		x += s.cx;
		y += s.cy;
	}
};

#include <Math/Vec2.h>
#include <Math/Size2.h>
#include <Math/Rect2.h>

static_assert(sizeof(Region2i) == 16, "Region2i size must be 16 bytes.");
static_assert(sizeof(Region2f) == 16, "Region2f size must be 16 bytes.");
static_assert(sizeof(Region2d) == 32, "Region2d size must be 32 bytes.");
