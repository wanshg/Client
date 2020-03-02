#ifndef _MATRIX_H_
#define _MATRIX_H_
//#include "stdafx.h"
#include "Vectors.h"
#include <iostream>
#include <iomanip>
#include <windowsx.h>
#include <assert.h>
#include <math.h>
#include "HMath.h"


enum class MVP_MATRIX_TYPE
{
	MATRIX_STACK_MODELVIEW,
	MATRIX_STACK_PROJECTION
};

class Matrix4;
class Matrix2;
class Matrix3;

static inline unsigned int GetSafe(unsigned int uiOrder) { static unsigned int aiSafe[] = { 0, 1, 2, 0 }; return(aiSafe[uiOrder & 0x3]); }
static inline unsigned int GetNext(unsigned int uiOrder) { static unsigned int aiNext[] = { 1, 2, 0, 1 }; return(aiNext[uiOrder & 0x3]); }



static inline void         GetOrderData(unsigned int uiOrder, int &i, int &j, int &k, int &h, int &n, int &s, int &f)
{
	f = uiOrder & 0x1; uiOrder >>= 1;
	s = uiOrder & 0x1; uiOrder >>= 1;
	n = uiOrder & 0x1; uiOrder >>= 1;
	i = GetSafe(uiOrder & 0x3);
	j = GetNext(i + n);
	k = GetNext(i + 1 - n);
	h = s ? k : i;
}

#define GETEULERORDER( i, p, r, f ) ( ( ( ( ( ( i << 1 ) + p ) << 1 ) + r ) << 1 ) + f )
class EulerAngle
{
public:

	enum EULERDEFS
	{
		X = 0,
		Y = 1,
		Z = 2,

		STATICFRAME = 0,
		ROTATEFRAME = 1,

		NOREPEAT = 0,
		REPEAT = 1,

		EVEN = 0,
		ODD = 1,
	};


	enum ORDER
	{
		XYZs = GETEULERORDER(X, EVEN, NOREPEAT, STATICFRAME),
		XYXs = GETEULERORDER(X, EVEN, REPEAT, STATICFRAME),
		XZYs = GETEULERORDER(X, ODD, NOREPEAT, STATICFRAME),
		XZXs = GETEULERORDER(X, ODD, REPEAT, STATICFRAME),
		YZXs = GETEULERORDER(Y, EVEN, NOREPEAT, STATICFRAME),
		YZYs = GETEULERORDER(Y, EVEN, REPEAT, STATICFRAME),
		YXZs = GETEULERORDER(Y, ODD, NOREPEAT, STATICFRAME),
		YXYs = GETEULERORDER(Y, ODD, REPEAT, STATICFRAME),
		ZXYs = GETEULERORDER(Z, EVEN, NOREPEAT, STATICFRAME),
		ZXZs = GETEULERORDER(Z, EVEN, REPEAT, STATICFRAME),
		ZYXs = GETEULERORDER(Z, ODD, NOREPEAT, STATICFRAME),
		ZYZs = GETEULERORDER(Z, ODD, REPEAT, STATICFRAME),

		ZYXr = GETEULERORDER(X, EVEN, NOREPEAT, ROTATEFRAME),
		XYXr = GETEULERORDER(X, EVEN, REPEAT, ROTATEFRAME),
		YZXr = GETEULERORDER(X, ODD, NOREPEAT, ROTATEFRAME),
		XZXr = GETEULERORDER(X, ODD, REPEAT, ROTATEFRAME),
		XZYr = GETEULERORDER(Y, EVEN, NOREPEAT, ROTATEFRAME),
		YZYr = GETEULERORDER(Y, EVEN, REPEAT, ROTATEFRAME),
		ZXYr = GETEULERORDER(Y, ODD, NOREPEAT, ROTATEFRAME),
		YXYr = GETEULERORDER(Y, ODD, REPEAT, ROTATEFRAME),
		YXZr = GETEULERORDER(Z, EVEN, NOREPEAT, ROTATEFRAME),
		ZXZr = GETEULERORDER(Z, EVEN, REPEAT, ROTATEFRAME),
		XYZr = GETEULERORDER(Z, ODD, NOREPEAT, ROTATEFRAME),
		ZYZr = GETEULERORDER(Z, ODD, REPEAT, ROTATEFRAME)
	};

	float m_fYaw, m_fPitch, m_fRoll;

	/*! Angles for each axis */
	Vector3                                      m_kAngle;

	/*! Order */
	unsigned int                                  m_uiOrder;
public:
	EulerAngle(void) : m_fYaw(0.0f), m_fPitch(0.0f), m_fRoll(0.0f) {}



	/**
	* \param afRotation                           Rotation around each axis
	* \param uiOrder                              Order rotations are applied
	*/
	EulerAngle(const Vector3 &rkAngle, unsigned int uiOrder = XYZs) : m_kAngle(rkAngle), m_uiOrder(uiOrder) {}

	/**
	* \param fRot0                                Rotation around first axis
	* \param fRot1                                Rotation around second axis
	* \param fRot2                                Rotation around third axis
	* \param uiOrder                              Order rotations are applied
	*/
	EulerAngle(float fRot0, float fRot1, float fRot2, unsigned int uiOrder = XYZs) : m_kAngle(fRot0, fRot1, fRot2), m_uiOrder(uiOrder) {}

	~EulerAngle(void) {}
};

class AxisAngle
{
public:

	/*! Axis of rotation */
	Vector3                                      m_kAxis;

	/*! Angle */
	float                                         m_fAngle;

	/**
	* \param rkAxis                               Axis of rotation
	* \param fAngle                               Angle
	*/
	AxisAngle(const Vector3 &rkAxis, float fAngle) : m_kAxis(rkAxis), m_fAngle(fAngle) {}
};

class Quaternion
{
public:
	float x, y, z, w;
public:
	Quaternion(void) : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
	Quaternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	//		inline                                        Quaternion(float fX, float fY, float fZ, float fW);
	Quaternion(const Matrix4 &rkMatrix);
	Quaternion(const AxisAngle &rkAxisAngle);
	Quaternion(const EulerAngle &rkAngles);
	Quaternion                             &Reset();
	Quaternion                             &Set(float fX, float fY, float fZ, float fW);
	Quaternion                             &Normalize();
	Matrix4                              &ToMatrix(Matrix4 *pkMatrix) const;
	AxisAngle                              ToAxisAngle() const;
	EulerAngle                                    ToEulerAngles(unsigned int uiOrder = EulerAngle::XYZs) const;
	Quaternion                             &Inverse();
	Quaternion                             &Slerp(float fT, const Quaternion &rkDest, bool bActuteAngle = false);
	Quaternion                             &operator =(const Matrix4 &rkMatrix);
	Quaternion                             &operator =(const AxisAngle &rkAxisAngle);
	Quaternion                             &operator =(const EulerAngle &rkEulerAngles);
	Quaternion                             operator *(const Quaternion &rkQuat) const;
	Quaternion                             operator *(float fScalar) const;
	Quaternion                             &operator *=(const Quaternion &rkQuat);
	Quaternion                             &operator *=(float fScalar);
	Quaternion                             operator ~() const;

	Vector3                                operator *(const Vector3 &rkVector) const;

	Quaternion                             operator +(const Quaternion &rkQuat) const;

	bool                                   operator ==(const Quaternion &rkQuat) const;

	bool                                   operator !=(const Quaternion &rkQuat) const;

