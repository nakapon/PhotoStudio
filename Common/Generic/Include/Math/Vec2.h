#pragma once

template <typename T> struct TMat22;
template <typename T> struct TMat33;

template <typename T> struct TVec2;

typedef TVec2<int> Vec2i;
typedef TVec2<float> Vec2f;
typedef TVec2<double> Vec2d;

template <typename T>
struct TVec2
{
public:
	T x, y;

public:
	TVec2()
		: x((T)0)
		, y((T)0)
	{ }

	TVec2(T xi, T yi)
		: x(xi)
		, y(yi)
	{ }

	TVec2(const TVec2& v)
		: x(v.x)
		, y(v.y)
	{ }

	void Set(T xi, T yi)
	{
		x = xi;
		y = yi;
	}

	// operator=
	TVec2& operator=(const TVec2& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	TVec2& operator=(const T& s)
	{
		x = y = s;
		return *this;
	}

	// operator+, operator+=
	friend TVec2 operator+(const TVec2& v1, const TVec2& v2)
	{
		return TVec2(v1.x + v2.x, v1.y + v2.y);
	}

	friend TVec2 operator+(const TVec2& v, const T& s)
	{
		return TVec2(v.x + s, v.y + s);
	}

	friend TVec2 operator+(const T& s, const TVec2& v)
	{
		return TVec2(s + v.x, s + v.y);
	}

	TVec2& operator+=(const TVec2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	TVec2& operator+=(const T& s)
	{
		x += s;
		y += s;
		return *this;
	}

	// operator-, operator-=
	friend TVec2 operator-(const TVec2& v1, const TVec2& v2)
	{
		return TVec2(v1.x - v2.x, v1.y - v2.y);
	}

	friend TVec2 operator-(const TVec2& v, const T& s)
	{
		return TVec2(v.x - s, v.y - s);
	}

	friend TVec2 operator-(const T& s, const TVec2& v)
	{
		return TVec2(s - v.x, s - v.y);
	}

	TVec2& operator-=(const TVec2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	TVec2& operator-=(const T& s)
	{
		x -= s;
		y -= s;
		return *this;
	}

	// operator*, operator*=
	friend TVec2 operator*(const TVec2& v1, const TVec2& v2)
	{
		return TVec2(v1.x * v2.x, v1.y * v2.y);
	}

	friend TVec2 operator*(const TVec2& v, const T& s)
	{
		return TVec2(v.x * s, v.y * s);
	}

	friend TVec2 operator*(const T& s, const TVec2& v)
	{
		return TVec2(s * v.x, s * v.y);
	}

	TVec2& operator*=(const TVec2& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	TVec2& operator*=(const T& s)
	{
		x *= s;
		y *= s;
		return *this;
	}

	friend TVec2 operator*(const TMat22<T>& lhs, const TVec2& rhs)
	{
		TVec2 v;
		v.x = lhs.m00 * rhs.x + lhs.m01 * rhs.y;
		v.y = lhs.m10 * rhs.x + lhs.m11 * rhs.y;
		return v;
	}

	friend TVec2 operator*(const TMat33<T>& lhs, const TVec2& rhs)
	{
		TVec2 v;
		v.x = lhs.m00 * rhs.x + lhs.m01 * rhs.y + lhs.m02;
		v.y = lhs.m10 * rhs.x + lhs.m11 * rhs.y + lhs.m12;
		return v;
	}

	// operator/, operator/=
	friend TVec2 operator/(const TVec2& v1, const TVec2& v2)
	{
		return TVec2(v1.x / v2.x, v1.y / v2.y);
	}

	friend TVec2 operator/(const TVec2& v, const T& s)
	{
		return TVec2(v.x / s, v.y / s);
	}

	friend TVec2 operator/(const T& s, const TVec2& v)
	{
		return TVec2(s / v.x, s / v.y);
	}

	TVec2& operator/=(const TVec2& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	TVec2& operator/=(const T& s)
	{
		x /= s;
		y /= s;
		return *this;
	}

	// Sign
	TVec2 operator+() const
	{
		return *this;
	}

	TVec2 operator-() const
	{
		return TVec2(-x, -y);
	}

	// operator==, operator!=
	friend bool operator==(const TVec2& v1, const TVec2& v2)
	{
		return v1.x == v2.x && v1.y == v2.y;
	}

	friend bool operator!=(const TVec2& v1, const TVec2& v2)
	{
		return v1.x != v2.x || v1.y != v2.y;
	}

	// operator[]
	const T& operator[](int idx) const
	{
		return *(&x + idx);
	}

	T& operator[](int idx)
	{
		return *(&x + idx);
	}

	// operator*
	operator T*()
	{
		return &x;
	}

	operator const T*() const
	{
		return &x;
	}

	// operator| (dot product)
	T operator|(const TVec2& v)
	{
		return Dot(v);
	}

	// Methods
	void SetZero()
	{
		x = y = (T)0;
	}

	void SetOne()
	{
		x = y = (T)1;
	}

	void Abs()
	{
		x = PFMath::Abs(x);
		y = PFMath::Abs(y);
	}

	bool IsZero() const
	{
		return x == (T)0 && y == (T)0;
	}

	bool HasZero() const
	{
		return x == (T)0 || y == (T)0;
	}

	bool IsZero(T eps) const
	{
		return PFMath::Abs(x) < eps && PFMath::Abs(y) < eps;
	}

	bool HasZero(T eps) const
	{
		return PFMath::Abs(x) < eps || PFMath::Abs(y) < eps;
	}

	T Len() const
	{
		return PFMath::Sqrt(LenSqr());
	}

	T LenSqr() const
	{
		return x * x + y * y;
	}

	T Distance(const TVec2& v) const
	{
		T dx = x - v.x;
		T dy = y - v.y;
		return PFMath::Sqrt(dx * dx + dy * dy);
	}

	T Dot(const TVec2& v) const
	{
		return x * v.x + y * v.y;
	}

	TVec2& Normalize()
	{
		T l = Len();
		x /= l;
		y /= l;
		return *this;
	}

	// WinDef POINT
	TVec2(const POINT& p)
	{
		x = (T)p.x;
		y = (T)p.y;
	}

	operator POINT() const
	{
		POINT pt = {(LONG)x, (LONG)y};
		return pt;
	}

	TVec2& operator=(const POINT& pt)
	{
		x = (T)pt.x;
		y = (T)pt.y;
		return *this;
	}

public:
	// Static Constants
	static const TVec2 One;
	static const TVec2 Zero;

	static const TVec2 UnitX;
	static const TVec2 UnitY;
};

static_assert(sizeof(Vec2i) == 8, "Vec2i size must be 8 bytes.");
static_assert(sizeof(Vec2f) == 8, "Vec2f size must be 8 bytes.");
static_assert(sizeof(Vec2d) == 16, "Vec2d size must be 16 bytes.");
