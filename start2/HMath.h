#ifndef __MATH_H_
#define __MATH_H_

#include <cmath>

//namespace AnYing
//{

const float PI = 3.1415926535897932384626433832795f;
const float TWO_PI = 2.0f * PI;
const float HALF_PI = 0.5f * PI;
const float SQRT_3 = sqrtf(3.0f);
const float HALF_SQRT_3 = 0.5f * sqrtf(3.0f);
const float EPSILON = 0.0001f;
const float SQREPSILON = 0.000001f;
const float DEG2RAD = 3.141593f / 180;

///*! Value of sqrt(3) */
//extern const float SQRT_3;

///*! Value of 0.5 * sqrt(3) */
//extern const float HALF_SQRT_3;

///*! Value of pi */
//extern const float PI;

///*! Value of 2.0 * pi */
//extern const float TWO_PI;

///*! Value of 0.5 * pi */
//extern const float HALF_PI;

///*! Float comparison threshold ( two float values fOne and fTwo are considered equal if fabsf( fOne - fTwo ) < EPSILON ) */
//extern const float EPSILON;

///*! Float comparison threshold for squares */
//extern const float SQREPSILON;

template <class T> inline const T                &MIN1(const T &a, const T &b) { return((a < b) ? a : b); }

template <class T> inline const T                &MAX1(const T &a, const T &b) { return((a > b) ? a : b); }

template <class T> inline const T                &MIN3(const T &a, const T &b, const T &c) { return((a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c)); }

template <class T> inline const T                &MAX3(const T &a, const T &b, const T &c) { return((a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c)); }

template <class T> inline void                    MINMAX(const T &a, const T &b, T &min, T &max) { if (a < b) { min = a; max = b; } else { min = b; max = a; } }

template <class T> inline void                    MINMAX3(const T &a, const T &b, const T &c, T &min, T &max) { min = MIN3(a, b, c); max = MAX3(a, b, c); }

template <class T> inline const T                 CLAMP(const T &val, const T &min, const T &max) { if (val <= min) return min; else if (val >= max) return max; return val; }

/**
* Convert from degrees to radians
* \param fA      Angle in degrees
* \return        Angle in radians
*/
inline float                        DEGTORAD(float fA) { return fA * 0.017453292519943295769236907684886f; }

/**
* Round float to nearest int
* Warning: x86 assembly, non-x86 just casts (truncates)
* \param fNum           Value
* \return               Closest int
*/
inline int                          RoundFloat(float fNum);

/**
* Drop float to next lowest int (use this instead of float->int casts)
* Warning: Not implemented, just casts.
* \param fNum           Value
* \return               Next lowest int
*/
inline int                          TruncFloat(float fNum);

/**
* Calculate inverse square root of value
* \param fN             Value
* \return               Inverse squre root of value, 1/sqrt(fN)
*/
inline float                        FastInvSqrt(float fN);

//	inline float                        fabsf(float _fv);





	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                                Quaternion                                         
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////


//}
#endif