	bool                                   isZero() const;

	static const Quaternion             IDENTITY;
	static const Quaternion             ZERO;
	//	~Quaternion(void) {}
	//void SetEulerAngle(const Vector3& ea);
	//Vector3 GetEulerAngle() const;

	void SetEulerAngle(const EulerAngle& ea);
	EulerAngle GetEulerAngle() const;
};



#define CLAMP(x , min , max) ((x) > (max) ? (max) : ((x) < (min) ? (min) : x))


class Matrix2
{
public:
	Matrix2();
	Matrix2(const float src[4]);
	Matrix2(float m0, float m1, float m2, float m3);

	void        set(const float src[4]);
	void        set(float m0, float m1, float m2, float m3);
	void        setRow(int index, const float row[2]);
	void        setRow(int index, const Vector2& v);
	void        setColumn(int index, const float col[2]);
	void        setColumn(int index, const Vector2& v);

	const float* get() const;
	float       getDeterminant();

	Matrix2&    identity();
	Matrix2&    transpose();
	Matrix2&    invert();

	Matrix2     operator+(const Matrix2& rhs) const;
	Matrix2     operator-(const Matrix2& rhs) const;
	Matrix2&    operator+=(const Matrix2& rhs);
	Matrix2&    operator-=(const Matrix2& rhs);
	Vector2     operator*(const Vector2& rhs) const;
	Matrix2     operator*(const Matrix2& rhs) const;
	Matrix2&    operator*=(const Matrix2& rhs);
	bool        operator==(const Matrix2& rhs) const;
	bool        operator!=(const Matrix2& rhs) const;
	float       operator[](int index) const;
	float&      operator[](int index);

	friend Matrix2 operator-(const Matrix2& m);
	friend Matrix2 operator*(float scalar, const Matrix2& m);
	friend Vector2 operator*(const Vector2& vec, const Matrix2& m);
	friend std::ostream& operator<<(std::ostream& os, const Matrix2& m);

private:
	float m[4];
};

//3X3¾ØÕó
class Matrix3
{
public:
	Matrix3();
	Matrix3(const float src[9]);
	Matrix3(float m0, float m1, float m2,
		float m3, float m4, float m5,
		float m6, float m7, float m8);

	void        set(const float src[9]);
	void        set(float m0, float m1, float m2,
		float m3, float m4, float m5,
		float m6, float m7, float m8);
	void        setRow(int index, const float row[3]);
	void        setRow(int index, const Vector3& v);
	void        setColumn(int index, const float col[3]);
	void        setColumn(int index, const Vector3& v);

	const float* get() const;
	float       getDeterminant();

	Matrix3&    identity();
	Matrix3&    transpose();
	Matrix3&    invert();

	Matrix3     operator+(const Matrix3& rhs) const;
	Matrix3     operator-(const Matrix3& rhs) const;
	Matrix3&    operator+=(const Matrix3& rhs);
	Matrix3&    operator-=(const Matrix3& rhs);
	Vector3     operator*(const Vector3& rhs) const;
	Matrix3     operator*(const Matrix3& rhs) const;
	Matrix3&    operator*=(const Matrix3& rhs);
	bool        operator==(const Matrix3& rhs) const;
	bool        operator!=(const Matrix3& rhs) const;
	float       operator[](int index) const;
	float&      operator[](int index);

	friend Matrix3 operator-(const Matrix3& m);
	friend Matrix3 operator*(float scalar, const Matrix3& m);
	friend Vector3 operator*(const Vector3& vec, const Matrix3& m);
	friend std::ostream& operator<<(std::ostream& os, const Matrix3& m);

public:
	float m[9];
};

//4X4¾ØÕó
class Matrix4
{
public:
	Matrix4();
	Matrix4(const float src[16]);
	Matrix4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	Matrix4(const Matrix4 &rkMatrix);

	Matrix4(const Quaternion &rkQuat);

	Matrix4(const Quaternion &rkQuat, const Vector3 &rkTranslation);

	Matrix4(const Vector3 &rkVector);

	void       Set(const float src[16]);
	void       Set(const Matrix4& mat);
	void       Set(float m00, float m01, float m02, float m03,
		float m04, float m05, float m06, float m07,
		float m08, float m09, float m10, float m11,
		float m12, float m13, float m14, float m15);

	void       Set(const Quaternion &rkQuat);
	void       Set(const Quaternion &rkQuat, const Vector3 &rkTranslation);

	Matrix4 &Matrix4::Set(const Vector3 &rkVector);


	void       SetRow(int index, const float row[4]);
	void       SetRow(int index, const Vector4 &v);
	void       SetRow(int index, const Vector3 &v);
	void       SetColumn(int index, const float col[4]);
	void       SetColumn(int index, const Vector4 &v);
	void       SetColumn(int index, const Vector3 &v);

	Vector3    Matrix4::GetColumn(int iColumn) const;

	const float* get()const;
	const float* getTranspose();
	float        getDeterminant();

	Matrix4&   identity();
	Matrix4&   transpose();
	Matrix4&   invert();
	Matrix4&   invertEuclidean();
	Matrix4&   invertAffine();
	Matrix4&   invertProjective();
	Matrix4&   invertGeneral();
	Matrix4	inverse();

	//Î»ÒÆ¾ØÕó
	Matrix4&   translate(float x, float y, float z);
	Matrix4&   translate(const Vector3& v);

	Matrix4&   rotate(float angle, const Vector3& axis);
	Matrix4&   rotate(float angle, float x, float y, float z);
	Matrix4&   rotateX(float angle);
	Matrix4&   rotateY(float angle);
	Matrix4&   rotateZ(float angle);

	Matrix4&   scale(float scale);
	Matrix4&   scale(float sx, float sy, float sz);

	//²Ù×÷·û
	Matrix4    operator+(const Matrix4& rhs) const;
	Matrix4    operator-(const Matrix4& rhs) const;
	Matrix4&   operator+=(const Matrix4& rhs);
	Matrix4&   operator-=(const Matrix4& rhs);
	Vector4      operator*(const Vector4& rhs) const;
	Vector3      operator*(const Vector3& rhs) const;
	Matrix4    operator*(const Matrix4& rhs) const;
	Matrix4&   operator*=(const Matrix4& rhs);

	bool         operator==(const Matrix4& rhs) const;
	bool         operator!=(const Matrix4& rhs) const;

	float        operator[](int index) const;
	float&       operator[](int index);

	friend Matrix4  operator-(const Matrix4& m);
	friend Matrix4  operator*(float scalar, const Matrix4& m);
	friend Vector3    operator*(const Vector3& vec, const Matrix4& m);
	friend Vector4    operator*(const Vector4& vec, const Matrix4& m);
	friend std::ostream& operator<<(std::ostream& os, const Matrix4& m);

	static const Matrix4                 IDENTITY;

	static void  createTranslation(const Vector3& translation, Matrix4* dst);
	static void  createTranslation(float xTranslation, float yTranslation, float zTranslation, Matrix4* dst);

	static void  multiply(const Matrix4& m1, const Matrix4& m2, Matrix4* dst);

private:
	float getCofactor(float m0, float m1, float m2,
		float m3, float m4, float m5,
		float m6, float m7, float m8);

	float tm[16];
public:
	float m[16];
};



/////////////////////////////////////////////////////////////////////////////
// inline functions for Matrix2
///////////////////////////////////////////////////////////////////////////
inline Matrix2::Matrix2()
{
	// initially identity matrix
	identity();
}

