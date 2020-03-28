#pragma once

template <typename T> struct TVec2;
template <typename T> struct TMat33;

template <typename T> struct TVec3;

typedef TVec3<int> Vec3i;
typedef TVec3<float> Vec3f;
typedef TVec3<double> Vec3d;

template <typename T>
struct TVec3
{
public:
	T x, y, z;

public:
	TVec3()
		: x((T)0)
		, y((T)0)
		, z((T)0)
	{ }

	TVec3(T xi, T yi, T zi)
		: x(xi)
		, y(yi)
		, z(zi)
	{ }

	TVec3(const TVec3& v)
		: x(v.x)
		, y(v.y)
		, z(v.z)
	{ }

	TVec3(const TVec2<T>& v, T zi)
		: x(v.x)
		, y(v.y)
		, z(zi)
	{ }

	void Set(T xi, T yi, T zi)
	{
		x = xi;
		y = yi;
		z = zi;
	}

	// operator=
	TVec3& operator=(const TVec3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	TVec3& operator=(const T& s)
	{
		x = y = z = s;
		return *this;
	}

	TVec3& operator=(const TVec2<T>& v)
	{
		x = v.x;
		y = v.y;
		z = (T)0;
		return *this;
	}

	// operator+, operator+=
	friend TVec3 operator+(const TVec3& v1, const TVec3& v2)
	{
		return TVec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	}

	friend TVec3 operator+(const TVec3& v, const T& s)
	{
		return TVec3(v.x + s, v.y + s, v.z + s);
	}

	friend TVec3 operator+(const T& s, const TVec3& v)
	{
		return TVec3(s + v.x, s + v.y, s + v.z);
	}

	TVec3& operator+=(const TVec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	TVec3& operator+=(const T& s)
	{
		x += s;
		y += s;
		z += s;
		return *this;
	}

	// operator-, operator-=
	friend TVec3 operator-(const TVec3& v1, const TVec3& v2)
	{
		return TVec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
	}

	friend TVec3 operator-(const TVec3& v, const T& s)
	{
		return TVec3(v.x - s, v.y - s, v.z - s);
	}

	friend TVec3 operator-(const T& s, const TVec3& v)
	{
		return TVec3(s - v.x, s - v.y, s - v.z);
	}

	TVec3& operator-=(const TVec3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	TVec3& operator-=(const T& s)
	{
		x -= s;
		y -= s;
		z -= s;
		return *this;
	}

	// operator*, operator*=
	friend TVec3 operator*(const TVec3& v1, const TVec3& v2)
	{
		return TVec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
	}

	friend TVec3 operator*(const TVec3& v, const T& s)
	{
		return TVec3(v.x * s, v.y * s, v.z * s);
	}

	friend TVec3 operator*(const T& s, const TVec3& v)
	{
		return TVec3(s * v.x, s * v.y, s * v.z);
	}

	TVec3& operator*=(const TVec3& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	TVec3& operator*=(const T& s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	friend TVec3 operator*(const TMat33<T>& lhs, const TVec3& rhs)
	{
		TVec3 v;
		v.x = lhs.m00 * rhs.x + lhs.m01 * rhs.y + lhs.m02 * rhs.z;
		v.y = lhs.m10 * rhs.x + lhs.m11 * rhs.y + lhs.m12 * rhs.z;
		v.z = lhs.m20 * rhs.x + lhs.m21 * rhs.y + lhs.m22 * rhs.z;
		return v;
	}

	// operator/, operator/=
	friend TVec3 operator/(const TVec3& v1, const TVec3& v2)
	{
		return TVec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
	}

	friend TVec3 operator/(const TVec3& v, const T& s)
	{
		return TVec3(v.x / s, v.y / s, v.z / s);
	}

	friend TVec3 operator/(const T& s, const TVec3& v)
	{
		return TVec3(s / v.x, s / v.y, s / v.z);
	}

	TVec3& operator/=(const TVec3& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	TVec3& operator/=(const T& s)
	{
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}

	// Sign
	TVec3 operator+() const
	{
		return *this;
	}

	TVec3 operator-() const
	{
		return TVec3(-x, -y, -z);
	}

	// operator==, operator!=
	friend bool operator==(const TVec3& v1, const TVec3& v2)
	{
		return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
	}

	friend bool operator!=(const TVec3& v1, const TVec3& v2)
	{
		return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z;
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
	T operator|(const TVec3& v)
	{
		return Dot(v);
	}

	// operator^ (cross product)
	T operator^(const TVec3& v)
	{
		return Cross(v);
	}

	// Methods
	void SetZero()
	{
		x = y = z = (T)0;
	}

	void SetOne()
	{
		x = y = z = (T)1;
	}

	void Abs()
	{
		x = PFMath::Abs(x);
		y = PFMath::Abs(y);
		z = PFMath::Abs(z);
	}

	bool IsZero() const
	{
		return x == (T)0 && y == (T)0 && z == (T)0;
	}

	bool HasZero() const
	{
		return x == (T)0 || y == (T)0 || z == (T)0;
	}

	bool IsZero(T eps) const
	{
		return PFMath::Abs(x) < eps && PFMath::Abs(y) < eps && PFMath::Abs(z) < eps;
	}

	bool HasZero(T eps) const
	{
		return PFMath::Abs(x) < eps || PFMath::Abs(y) < eps || PFMath::Abs(z) < eps;
	}

	T Len() const
	{
		return sqrt(LenSqr());
	}

	T LenSqr() const
	{
		return x * x + y * y + z * z;
	}

	T Distance(const TVec3& v) const
	{
		T dx = x - v.x;
		T dy = y - v.y;
		T dz = z - v.z;
		return sqrt(dx * dx + dy * dy + dz * dz);
	}

	T Dot(const TVec3& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	TVec3 Cross(const TVec3& v) const
	{
		return TVec3(y * v.z - z * v.y,
					 z * v.x - x * v.z,
					 x * v.y - y * v.x);
	}

	TVec3& Normalize()
	{
		T l = Len();
		x /= l;
		y /= l;
		z /= l;
		return *this;
	}

public:
	// Static Constants
	static const TVec3 One;
	static const TVec3 Zero;

	static const TVec3 UnitX;
	static const TVec3 UnitY;
	static const TVec3 UnitZ;
};

static_assert(sizeof(Vec3i) == 12, "Vec3i size must be 12 bytes.");
static_assert(sizeof(Vec3f) == 12, "Vec3f size must be 12 bytes.");
static_assert(sizeof(Vec3d) == 24, "Vec3d size must be 24 bytes.");
