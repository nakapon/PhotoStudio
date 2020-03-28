#pragma once

/* Row-major order 3x3 matrix */

template <typename T> struct TMat33;

typedef TMat33<float> Mat33f;
typedef TMat33<double> Mat33d;

template <typename T>
struct TMat33
{
public:
	enum type_zero { ZERO };
	enum type_identity { IDENTITY };

	union
	{
		T m[9];
		T d[3][3];
		struct
		{
			T m00, m01, m02;
			T m10, m11, m12;
			T m20, m21, m22;
		};
	};

public:
	TMat33()
	{ }

	TMat33(type_zero)
	{
		m00 = (T)0; m01 = (T)0; m02 = (T)0;
		m10 = (T)0; m11 = (T)0; m12 = (T)0;
		m20 = (T)0; m21 = (T)0; m22 = (T)0;
	}

	TMat33(type_identity)
	{
		m00 = (T)1; m01 = (T)0; m02 = (T)0;
		m10 = (T)0; m11 = (T)1; m12 = (T)0;
		m20 = (T)0; m21 = (T)0; m22 = (T)1;
	}

	TMat33(const TMat33& mat)
	{
		for(int i = 0; i < 9; i++)
		{
			m[i] = mat.m[i];
		}
	}

	TMat33(T _m00, T _m01, T _m02
		 , T _m10, T _m11, T _m12
		 , T _m20, T _m21, T _m22)
	{
		m00 = _m00; m01 = _m01; m02 = _m02;
		m10 = _m10; m11 = _m11; m12 = _m12;
		m20 = _m20; m21 = _m21; m22 = _m22;
	}

	void Set(T _m00, T _m01, T _m02,
			 T _m10, T _m11, T _m12,
			 T _m20, T _m21, T _m22)
	{
		m00 = _m00; m01 = _m01; m02 = _m02;
		m10 = _m10; m11 = _m11; m12 = _m12;
		m20 = _m20; m21 = _m21; m22 = _m22;
	}

	// operator+, operator+=
	friend TMat33 operator+(const TMat33& lhs, const TMat33& rhs)
	{
		TMat33 mat;
		mat.m00 = lhs.m00 + rhs.m00;
		mat.m01 = lhs.m01 + rhs.m01;
		mat.m02 = lhs.m02 + rhs.m02;
		mat.m10 = lhs.m10 + rhs.m10;
		mat.m11 = lhs.m11 + rhs.m11;
		mat.m12 = lhs.m12 + rhs.m12;
		mat.m20 = lhs.m20 + rhs.m20;
		mat.m21 = lhs.m21 + rhs.m21;
		mat.m22 = lhs.m22 + rhs.m22;
		return mat;
	}

	TMat33& operator+=(const TMat33& rhs)
	{
		TMat33 mat;
		mat.m00 += rhs.m00;
		mat.m01 += rhs.m01;
		mat.m02 += rhs.m02;
		mat.m10 += rhs.m10;
		mat.m11 += rhs.m11;
		mat.m12 += rhs.m12;
		mat.m20 += rhs.m20;
		mat.m21 += rhs.m21;
		mat.m22 += rhs.m22;
		return *this;
	}

	// operator-, operator-=
	friend TMat33 operator-(const TMat33& lhs, const TMat33& rhs)
	{
		TMat33 mat;
		mat.m00 = lhs.m00 - rhs.m00;
		mat.m01 = lhs.m01 - rhs.m01;
		mat.m02 = lhs.m02 - rhs.m02;
		mat.m10 = lhs.m10 - rhs.m10;
		mat.m11 = lhs.m11 - rhs.m11;
		mat.m12 = lhs.m12 - rhs.m12;
		mat.m20 = lhs.m20 - rhs.m20;
		mat.m21 = lhs.m21 - rhs.m21;
		mat.m22 = lhs.m22 - rhs.m22;
		return mat;
	}

	TMat33& operator-=(const TMat33& rhs)
	{
		TMat33 mat;
		mat.m00 -= rhs.m00;
		mat.m01 -= rhs.m01;
		mat.m02 -= rhs.m02;
		mat.m10 -= rhs.m10;
		mat.m11 -= rhs.m11;
		mat.m12 -= rhs.m12;
		mat.m20 -= rhs.m20;
		mat.m21 -= rhs.m21;
		mat.m22 -= rhs.m22;
		return *this;
	}