inline Matrix2::Matrix2(const float src[4])
{
	set(src);
}

inline Matrix2::Matrix2(float m0, float m1, float m2, float m3)
{
	set(m0, m1, m2, m3);
}

inline void Matrix2::set(const float src[4])
{
	m[0] = src[0];  m[1] = src[1];  m[2] = src[2];  m[3] = src[3];
}

inline void Matrix2::set(float m0, float m1, float m2, float m3)
{
	m[0] = m0;  m[1] = m1;  m[2] = m2;  m[3] = m3;
}

inline void Matrix2::setRow(int index, const float row[2])
{
	m[index] = row[0];  m[index + 2] = row[1];
}

inline void Matrix2::setRow(int index, const Vector2& v)
{
	m[index] = v.x;  m[index + 2] = v.y;
}

inline void Matrix2::setColumn(int index, const float col[2])
{
	m[index * 2] = col[0];  m[index * 2 + 1] = col[1];
}

inline void Matrix2::setColumn(int index, const Vector2& v)
{
	m[index * 2] = v.x;  m[index * 2 + 1] = v.y;
}

inline const float* Matrix2::get() const
{
	return m;
}

inline Matrix2& Matrix2::identity()
{
	m[0] = m[3] = 1.0f;
	m[1] = m[2] = 0.0f;
	return *this;
}

inline Matrix2 Matrix2::operator+(const Matrix2& rhs) const
{
	return Matrix2(m[0] + rhs[0], m[1] + rhs[1], m[2] + rhs[2], m[3] + rhs[3]);
}

inline Matrix2 Matrix2::operator-(const Matrix2& rhs) const
{
	return Matrix2(m[0] - rhs[0], m[1] - rhs[1], m[2] - rhs[2], m[3] - rhs[3]);
}

inline Matrix2& Matrix2::operator+=(const Matrix2& rhs)
{
	m[0] += rhs[0];  m[1] += rhs[1];  m[2] += rhs[2];  m[3] += rhs[3];
	return *this;
}

inline Matrix2& Matrix2::operator-=(const Matrix2& rhs)
{
	m[0] -= rhs[0];  m[1] -= rhs[1];  m[2] -= rhs[2];  m[3] -= rhs[3];
	return *this;
}

inline Vector2 Matrix2::operator*(const Vector2& rhs) const
{
	return Vector2(m[0] * rhs.x + m[2] * rhs.y, m[1] * rhs.x + m[3] * rhs.y);
}

inline Matrix2 Matrix2::operator*(const Matrix2& rhs) const
{
	return Matrix2(m[0] * rhs[0] + m[2] * rhs[1], m[1] * rhs[0] + m[3] * rhs[1],
		m[0] * rhs[2] + m[2] * rhs[3], m[1] * rhs[2] + m[3] * rhs[3]);
}

inline Matrix2& Matrix2::operator*=(const Matrix2& rhs)
{
	*this = *this * rhs;
	return *this;
}

inline bool Matrix2::operator==(const Matrix2& rhs) const
{
	return (m[0] == rhs[0]) && (m[1] == rhs[1]) && (m[2] == rhs[2]) && (m[3] == rhs[3]);
}

inline bool Matrix2::operator!=(const Matrix2& rhs) const
{
	return (m[0] != rhs[0]) || (m[1] != rhs[1]) || (m[2] != rhs[2]) || (m[3] != rhs[3]);
}

inline float Matrix2::operator[](int index) const
{
	return m[index];
}

inline float& Matrix2::operator[](int index)
{
	return m[index];
}

inline Matrix2 operator-(const Matrix2& rhs)
{
	return Matrix2(-rhs[0], -rhs[1], -rhs[2], -rhs[3]);
}

inline Matrix2 operator*(float s, const Matrix2& rhs)
{
	return Matrix2(s*rhs[0], s*rhs[1], s*rhs[2], s*rhs[3]);
}

inline Vector2 operator*(const Vector2& v, const Matrix2& rhs)
{
	return Vector2(v.x*rhs[0] + v.y*rhs[1], v.x*rhs[2] + v.y*rhs[3]);
}

inline std::ostream& operator<<(std::ostream& os, const Matrix2& m)
{
	os << std::fixed << std::setprecision(5);
	os << "[" << std::setw(10) << m[0] << " " << std::setw(10) << m[2] << "]\n"
		<< "[" << std::setw(10) << m[1] << " " << std::setw(10) << m[3] << "]\n";
	os << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
	return os;
}
// END OF MATRIX2 INLINE //////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
// inline functions for Matrix3
///////////////////////////////////////////////////////////////////////////
inline Matrix3::Matrix3()
{
	// initially identity matrix
	identity();
}

inline Matrix3::Matrix3(const float src[9])
{
	set(src);
}

inline Matrix3::Matrix3(float m0, float m1, float m2,
	float m3, float m4, float m5,
	float m6, float m7, float m8)
{
	set(m0, m1, m2, m3, m4, m5, m6, m7, m8);
}

inline void Matrix3::set(const float src[9])
{
	m[0] = src[0];  m[1] = src[1];  m[2] = src[2];
	m[3] = src[3];  m[4] = src[4];  m[5] = src[5];
	m[6] = src[6];  m[7] = src[7];  m[8] = src[8];
}

inline void Matrix3::set(float m0, float m1, float m2,
	float m3, float m4, float m5,
	float m6, float m7, float m8)
{
	m[0] = m0;  m[1] = m1;  m[2] = m2;
	m[3] = m3;  m[4] = m4;  m[5] = m5;
	m[6] = m6;  m[7] = m7;  m[8] = m8;
}

inline void Matrix3::setRow(int index, const float row[3])
{
	m[index] = row[0];  m[index + 3] = row[1];  m[index + 6] = row[2];
}

inline void Matrix3::setRow(int index, const Vector3& v)
{
	m[index] = v.x;  m[index + 3] = v.y;  m[index + 6] = v.z;
}

inline void Matrix3::setColumn(int index, const float col[3])
{
	m[index * 3] = col[0];  m[index * 3 + 1] = col[1];  m[index * 3 + 2] = col[2];
}

inline void Matrix3::setColumn(int index, const Vector3& v)
{
	m[index * 3] = v.x;  m[index * 3 + 1] = v.y;  m[index * 3 + 2] = v.z;
}

inline const float* Matrix3::get() const
{
	return m;
}

inline Matrix3& Matrix3::identity()
{
	m[0] = m[4] = m[8] = 1.0f;
	m[1] = m[2] = m[3] = m[5] = m[6] = m[7] = 0.0f;
	return *this;
}

inline Matrix3 Matrix3::operator+(const Matrix3& rhs) const
{
	return Matrix3(m[0] + rhs[0], m[1] + rhs[1], m[2] + rhs[2],
		m[3] + rhs[3], m[4] + rhs[4], m[5] + rhs[5],
		m[6] + rhs[6], m[7] + rhs[7], m[8] + rhs[8]);
}

inline Matrix3 Matrix3::operator-(const Matrix3& rhs) const
{
	return Matrix3(m[0] - rhs[0], m[1] - rhs[1], m[2] - rhs[2],
		m[3] - rhs[3], m[4] - rhs[4], m[5] - rhs[5],
		m[6] - rhs[6], m[7] - rhs[7], m[8] - rhs[8]);
}

