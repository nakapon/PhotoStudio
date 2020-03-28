#pragma once

template <typename T> struct TVec2;
template <typename T> struct TSize2;
template <typename T> struct TRegion2;

template <typename T> struct TMat33;

template <typename T> struct TRect2;

typedef TRect2<int> Rect2i;
typedef TRect2<float> Rect2f;
typedef TRect2<double> Rect2d;

template <typename T>
struct TRect2
{
public:
	T left, top, right, bottom;

public:
	TRect2()
		: left((T)0)
		, top((T)0)
		, right((T)0)
		, bottom((T)0)
	{ }

	TRect2(T l, T t, T r, T b)
		: left(l)
		, top(t)
		, right(r)
		, bottom(b)
	{ }

	TRect2(const TVec2<T> &c, T rx, T ry)
		: left(c.x - rx)
		, top(c.y - ry)
		, right(c.x + rx + (T)1)
		, bottom(c.y + ry + (T)1)
	{ }

	TRect2(const TRect2 &rc)
		: left(rc.left)
		, top(rc.top)
		, right(rc.right)
		, bottom(rc.bottom)
	{ }

	TRect2(const TSize2<T> &s)
		: left((T)0)
		, top((T)0)
		, right(s.w)
		, bottom(s.h)
	{ }

	TRect2(const TRegion2<T> &rgn)
		: left(rgn.x)
		, top(rgn.y)
		, right(rgn.x + rgn.w)
		, bottom(rgn.y + rgn.h)
	{ }