	// operator*, operator*=
	friend TMat33 operator*(const TMat33& lhs, const TMat33& rhs)
	{
		TMat33 mat;
		mat.m00 = lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10 + lhs.m02 * rhs.m20;
		mat.m01 = lhs.m00 * rhs.m01 + lhs.m01 * rhs.m11 + lhs.m02 * rhs.m21;
		mat.m02 = lhs.m00 * rhs.m02 + lhs.m01 * rhs.m12 + lhs.m02 * rhs.m22;
		mat.m10 = lhs.m10 * rhs.m00 + lhs.m11 * rhs.m10 + lhs.m12 * rhs.m20;
		mat.m11 = lhs.m10 * rhs.m01 + lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21;
		mat.m12 = lhs.m10 * rhs.m02 + lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22;
		mat.m20 = lhs.m20 * rhs.m00 + lhs.m21 * rhs.m10 + lhs.m22 * rhs.m20;
		mat.m21 = lhs.m20 * rhs.m01 + lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21;
		mat.m22 = lhs.m20 * rhs.m02 + lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22;
		return mat;
	}

	TMat33& operator*=(const TMat33& rhs)
	{
		TMat33 lhs = *this;
		m00 = lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10 + lhs.m02 * rhs.m20;
		m01 = lhs.m00 * rhs.m01 + lhs.m01 * rhs.m11 + lhs.m02 * rhs.m21;
		m02 = lhs.m00 * rhs.m02 + lhs.m01 * rhs.m12 + lhs.m02 * rhs.m22;
		m10 = lhs.m10 * rhs.m00 + lhs.m11 * rhs.m10 + lhs.m12 * rhs.m20;
		m11 = lhs.m10 * rhs.m01 + lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21;
		m12 = lhs.m10 * rhs.m02 + lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22;
		m20 = lhs.m20 * rhs.m00 + lhs.m21 * rhs.m10 + lhs.m22 * rhs.m20;
		m21 = lhs.m20 * rhs.m01 + lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21;
		m22 = lhs.m20 * rhs.m02 + lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22;
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
		TMat33 mat = *this;

		// calculate the cofactor-matrix (=transposed adjoint-matrix)
		m00 = mat.m22 * mat.m11 - mat.m12 * mat.m21;
		m01 = mat.m02 * mat.m21 - mat.m22 * mat.m01;
		m02 = mat.m12 * mat.m01 - mat.m02 * mat.m11;
		m10 = mat.m12 * mat.m20 - mat.m22 * mat.m10;
		m11 = mat.m22 * mat.m00 - mat.m02 * mat.m20;
		m12 = mat.m02 * mat.m10 - mat.m12 * mat.m00;
		m20 = mat.m10 * mat.m21 - mat.m20 * mat.m11;
		m21 = mat.m20 * mat.m01 - mat.m00 * mat.m21;
		m22 = mat.m00 * mat.m11 - mat.m10 * mat.m01;

		// calculate determinant
		T det = (mat.m00 * m00 + mat.m10 * m01 + mat.m20 * m02);
		if(fabs(det) < 1E-20f)
			return false;

		//divide the cofactor-matrix by the determinant
		T idet = (T)1.0 / det;
		m00 *= idet; m01 *= idet; m02 *= idet;
		m10 *= idet; m11 *= idet; m12 *= idet;
		m20 *= idet; m21 *= idet; m22 *= idet;

		return true;
	}

	TMat33 GetInverted() const
	{
		TMat33 mat = *this;
		mat.Invert();
		return mat;
	}

	TMat33 GetTransposed() const
	{
		TMat33 mat;
		mat.m00 = m00; mat.m01 = m10; mat.m02 = m20;
		mat.m10 = m01; mat.m11 = m11; mat.m12 = m21;
		mat.m20 = m02; mat.m21 = m12; mat.m22 = m22;
		return mat;
	}

public:
	static const TMat33 Identity;
};

static_assert(sizeof(Mat33f) == 36, "Mat33f size must be 36 bytes.");
static_assert(sizeof(Mat33d) == 72, "Mat33d size must be 72 bytes.");