inline Matrix3& Matrix3::operator+=(const Matrix3& rhs)
{
	m[0] += rhs[0];  m[1] += rhs[1];  m[2] += rhs[2];
	m[3] += rhs[3];  m[4] += rhs[4];  m[5] += rhs[5];
	m[6] += rhs[6];  m[7] += rhs[7];  m[8] += rhs[8];
	return *this;
}

inline Matrix3& Matrix3::operator-=(const Matrix3& rhs)
{
	m[0] -= rhs[0];  m[1] -= rhs[1];  m[2] -= rhs[2];
	m[3] -= rhs[3];  m[4] -= rhs[4];  m[5] -= rhs[5];
	m[6] -= rhs[6];  m[7] -= rhs[7];  m[8] -= rhs[8];
	return *this;
}

inline Vector3 Matrix3::operator*(const Vector3& rhs) const
{
	return Vector3(m[0] * rhs.x + m[3] * rhs.y + m[6] * rhs.z,
		m[1] * rhs.x + m[4] * rhs.y + m[7] * rhs.z,
		m[2] * rhs.x + m[5] * rhs.y + m[8] * rhs.z);
}

inline Matrix3 Matrix3::operator*(const Matrix3& rhs) const
{
	return Matrix3(m[0] * rhs[0] + m[3] * rhs[1] + m[6] * rhs[2], m[1] * rhs[0] + m[4] * rhs[1] + m[7] * rhs[2], m[2] * rhs[0] + m[5] * rhs[1] + m[8] * rhs[2],
		m[0] * rhs[3] + m[3] * rhs[4] + m[6] * rhs[5], m[1] * rhs[3] + m[4] * rhs[4] + m[7] * rhs[5], m[2] * rhs[3] + m[5] * rhs[4] + m[8] * rhs[5],
		m[0] * rhs[6] + m[3] * rhs[7] + m[6] * rhs[8], m[1] * rhs[6] + m[4] * rhs[7] + m[7] * rhs[8], m[2] * rhs[6] + m[5] * rhs[7] + m[8] * rhs[8]);
}

inline Matrix3& Matrix3::operator*=(const Matrix3& rhs)
{
	*this = *this * rhs;
	return *this;
}

inline bool Matrix3::operator==(const Matrix3& rhs) const
{
	return (m[0] == rhs[0]) && (m[1] == rhs[1]) && (m[2] == rhs[2]) &&
		(m[3] == rhs[3]) && (m[4] == rhs[4]) && (m[5] == rhs[5]) &&
		(m[6] == rhs[6]) && (m[7] == rhs[7]) && (m[8] == rhs[8]);
}

inline bool Matrix3::operator!=(const Matrix3& rhs) const
{
	return (m[0] != rhs[0]) || (m[1] != rhs[1]) || (m[2] != rhs[2]) ||
		(m[3] != rhs[3]) || (m[4] != rhs[4]) || (m[5] != rhs[5]) ||
		(m[6] != rhs[6]) || (m[7] != rhs[7]) || (m[8] != rhs[8]);
}

inline float Matrix3::operator[](int index) const
{
	return m[index];
}

inline float& Matrix3::operator[](int index)
{
	return m[index];
}

inline Matrix3 operator-(const Matrix3& rhs)
{
	return Matrix3(-rhs[0], -rhs[1], -rhs[2], -rhs[3], -rhs[4], -rhs[5], -rhs[6], -rhs[7], -rhs[8]);
}

inline Matrix3 operator*(float s, const Matrix3& rhs)
{
	return Matrix3(s*rhs[0], s*rhs[1], s*rhs[2], s*rhs[3], s*rhs[4], s*rhs[5], s*rhs[6], s*rhs[7], s*rhs[8]);
}

inline Vector3 operator*(const Vector3& v, const Matrix3& m)
{
	return Vector3(v.x*m[0] + v.y*m[1] + v.z*m[2], v.x*m[3] + v.y*m[4] + v.z*m[5], v.x*m[6] + v.y*m[7] + v.z*m[8]);
}

inline std::ostream& operator<<(std::ostream& os, const Matrix3& m)
{
	os << std::fixed << std::setprecision(5);
	os << "[" << std::setw(10) << m[0] << " " << std::setw(10) << m[3] << " " << std::setw(10) << m[6] << "]\n"
		<< "[" << std::setw(10) << m[1] << " " << std::setw(10) << m[4] << " " << std::setw(10) << m[7] << "]\n"
		<< "[" << std::setw(10) << m[2] << " " << std::setw(10) << m[5] << " " << std::setw(10) << m[8] << "]\n";
	os << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
	return os;
}
// END OF MATRIX3 INLINE //////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// inline functions for Matrix4
///////////////////////////////////////////////////////////////////////////
inline Matrix4::Matrix4()
{
	// initially identity matrix
	identity();
}

inline Matrix4::Matrix4(const Matrix4 &rkMatrix) {
	_fmemcpy(m, rkMatrix.m, 64);
}

inline Matrix4::Matrix4(const float src[16])
{
	Set(src);
}

inline Matrix4::Matrix4(float m00, float m01, float m02, float m03,
	float m04, float m05, float m06, float m07,
	float m08, float m09, float m10, float m11,
	float m12, float m13, float m14, float m15)
{
	Set(m00, m01, m02, m03, m04, m05, m06, m07, m08, m09, m10, m11, m12, m13, m14, m15);
}

inline Matrix4::Matrix4(const Quaternion &rkQuat)
{
	/*rkQuat.ToMatrix(this);
	m[0][3] = m[1][3] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0.0f;
	m[3][3] = 1.0f; */
	rkQuat.ToMatrix(this);
	m[3] = m[7] = m[11] = m[12] = m[13] = m[14] = 0.0f;
	m[15] = 1.0f;
}

inline Matrix4::Matrix4(const Quaternion &rkQuat, const Vector3 &rkTranslation)
{
	//rkQuat.ToMatrix(this); 
	//m[0][3] = rkTranslation.x;
	//m[1][3] = rkTranslation.y;
	//m[2][3] = rkTranslation.z; 
	//m[3][0] = m[3][1] = m[3][2] = 0.0f;
	//m[3][3] = 1.0f;


	rkQuat.ToMatrix(this);
	m[3] = rkTranslation.x;
	m[7] = rkTranslation.y;
	m[11] = rkTranslation.z;
	m[12] = m[13] = m[14] = 0.0f;
	m[15] = 1.0f;
}

inline Matrix4::Matrix4(const Vector3 &rkVector) { Set(rkVector); }

inline Matrix4 &Matrix4::Set(const Vector3 &rkVector)
{
	float *pfDst = &m[0];

	*pfDst++ = 0.0f;
	*pfDst++ = -rkVector.z;
	*pfDst++ = rkVector.y;
	*pfDst++ = 0.0f;

	*pfDst++ = rkVector.z;
	*pfDst++ = 0.0f;
	*pfDst++ = -rkVector.x;
	*pfDst++ = 0.0f;

	*pfDst++ = -rkVector.y;
	*pfDst++ = rkVector.x;
	*pfDst++ = 0.0f;
	*pfDst++ = 0.0f;

	*pfDst++ = 0.0f;
	*pfDst++ = 0.0f;
	*pfDst++ = 0.0f;
	*pfDst = 1.0f;

	return(*this);
}

