#ifndef _MATHF_H_
#define _MATHF_H_

#include <cmath>
#include <math.h>

struct Mathf
{
public:
	static const float Deg2Rad;
	static const float Infinity;
	static const float NegativeInfinity;
	static const float PI;
	static const float Rad2Deg;

	static float Epsilon;

	static float Abs(float f);
	static int Abs(int value);
	static float Acos(float f);
	static bool Approximately(float a, float b);
	static float Asin(float f);
	static float Atan(float f);
	static float Atan2(float y, float x);
	static float Ceil(float f);
	static int CeilToInt(float f);
	static float Clamp(float value, float min, float max);
	static int Clamp(int value, int min, int max);
	static float Clamp01(float value);
	static int ClosestPowerOfTwo(int value);
	//static Color CorrelatedColorTemperatureToRGB(float kelvin);
	static float Cos(float f);
	static float DeltaAngle(float current, float target);
	static float Exp(float power);
	static unsigned short FloatToHalf(float val);
	static float Floor(float f);
	static int FloorToInt(float f);
	static float Gamma(float value, float absmax, float gamma);
	static float GammaToLinearSpace(float value);
	static float HalfToFloat(unsigned short val);
	static float InverseLerp(float a, float b, float value);
	static bool IsPowerOfTwo(int value);
	static float Lerp(float a, float b, float t);
	static float LerpAngle(float a, float b, float t);
	static float LerpUnclamped(float a, float b, float t);
	static float LinearToGammaSpace(float value);
	static float Log(float f);
	static float Log(float f, float p);
	static float Log10(float f);
	//static float Max(float[] values);
	//static int Max(int[] values);
	static float Max(float a, float b);
	static int Max(int a, int b);
	//static float Min(float[] values);
	//static int Min(int[] values);
	static float Min(float a, float b);
	static int Min(int a, int b);
	static float MoveTowards(float current, float target, float maxDelta);
	static float MoveTowardsAngle(float current, float target, float maxDelta);
	static int NextPowerOfTwo(int value);
	static float PerlinNoise(float x, float y);
	static float PingPong(float t, float length);
	static float Pow(float f, float p);
	static float Repeat(float t, float length);
	static float Round(float f);
	static int RoundToInt(float f);
	static float Sign(float f);
	static float Sin(float f);
	static float SmoothDamp(float current, float target, float& currentVelocity, float smoothTime);
	static float SmoothDamp(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed);
	static float SmoothDamp(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed, float deltaTime);
	static float SmoothDampAngle(float current, float target, float& currentVelocity, float smoothTime);
	static float SmoothDampAngle(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed);
	static float SmoothDampAngle(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed, float deltaTime);
	static float SmoothStep(float from, float to, float t);
	static float Sqrt(float f);
	static float Tan(float f);
};
#endif
