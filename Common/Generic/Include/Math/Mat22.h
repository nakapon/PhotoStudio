#pragma once

/* Row-major order 2x2 matrix */

template <typename T> struct TMat22;

typedef TMat22<float> Mat22f;
typedef TMat22<double> Mat22d;

template <typename T>
struct TMat22
{
public:
	enum type_zero { ZERO };
	enum type_identity { IDENTITY };

	union
	{
		T m[4];
		T d[2][2];
		struct
		{
			T m00, m01;
			T m10, m11;
		};
	};

public:
	TMat22()
	{ }

	TMat22(type_zero)
	{
		m00 = (T)0; m01 = (T)0;
		m10 = (T)0; m11 = (T)0;
	}

	TMat22(type_identity)
	{
		m00 = (T)1; m01 = (T)0;
		m10 = (T)0; m11 = (T)1;
	}

	TMat22(const TMat22& mat)
	{
		for(int i = 0; i < 4; i++)
		{
			m[i] = mat.m[i];
		}
	}

	TMat22(T _m00, T _m01
		 , T _m10, T _m11)
	{
		m00 = _m00; m01 = _m01;
		m10 = _m10; m11 = _m11;
	}

	void Set(T _m00, T _m01,
			 T _m10, T _m11)
	{
		m00 = _m00; m01 = _m01;
		m10 = _m10; m11 = _m11;
	}

	// operator+, operator+=
	friend TMat22 operator+(const TMat22& lhs, const TMat22& rhs)
	{
		TMat22 mat;
		mat.m00 = lhs.m00 + rhs.m00;
		mat.m01 = lhs.m01 + rhs.m01;
		mat.m10 = lhs.m10 + rhs.m10;
		mat.m11 = lhs.m11 + rhs.m11;
		return mat;
	}

	TMat22& operator+=(const TMat22& rhs)
	{
		TMat22 mat;
		mat.m00 += rhs.m00;
		mat.m01 += rhs.m01;
		mat.m10 += rhs.m10;
		mat.m11 += rhs.m11;
		return *this;
	}

	// operator-, operator-=
	friend TMat22 operator-(const TMat22& lhs, const TMat22& rhs)
	{
		TMat22 mat;
		mat.m00 = lhs.m00 - rhs.m00;
		mat.m01 = lhs.m01 - rhs.m01;
		mat.m10 = lhs.m10 - rhs.m10;
		mat.m11 = lhs.m11 - rhs.m11;
		return mat;
	}

	TMat22& operator-=(const TMat22& rhs)
	{
		TMat22 mat;
		mat.m00 -= rhs.m00;
		mat.m01 -= rhs.m01;
		mat.m10 -= rhs.m10;
		mat.m11 -= rhs.m11;
		return *this;
	}

	// operator*, operator*=
	friend TMat22 operator*(const TMat22& lhs, const TMat22& rhs)
	{
		TMat22 mat;
		mat.m00 = lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10;
		mat.m01 = lhs.m00 * rhs.m01 + lhs.m01 * rhs.m11;
		mat.m10 = lhs.m10 * rhs.m00 + lhs.m11 * rhs.m10;
		mat.m11 = lhs.m10 * rhs.m01 + lhs.m11 * rhs.m11;
		return mat;
	}

	TMat22& operator*=(const TMat22& rhs)
	{
		TMat22 lhs = *this;
		m00 = lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10;
		m01 = lhs.m00 * rhs.m01 + lhs.m01 * rhs.m11;
		m10 = lhs.m10 * rhs.m00 + lhs.m11 * rhs.m10;
		m11 = lhs.m10 * rhs.m01 + lhs.m11 * rhs.m11;
		return *this;
	}

	// operator[]
	const T& operator[](int idx) const
	{
		return m[idx];
	}

	T& operator[](int idx)
	{
		return m[idx];
	}

	// operator*
	operator T*()
	{
		return &m[0];
	}

	operator const T*() const
	{
		return &m[0];
	}

	bool Invert()
	{
		TMat22 mat = *this;

		// calculate the cofactor-matrix (=transposed adjoint-matrix)
		m00 =   mat.m11;
		m01 = - mat.m01;
		m10 = - mat.m10;
		m11 =   mat.m00;

		// calculate determinant
		T det = (mat.m00 * mat.m11 - mat.m01 * mat.m10);
		if(fabs(det) < 1E-20f)
			return false;

		//divide the cofactor-matrix by the determinant
		T idet = (T)1.0 / det;
		m00 *= idet; m01 *= idet;
		m10 *= idet; m11 *= idet;

		return true;
	}

	TMat22 GetInverted() const
	{
		TMat22 mat = *this;
		mat.Invert();
		return mat;
	}

	TMat22 GetTransposed() const
	{
		TMat22 mat;
		mat.m00 = m00; mat.m01 = m10;
		mat.m10 = m01; mat.m11 = m11;
		return mat;
	}

public:
	static const TMat22 Identity;
};

static_assert(sizeof(Mat22f) == 16, "Mat22f size must be 16 bytes.");
static_assert(sizeof(Mat22d) == 32, "Mat22d size must be 32 bytes.");