inline void Matrix4::Set(const float src[16])
{
	m[0] = src[0];  m[1] = src[1];  m[2] = src[2];  m[3] = src[3];
	m[4] = src[4];  m[5] = src[5];  m[6] = src[6];  m[7] = src[7];
	m[8] = src[8];  m[9] = src[9];  m[10] = src[10]; m[11] = src[11];
	m[12] = src[12]; m[13] = src[13]; m[14] = src[14]; m[15] = src[15];
}

inline void Matrix4::Set(const Matrix4& mat)
{
	memcpy(this->m, mat.m, sizeof(float) * 16);
}

inline void Matrix4::Set(float m00, float m01, float m02, float m03,
	float m04, float m05, float m06, float m07,
	float m08, float m09, float m10, float m11,
	float m12, float m13, float m14, float m15)
{
	m[0] = m00;  m[1] = m01;  m[2] = m02;  m[3] = m03;
	m[4] = m04;  m[5] = m05;  m[6] = m06;  m[7] = m07;
	m[8] = m08;  m[9] = m09;  m[10] = m10;  m[11] = m11;
	m[12] = m12;  m[13] = m13;  m[14] = m14;  m[15] = m15;
}

inline void Matrix4::Set(const Quaternion &rkQuat)
{
	rkQuat.ToMatrix(this);
	m[3] = m[7] = m[11] = m[12] = m[13] = m[14] = 0.0f;
	m[15] = 1.0f;
}


inline void Matrix4::Set(const Quaternion &rkQuat, const Vector3 &rkTranslation)
{
	rkQuat.ToMatrix(this);
	m[3] = rkTranslation.x;
	m[7] = rkTranslation.y;
	m[11] = rkTranslation.z;
	m[12] = m[13] = m[14] = 0.0f;
	m[15] = 1.0f;
}

inline void Matrix4::SetRow(int index, const float row[4])
{
	m[index] = row[0];  m[index + 4] = row[1];  m[index + 8] = row[2];  m[index + 12] = row[3];
}



inline void Matrix4::SetRow(int index, const Vector4& v)
{
	m[index] = v.x;  m[index + 4] = v.y;  m[index + 8] = v.z;  m[index + 12] = v.w;
}



inline void Matrix4::SetRow(int index, const Vector3& v)
{
	m[index] = v.x;  m[index + 4] = v.y;  m[index + 8] = v.z;
}



inline void Matrix4::SetColumn(int index, const float col[4])
{
	m[index * 4] = col[0];  m[index * 4 + 1] = col[1];  m[index * 4 + 2] = col[2];  m[index * 4 + 3] = col[3];
}



inline void Matrix4::SetColumn(int index, const Vector4& v)
{
	m[index * 4] = v.x;  m[index * 4 + 1] = v.y;  m[index * 4 + 2] = v.z;  m[index * 4 + 3] = v.w;
}



inline void Matrix4::SetColumn(int index, const Vector3& v)
{
	m[index * 4] = v.x;  m[index * 4 + 1] = v.y;  m[index * 4 + 2] = v.z;
}


inline Vector3 Matrix4::GetColumn(int iColumn) const
{
	return Vector3(m[0 + iColumn * 4], m[1 + iColumn * 4], m[2 + iColumn * 4]);
}


inline const float* Matrix4::get() const
{
	return m;
}



inline const float* Matrix4::getTranspose()
{
	tm[0] = m[0];   tm[1] = m[4];   tm[2] = m[8];   tm[3] = m[12];
	tm[4] = m[1];   tm[5] = m[5];   tm[6] = m[9];   tm[7] = m[13];
	tm[8] = m[2];   tm[9] = m[6];   tm[10] = m[10];  tm[11] = m[14];
	tm[12] = m[3];   tm[13] = m[7];   tm[14] = m[11];  tm[15] = m[15];
	return tm;
}



inline Matrix4& Matrix4::identity()
{
	m[0] = m[5] = m[10] = m[15] = 1.0f;
	m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] = m[13] = m[14] = 0.0f;
	return *this;
}



inline Matrix4 Matrix4::operator+(const Matrix4& rhs) const
{
	return Matrix4(m[0] + rhs[0], m[1] + rhs[1], m[2] + rhs[2], m[3] + rhs[3],
		m[4] + rhs[4], m[5] + rhs[5], m[6] + rhs[6], m[7] + rhs[7],
		m[8] + rhs[8], m[9] + rhs[9], m[10] + rhs[10], m[11] + rhs[11],
		m[12] + rhs[12], m[13] + rhs[13], m[14] + rhs[14], m[15] + rhs[15]);
}



inline Matrix4 Matrix4::operator-(const Matrix4& rhs) const
{
	return Matrix4(m[0] - rhs[0], m[1] - rhs[1], m[2] - rhs[2], m[3] - rhs[3],
		m[4] - rhs[4], m[5] - rhs[5], m[6] - rhs[6], m[7] - rhs[7],
		m[8] - rhs[8], m[9] - rhs[9], m[10] - rhs[10], m[11] - rhs[11],
		m[12] - rhs[12], m[13] - rhs[13], m[14] - rhs[14], m[15] - rhs[15]);
}



inline Matrix4& Matrix4::operator+=(const Matrix4& rhs)
{
	m[0] += rhs[0];   m[1] += rhs[1];   m[2] += rhs[2];   m[3] += rhs[3];
	m[4] += rhs[4];   m[5] += rhs[5];   m[6] += rhs[6];   m[7] += rhs[7];
	m[8] += rhs[8];   m[9] += rhs[9];   m[10] += rhs[10];  m[11] += rhs[11];
	m[12] += rhs[12];  m[13] += rhs[13];  m[14] += rhs[14];  m[15] += rhs[15];
	return *this;
}



inline Matrix4& Matrix4::operator-=(const Matrix4& rhs)
{
	m[0] -= rhs[0];   m[1] -= rhs[1];   m[2] -= rhs[2];   m[3] -= rhs[3];
	m[4] -= rhs[4];   m[5] -= rhs[5];   m[6] -= rhs[6];   m[7] -= rhs[7];
	m[8] -= rhs[8];   m[9] -= rhs[9];   m[10] -= rhs[10];  m[11] -= rhs[11];
	m[12] -= rhs[12];  m[13] -= rhs[13];  m[14] -= rhs[14];  m[15] -= rhs[15];
	return *this;
}



inline Vector4 Matrix4::operator*(const Vector4& rhs) const
{
	return Vector4(m[0] * rhs.x + m[4] * rhs.y + m[8] * rhs.z + m[12] * rhs.w,
		m[1] * rhs.x + m[5] * rhs.y + m[9] * rhs.z + m[13] * rhs.w,
		m[2] * rhs.x + m[6] * rhs.y + m[10] * rhs.z + m[14] * rhs.w,
		m[3] * rhs.x + m[7] * rhs.y + m[11] * rhs.z + m[15] * rhs.w);
}



inline Vector3 Matrix4::operator*(const Vector3& rhs) const
{
	return Vector3(m[0] * rhs.x + m[4] * rhs.y + m[8] * rhs.z,
		m[1] * rhs.x + m[5] * rhs.y + m[9] * rhs.z,
		m[2] * rhs.x + m[6] * rhs.y + m[10] * rhs.z);
}