	void SetRect(T l, T t, T r, T b)
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}

	void SetRegion(T x, T y, T w, T h)
	{
		left = x;
		top = y;
		right = x + w;
		bottom = y + h;
	}

	void SetSize(T w, T h)
	{
		right = left + w;
		bottom = top + h;
	}

	// operator=
	TRect2& operator=(const TRect2& rc)
	{
		left = rc.left;
		top = rc.top;
		right = rc.right;
		bottom = rc.bottom;
		return *this;
	}

	TRect2& operator=(const TRegion2<T>& rgn)
	{
		left = rgn.x;
		top = rgn.y;
		right = rgn.x + rgn.w;
		bottom = rgn.y + rgn.h;
		return *this;
	}

	TRect2& operator=(const T& s)
	{
		left = (T)0;
		top = (T)0;
		right = s;
		bottom = s;
		return *this;
	}

	// operator+, operator+=
	TRect2& operator+(const TRect2& rc)
	{
		left = PFMath::Min<T>(left, rc.left);
		top = PFMath::Min<T>(top, rc.top);
		right = PFMath::Max<T>(right, rc.right);
		bottom = PFMath::Max<T>(bottom, rc.bottom);
		return *this;
	}

	TRect2& operator+=(const TRect2& rc)
	{
		left = PFMath::Min<T>(left, rc.left);
		top = PFMath::Min<T>(top, rc.top);
		right = PFMath::Max<T>(right, rc.right);
		bottom = PFMath::Max<T>(bottom, rc.bottom);
		return *this;
	}

	// operator*
	friend TRect2 operator*(const TMat33<T>& lhs, const TRect2& rhs)
	{
		TRect2 r;
		r.left   = lhs.m00 * rhs.left + lhs.m01 * rhs.top + lhs.m02;
		r.top    = lhs.m10 * rhs.left + lhs.m11 * rhs.top + lhs.m12;
		r.right  = lhs.m00 * rhs.right + lhs.m01 * rhs.bottom + lhs.m02;
		r.bottom = lhs.m10 * rhs.right + lhs.m11 * rhs.bottom + lhs.m12;
		return r;
	}

	// Methods
	T x() const
	{
		return left;
	}

	T y() const
	{
		return top;
	}

	T width() const
	{
		return right - left;
	}

	T height() const
	{
		return bottom - top;
	}

	TVec2<T> Point() const
	{
		return TVec2<T>(left, top);
	}

	TSize2<T> Size() const
	{
		return TSize2<T>(right - left, bottom - top);
	}

	TVec2<T> TopLeft() const
	{
		return TVec2<T>(left, top);
	}

	TVec2<T> BottomRight() const
	{
		return TVec2<T>(right, bottom);
	}

	TVec2<T> Center() const
	{
		return TVec2<T>((left + right) / (T)2, (top + bottom) / (T)2);
	}

	bool Contains(T x, T y) const
	{
		return (left <= x && x <= right && top <= y && y <= bottom);
	}

	bool Contains(const TVec2<T> &p) const
	{
		return (left <= p.x && p.x <= right && top <= p.y && p.y <= bottom);
	}

	bool Overlaps(const TRect2<T> &rc) const
	{
		TVec2<T> tl, br;
		tl.x = PFMath::Max<T>(left, rc.left);
		tl.y = PFMath::Max<T>(top, rc.top);
		br.x = PFMath::Min<T>(right, rc.right);
		br.y = PFMath::Min<T>(bottom, rc.bottom);
		return ((T)0 <= (br.x - tl.x)) && ((T)0 <= (br.y - tl.y));
	}

	T Area() const
	{
		return (right - left) * (bottom - top);
	}

	void MoveX(T x)
	{
		right = x + (right - left);
		left = x;
	}

	void MoveY(T y)
	{
		bottom = y + (bottom - top);
		top = y;
	}

	void MoveXY(T x, T y)
	{
		right = x + (right - left);
		left = x;
		bottom = y + (bottom - top);
		top = y;
	}

	void MoveXY(TVec2<T> &p)
	{
		right = p.x + (right - left);
		left = p.x;
		bottom = p.y + (bottom - top);
		top = p.y;
	}

	void Offset(T x, T y)
	{
		left += x;
		top += y;
		right += x;
		bottom += y;
	}

	void Offset(const TVec2<T> &p)
	{
		left += p.x;
		top += p.y;
		right += p.x;
		bottom += p.y;
	}

	void Offset(const TSize2<T> &s)
	{
		left += s.w;
		top += s.h;
		right += s.w;
		bottom += s.h;
	}

	void Normalize()
	{
		T tl = left, tt = top, tr = right, tb = bottom;
		left = PFMath::Min<T>(tl, tr);
		top = PFMath::Min<T>(tt, tb);
		right = PFMath::Max<T>(tl, tr);
		bottom = PFMath::Max<T>(tt, tb);
	}

	static TRect2<T> Intersect(const TRect2<T> rc1, const TRect2<T> rc2)
	{
		return TRect2<T>(PFMath::Max<T>(rc1.left, rc2.left), PFMath::Max<T>(rc1.top, rc2.top),
							PFMath::Min<T>(rc1.right, rc2.right), PFMath::Min<T>(rc1.bottom, rc2.bottom));
	}

	static TRect2<T> Union(const TRect2<T> rc1, const TRect2<T> rc2)
	{
		return TRect2<T>(PFMath::Min<T>(rc1.left, rc2.left), PFMath::Min<T>(rc1.top, rc2.top),
							PFMath::Max<T>(rc1.right, rc2.right), PFMath::Max<T>(rc1.bottom, rc2.bottom));
	}

	void Inflate(T s)
	{
		left -= s;
		top -= s;
		right += s;
		bottom += s;
	}

	void Inflate(T w, T h)
	{
		left -= w;
		top -= h;
		right += w;
		bottom += h;
	}

	void Inflate(T l, T t, T r, T b)
	{
		left -= l;
		top -= t;
		right += r;
		bottom += b;
	}

	void Deflate(T s)
	{
		left += s;
		top += s;
		right -= s;
		bottom -= s;
	}

	void Deflate(T w, T h)
	{
		left += w;
		top += h;
		right -= w;
		bottom -= h;
	}

	void Deflate(T l, T t, T r, T b)
	{
		left += l;
		top += t;
		right -= r;
		bottom -= b;
	}

	// WinDef RECT
	TRect2(const RECT &rc)
	{
		left = (T)rc.left;
		top = (T)rc.top;
		right = (T)rc.right;
		bottom = (T)rc.bottom;
	}

	operator RECT() const
	{
		RECT rc = {(LONG)left, (LONG)top, (LONG)right, (LONG)bottom};
		return rc;
	}

	TRect2& operator=(const RECT& rc)
	{
		left = (T)rc.left;
		top = (T)rc.top;
		right = (T)rc.right;
		bottom = (T)rc.bottom;
		return *this;
	}

	bool Contains(const POINT &p) const
	{
		return (left <= p.x && p.x <= right && top <= p.y && p.y <= bottom);
	}

	void MoveXY(POINT &p)
	{
		right = p.x + (right - left);
		left = p.x;
		bottom = p.y + (bottom - top);
		top = p.y;
	}

	void Offset(const POINT &p)
	{
		left += p.x;
		top += p.y;
		right += p.x;
		bottom += p.y;
	}

	void Offset(const SIZE &s)
	{
		left += s.cx;
		top += s.cy;
		right += s.cx;
		bottom += s.cy;
	}
};

#include <Math/Vec2.h>
#include <Math/Size2.h>
#include <Math/Region2.h>

static_assert(sizeof(Rect2i) == 16, "Rect2i size must be 16 bytes.");
static_assert(sizeof(Rect2f) == 16, "Rect2f size must be 16 bytes.");
static_assert(sizeof(Rect2d) == 32, "Rect2d size must be 32 bytes.");