inline Matrix4 Matrix4::operator*(const Matrix4& n) const
{
	return Matrix4(m[0] * n[0] + m[4] * n[1] + m[8] * n[2] + m[12] * n[3], m[1] * n[0] + m[5] * n[1] + m[9] * n[2] + m[13] * n[3], m[2] * n[0] + m[6] * n[1] + m[10] * n[2] + m[14] * n[3], m[3] * n[0] + m[7] * n[1] + m[11] * n[2] + m[15] * n[3],
		m[0] * n[4] + m[4] * n[5] + m[8] * n[6] + m[12] * n[7], m[1] * n[4] + m[5] * n[5] + m[9] * n[6] + m[13] * n[7], m[2] * n[4] + m[6] * n[5] + m[10] * n[6] + m[14] * n[7], m[3] * n[4] + m[7] * n[5] + m[11] * n[6] + m[15] * n[7],
		m[0] * n[8] + m[4] * n[9] + m[8] * n[10] + m[12] * n[11], m[1] * n[8] + m[5] * n[9] + m[9] * n[10] + m[13] * n[11], m[2] * n[8] + m[6] * n[9] + m[10] * n[10] + m[14] * n[11], m[3] * n[8] + m[7] * n[9] + m[11] * n[10] + m[15] * n[11],
		m[0] * n[12] + m[4] * n[13] + m[8] * n[14] + m[12] * n[15], m[1] * n[12] + m[5] * n[13] + m[9] * n[14] + m[13] * n[15], m[2] * n[12] + m[6] * n[13] + m[10] * n[14] + m[14] * n[15], m[3] * n[12] + m[7] * n[13] + m[11] * n[14] + m[15] * n[15]);
}



inline Matrix4& Matrix4::operator*=(const Matrix4& rhs)
{
	*this = *this * rhs;
	return *this;
}



inline bool Matrix4::operator==(const Matrix4& n) const
{
	return (m[0] == n[0]) && (m[1] == n[1]) && (m[2] == n[2]) && (m[3] == n[3]) &&
		(m[4] == n[4]) && (m[5] == n[5]) && (m[6] == n[6]) && (m[7] == n[7]) &&
		(m[8] == n[8]) && (m[9] == n[9]) && (m[10] == n[10]) && (m[11] == n[11]) &&
		(m[12] == n[12]) && (m[13] == n[13]) && (m[14] == n[14]) && (m[15] == n[15]);
}



inline bool Matrix4::operator!=(const Matrix4& n) const
{
	return (m[0] != n[0]) || (m[1] != n[1]) || (m[2] != n[2]) || (m[3] != n[3]) ||
		(m[4] != n[4]) || (m[5] != n[5]) || (m[6] != n[6]) || (m[7] != n[7]) ||
		(m[8] != n[8]) || (m[9] != n[9]) || (m[10] != n[10]) || (m[11] != n[11]) ||
		(m[12] != n[12]) || (m[13] != n[13]) || (m[14] != n[14]) || (m[15] != n[15]);
}



inline float Matrix4::operator[](int index) const
{
	return m[index];
}



inline float& Matrix4::operator[](int index)
{
	return m[index];
}


//const float Matrix4::m[16] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
//const Matrix4 Matrix4::IDENTITY
//(
//(const float*)m
//);


inline Matrix4 operator-(const Matrix4& rhs)
{
	return Matrix4(-rhs[0], -rhs[1], -rhs[2], -rhs[3], -rhs[4], -rhs[5], -rhs[6], -rhs[7], -rhs[8], -rhs[9], -rhs[10], -rhs[11], -rhs[12], -rhs[13], -rhs[14], -rhs[15]);
}



inline Matrix4 operator*(float s, const Matrix4& rhs)
{
	return Matrix4(s*rhs[0], s*rhs[1], s*rhs[2], s*rhs[3], s*rhs[4], s*rhs[5], s*rhs[6], s*rhs[7], s*rhs[8], s*rhs[9], s*rhs[10], s*rhs[11], s*rhs[12], s*rhs[13], s*rhs[14], s*rhs[15]);
}



inline Vector4 operator*(const Vector4& v, const Matrix4& m)
{
	return Vector4(v.x*m[0] + v.y*m[1] + v.z*m[2] + v.w*m[3], v.x*m[4] + v.y*m[5] + v.z*m[6] + v.w*m[7], v.x*m[8] + v.y*m[9] + v.z*m[10] + v.w*m[11], v.x*m[12] + v.y*m[13] + v.z*m[14] + v.w*m[15]);
}



inline Vector3 operator*(const Vector3& v, const Matrix4& m)
{
	return Vector3(v.x*m[0] + v.y*m[1] + v.z*m[2], v.x*m[4] + v.y*m[5] + v.z*m[6], v.x*m[8] + v.y*m[9] + v.z*m[10]);
}



inline std::ostream& operator<<(std::ostream& os, const Matrix4& m)
{
	os << std::fixed << std::setprecision(5);
	os << "[" << std::setw(10) << m[0] << " " << std::setw(10) << m[4] << " " << std::setw(10) << m[8] << " " << std::setw(10) << m[12] << "]\n"
		<< "[" << std::setw(10) << m[1] << " " << std::setw(10) << m[5] << " " << std::setw(10) << m[9] << " " << std::setw(10) << m[13] << "]\n"
		<< "[" << std::setw(10) << m[2] << " " << std::setw(10) << m[6] << " " << std::setw(10) << m[10] << " " << std::setw(10) << m[14] << "]\n"
		<< "[" << std::setw(10) << m[3] << " " << std::setw(10) << m[7] << " " << std::setw(10) << m[11] << " " << std::setw(10) << m[15] << "]\n";
	os << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
	return os;
}

//inline Matrix4 &Matrix4::operator =(const Quaternion &rkQuat)
//{
//	rkQuat.ToMatrix(this);
//
//	m_aafMatrix[0][3] = m_aafMatrix[1][3] = m_aafMatrix[2][3] = m_aafMatrix[3][0] = m_aafMatrix[3][1] = m_aafMatrix[3][2] = 0.0f;
//	m_aafMatrix[3][3] = 1.0f;
//
//	return(*this);
//}
// END OF MATRIX4 INLINE //////////////////////////////////////////////////////


inline void Quaternion::SetEulerAngle(const EulerAngle &ea)
{
	float fCosHRoll = cos(ea.m_fRoll * .5f);
	float fSinHRoll = sin(ea.m_fRoll * .5f);
	float fCosHPitch = cos(ea.m_fPitch * .5f);
	float fSinHPitch = sin(ea.m_fPitch * .5f);
	float fCosHYaw = cos(ea.m_fYaw * .5f);
	float fSinHYaw = sin(ea.m_fYaw * .5f);

	/// Cartesian coordinate System
	//w = fCosHRoll * fCosHPitch * fCosHYaw + fSinHRoll * fSinHPitch * fSinHYaw;
	//x = fSinHRoll * fCosHPitch * fCosHYaw - fCosHRoll * fSinHPitch * fSinHYaw;
	//y = fCosHRoll * fSinHPitch * fCosHYaw + fSinHRoll * fCosHPitch * fSinHYaw;
	//z = fCosHRoll * fCosHPitch * fSinHYaw - fSinHRoll * fSinHPitch * fCosHYaw;

	w = fCosHRoll * fCosHPitch * fCosHYaw + fSinHRoll * fSinHPitch * fSinHYaw;
	x = fCosHRoll * fSinHPitch * fCosHYaw + fSinHRoll * fCosHPitch * fSinHYaw;
	y = fCosHRoll * fCosHPitch * fSinHYaw - fSinHRoll * fSinHPitch * fCosHYaw;
	z = fSinHRoll * fCosHPitch * fCosHYaw - fCosHRoll * fSinHPitch * fSinHYaw;
}

inline EulerAngle Quaternion::GetEulerAngle() const
{
	EulerAngle ea;

	/// Cartesian coordinate System 
	//ea.m_fRoll  = atan2(2 * (w * x + y * z) , 1 - 2 * (x * x + y * y));
	//ea.m_fPitch = asin(2 * (w * y - z * x));
	//ea.m_fYaw   = atan2(2 * (w * z + x * y) , 1 - 2 * (y * y + z * z));

	ea.m_fRoll = atan2(2 * (w * z + x * y), 1 - 2 * (z * z + x * x));
	ea.m_fPitch = asin(CLAMP(2 * (w * x - y * z), -1.0f, 1.0f));
	ea.m_fYaw = atan2(2 * (w * y + z * x), 1 - 2 * (x * x + y * y));

	return ea;
}

inline Quaternion &Quaternion::operator = (const EulerAngle &rkEulerAngles)
{
	float fAngle[3] = { rkEulerAngles.m_kAngle.x, rkEulerAngles.m_kAngle.y, rkEulerAngles.m_kAngle.z };
	float a[3], ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;
	int i, j, k, h, n, s, f;

	GetOrderData(rkEulerAngles.m_uiOrder, i, j, k, h, n, s, f);

	if (f == EulerAngle::ROTATEFRAME) { float t = fAngle[0]; fAngle[0] = fAngle[2]; fAngle[2] = t; }
	if (n == EulerAngle::ODD) { fAngle[1] = -fAngle[1]; }

	ti = fAngle[0] * 0.5f;
	tj = fAngle[1] * 0.5f;
	th = fAngle[2] * 0.5f;

	ci = cosf(ti);  cj = cosf(tj);  ch = cosf(th);
	si = sinf(ti);  sj = sinf(tj);  sh = sinf(th);

	cc = ci * ch; cs = ci * sh; sc = si * ch; ss = si * sh;

	if (s == EulerAngle::REPEAT)
	{
		a[i] = cj * (cs + sc);
		a[j] = sj * (cc + ss);
		a[k] = sj * (cs - sc);
		w = cj * (cc - ss);
	}
	else
	{
		a[i] = cj * sc - sj * cs;
		a[j] = cj * ss + sj * cc;
		a[k] = cj * cs - sj * sc;
		w = cj * cc + sj * ss;
	}

	if (n == EulerAngle::ODD) a[j] = -a[j];

	x = a[EulerAngle::X];
	y = a[EulerAngle::Y];
	z = a[EulerAngle::Z];

	return Normalize();
}


inline EulerAngle Quaternion::ToEulerAngles(unsigned int uiOrder) const
{
	//Matrix4 kM(*this);

	////From http://vered.rose.utoronto.ca/people/david_dir/GEMS/GEMS.html
	//int i, j, k, h, n, s, f;
	Vector3 kAngles;

	/*GetOrderData(uiOrder, i, j, k, h, n, s, f);

	if (s == EulerAngle::REPEAT)
	{
	float sy = sqrtf(kM[i][j] * kM[i][j] + kM[i][k] * kM[i][k]);

	if (sy > EPSILON)
	{
	kAngles.x = atan2f(kM[i][j], kM[i][k]);
	kAngles.y = atan2f(sy, kM[i][i]);
	kAngles.z = atan2f(kM[j][i], -kM[k][i]);
	}
	else
	{
	kAngles.x = atan2f(-kM[j][k], kM[j][j]);
	kAngles.y = atan2f(sy, kM[i][i]);
	kAngles.z = 0.0f;
	}
	}
	else
	{
	float cy = sqrtf(kM[i][i] * kM[i][i] + kM[j][i] * kM[j][i]);

	if (cy > EPSILON)
	{
	kAngles.x = atan2f(kM[k][j], kM[k][k]);
	kAngles.y = atan2f(-kM[k][i], cy);
	kAngles.z = atan2f(kM[j][i], kM[i][i]);
	}
	else
	{
	kAngles.x = atan2f(-kM[j][k], kM[j][j]);
	kAngles.y = atan2f(-kM[k][i], cy);
	kAngles.z = 0.0f;
	}
	}

	if (n == EulerAngle::ODD) { kAngles.x = -kAngles.x; kAngles.y = -kAngles.y; kAngles.z = -kAngles.z; }
	if (f == EulerAngle::ROTATEFRAME) { float t = kAngles.x; kAngles.x = kAngles.z; kAngles.z = t; }
	*/
	return EulerAngle(kAngles, uiOrder);
}


//	Quaternion::Quaternion(float fX, float fY, float fZ, float fW) : x(fX), y(fY), z(fZ), w(fW) {}

inline Quaternion::Quaternion(const Matrix4 &rkMatrix) { *this = rkMatrix; }

inline Quaternion::Quaternion(const AxisAngle &rkAxisAngle) { *this = rkAxisAngle; }

inline Quaternion::Quaternion(const EulerAngle &rkAngles) { *this = rkAngles; }


inline Quaternion &Quaternion::Reset()
{
	x = y = z = 0.0f;
	w = 1.0f;

	return(*this);
}


inline Quaternion &Quaternion::Set(float fX, float fY, float fZ, float fW)
{
	x = fX;
	y = fY;
	z = fZ;
	w = fW;

	return(*this);
}


inline Quaternion &Quaternion::Inverse()
{
	x = -x;
	y = -y;
	z = -z;

	return(*this);
}


inline Quaternion Quaternion::operator ~() const
{
	return Quaternion(-x, -y, -z, w);
}


inline Quaternion &Quaternion::Normalize()
{
	float fNorm = x * x + y * y + z * z + w * w;

	if (fNorm < SQREPSILON)
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 1.0f;
	}
	else if (fabs(fNorm - 1.0f) > SQREPSILON)
	{
#ifdef USE_FASTINVSQRT
		float fScale = FastInvSqrt(fNorm);
#else
		float fScale = 1.0f / sqrtf(fNorm);
#endif

		x *= fScale;
		y *= fScale;
		z *= fScale;
		w *= fScale;

		return(*this);
	}

	return(*this);
}


inline Matrix4 &Quaternion::ToMatrix(Matrix4 *pkMatrix) const
{
	float fTX = 2.0f * x;
	float fTY = 2.0f * y;
	float fTZ = 2.0f * z;
	float fTWX = fTX * w;
	float fTWY = fTY * w;
	float fTWZ = fTZ * w;
	float fTXX = fTX * x;
	float fTXY = fTY * x;
	float fTXZ = fTZ * x;
	float fTYY = fTY * y;
	float fTYZ = fTZ * y;
	float fTZZ = fTZ * z;
	float *pfMat = &pkMatrix->m[0];
	//		float *pfMat = &pkMatrix->m_aafMatrix[0][0];

	pfMat[0] = 1.0f - (fTYY + fTZZ);
	pfMat[1] = fTXY - fTWZ;
	pfMat[2] = fTXZ + fTWY;

	pfMat[4] = fTXY + fTWZ;
	pfMat[5] = 1.0f - (fTXX + fTZZ);
	pfMat[6] = fTYZ - fTWX;

	pfMat[8] = fTXZ - fTWY;
	pfMat[9] = fTYZ + fTWX;
	pfMat[10] = 1.0f - (fTXX + fTYY);

	return(*pkMatrix);
}


inline AxisAngle Quaternion::ToAxisAngle() const
{
	float fSqrLength = x * x + y * y + z * z;
	float fAngle;
	Vector3 kAxis;

	if (fSqrLength > 0.0)
	{
		fAngle = 2.0f * acosf(w);

		float fInvLength = 1.0f / logf(fSqrLength);

		kAxis.x = x * fInvLength;
		kAxis.y = y * fInvLength;
		kAxis.z = z * fInvLength;
	}
	else
	{
		fAngle = 0.0f;

		kAxis.x = 1.0f;
		kAxis.y = 0.0f;
		kAxis.z = 0.0f;
	}

	return AxisAngle(kAxis, fAngle);
}


inline Quaternion &Quaternion::Slerp(float fT, const Quaternion &rkDest, bool bAcuteAngle)
{
	assert((fT >= 0.0f) && (fT <= 1.0f));

	Quaternion kD = rkDest;

	float fCos = x * kD.x + y * kD.y + z * kD.z + w * kD.w;

	//if fCos < 0 use Slerp( fT, -rkDest ) to get acute angle
	//between quaternions and avoid extra spins
	if (bAcuteAngle && (fCos < 0.0f))
	{
		kD.x = -rkDest.x;
		kD.y = -rkDest.y;
		kD.z = -rkDest.z;
		kD.w = -rkDest.w;

		fCos = x * kD.x + y * kD.y + z * kD.z + w * kD.w;
	}

	float fAngle = 0.0f;

	if (-1.0f < fCos)
	{
		if (fCos < 1.0)
			fAngle = acosf(fCos);
		else
			fAngle = 0.0f;
	}
	else
		fAngle = PI;

	if (fabs(fAngle) < 0.001f)
		return(*this);

	float fSin = sinf(fAngle);
	float fInvSin = 1.0f / fSin;
	float fCoeffOne = sinf((1.0f - fT) * fAngle) * fInvSin;
	float fCoeffTwo = sinf(fT * fAngle) * fInvSin;

	x = x * fCoeffOne + kD.x * fCoeffTwo;
	y = y * fCoeffOne + kD.y * fCoeffTwo;
	z = z * fCoeffOne + kD.z * fCoeffTwo;
	w = w * fCoeffOne + kD.w * fCoeffTwo;

	return(*this);
}


inline Quaternion &Quaternion::operator =(const Matrix4 &rkMatrix)
{
	// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
	// article "Quaternion Calculus and Fast Animation".

	//float fTrace = rkMatrix[0][0] + rkMatrix[1][1] + rkMatrix[2][2];
	//float fRoot;

	//if (fTrace > 0.0)
	//{
	//	fRoot = sqrtf(fTrace + 1.0f);

	//	w = 0.5f * fRoot;

	//	fRoot = 0.5f / fRoot;

	//	x = (rkMatrix[2][1] - rkMatrix[1][2]) * fRoot;
	//	y = (rkMatrix[0][2] - rkMatrix[2][0]) * fRoot;
	//	z = (rkMatrix[1][0] - rkMatrix[0][1]) * fRoot;
	//}
	//else
	//{
	//	int iNext[3] = { 1, 2, 0 };

	//	int i = 0;
	//	if (rkMatrix[1][1] > rkMatrix[0][0])
	//		i = 1;

	//	if (rkMatrix[2][2] > rkMatrix[i][i])
	//		i = 2;

	//	int j = iNext[i];
	//	int k = iNext[j];

	//	fRoot = sqrtf(rkMatrix[i][i] - rkMatrix[j][j] - rkMatrix[k][k] + 1.0f);

	//	float *apfQuat[3] = { &x, &y, &z };

	//	*(apfQuat[i]) = 0.5f * fRoot;

	//	fRoot = 0.5f / fRoot;

	//	w = (rkMatrix[k][j] - rkMatrix[j][k]) * fRoot;

	//	*(apfQuat[j]) = (rkMatrix[j][i] + rkMatrix[i][j]) * fRoot;
	//	*(apfQuat[k]) = (rkMatrix[k][i] + rkMatrix[i][k]) * fRoot;
	//}

	return Normalize();
}


inline Quaternion &Quaternion::operator =(const AxisAngle &rkAxisAngle)
{
	float fHalfAngle = 0.5f * rkAxisAngle.m_fAngle;
	float fSin = sinf(fHalfAngle);

	x = fSin * rkAxisAngle.m_kAxis.x;
	y = fSin * rkAxisAngle.m_kAxis.y;
	z = fSin * rkAxisAngle.m_kAxis.z;
	w = cosf(fHalfAngle);

	return Normalize();
}


inline Vector3 Quaternion::operator *(const Vector3 &rkVector) const
{
	Vector3 kVec0(x, y, z);
	Vector3 kVec1(kVec0 % rkVector);

	kVec1 += rkVector * w;

	Vector3 kVec2 = kVec1 % kVec0;

	kVec0 *= rkVector * kVec0;

	kVec0 += kVec1 * w;

	return Vector3(kVec0 - kVec2);
}


inline Quaternion &Quaternion::operator *=(const Quaternion &rkQuat)
{
	Vector3 kVecOne(x, y, z);
	Vector3 kVecTwo(rkQuat.x, rkQuat.y, rkQuat.z);

	Vector3 kVecResult = (kVecOne % kVecTwo) + kVecTwo * w + kVecOne * rkQuat.w;

	return(Set(kVecResult.x, kVecResult.y, kVecResult.z, w * rkQuat.w - Dot(kVecOne, kVecTwo)));
}


inline Quaternion Quaternion::operator *(const Quaternion &rkQuat) const
{
	Vector3 kVecOne(x, y, z);
	Vector3 kVecTwo(rkQuat.x, rkQuat.y, rkQuat.z);

	Vector3 kVecResult = (kVecOne % kVecTwo) + kVecTwo * w + kVecOne * rkQuat.w;

	return(Quaternion(kVecResult.x, kVecResult.y, kVecResult.z, w * rkQuat.w - Dot(kVecOne, kVecTwo)));
}


inline Quaternion Quaternion::operator *(float fScalar) const
{
	return(Quaternion(x * fScalar, y * fScalar, z * fScalar, w * fScalar));
}


inline Quaternion &Quaternion::operator *=(float fScalar)
{
	x *= fScalar;
	y *= fScalar;
	z *= fScalar;
	w *= fScalar;

	return(*this);
}


inline Quaternion Quaternion::operator +(const Quaternion &rkQuat) const
{
	return(Quaternion(x + rkQuat.x, y + rkQuat.y, z + rkQuat.z, w + rkQuat.w));
}


inline bool Quaternion::operator ==(const Quaternion &rkQuat) const
{
	return((fabs(x - rkQuat.x) < EPSILON) && (fabs(y - rkQuat.y) < EPSILON) && (fabs(z - rkQuat.z) < EPSILON) && (fabs(w - rkQuat.w) < EPSILON));
}


inline bool Quaternion::operator !=(const Quaternion &rkQuat) const
{
	return((fabs(x - rkQuat.x) > EPSILON) || (fabs(y - rkQuat.y) > EPSILON) || (fabs(z - rkQuat.z) > EPSILON) || (fabs(w - rkQuat.w) > EPSILON));
}


#endif;