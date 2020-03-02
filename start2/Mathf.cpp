#include "Mathf.h"

const float Mathf::Deg2Rad = 0.0174533f;
const float Mathf::Infinity = 1.0f;	//ÐÞ¸Ä
const float Mathf::NegativeInfinity = -1.0f;//ÐÞ¸Ä
const float Mathf::PI = 3.14159f;
const float Mathf::Rad2Deg = 57.2958f;

float Mathf::Abs(float f) { return fabsf(f); }
int Mathf::Abs(int value) { return labs(value); };
float Mathf::Acos(float f) { return acosf(f); };
bool Mathf::Approximately(float a, float b) { return false; };
float Mathf::Asin(float f) { return asinf(f); };
float Mathf::Atan(float f) { return atanf(f); };
float Mathf::Atan2(float y, float x) { return atan2f(y, x); };
float Mathf::Ceil(float f) { return ceilf(f); };
int Mathf::CeilToInt(float f) { return ceil(f); };
float Mathf::Clamp(float value, float min, float max) { if (value <= min) return min; else if (value >= max) return max; return value; };
int Mathf::Clamp(int value, int min, int max) { if (value <= min) return min; else if (value >= max) return max; return value; };
float Mathf::Clamp01(float value) { if (value <= 0) return 0; else if (value >= 1) return 1; return value; };
int Mathf::ClosestPowerOfTwo(int value) { return 0; }
//Color Mathf::CorrelatedColorTemperatureToRGB(float kelvin);
float Mathf::Cos(float f) { return cosf(f); }
float Mathf::DeltaAngle(float current, float target) { return 0; }
float Mathf::Exp(float power) { return exp(power); }
unsigned short Mathf::FloatToHalf(float val) { return 0; };
float Mathf::Floor(float f) { return floorf(f); };
int Mathf::FloorToInt(float f) { return floor(f); };
float Mathf::Gamma(float value, float absmax, float gamma) { return 0; }
float Mathf::GammaToLinearSpace(float value) { return 1.0f; }
float Mathf::HalfToFloat(unsigned short val) { return 0; }
float Mathf::InverseLerp(float a, float b, float value) { return 0; };
bool Mathf::IsPowerOfTwo(int value) { return 0; };
float Mathf::Lerp(float a, float b, float t) { return (1 - t) * a + t * b; }
float Mathf::LerpAngle(float a, float b, float t) { return (1 - t) * a + t * b; }
float Mathf::LerpUnclamped(float a, float b, float t) { return (1 - t) * a + t * b; }
float Mathf::LinearToGammaSpace(float value) { return 0; }
float Mathf::Log(float f) { return logf(f); };
float Mathf::Log(float f, float p) { return 0; };
float Mathf::Log10(float f) { return log10f(f); };
//float Mathf::Max(params float[] values);
//int Mathf::Max(params int[] values);
float Mathf::Max(float a, float b) { return a > b ? a : b; };
int Mathf::Max(int a, int b) { return a > b ? a : b; };
//float Mathf::Min(params float[] values);
//int Mathf::Min(params int[] values);
float Mathf::Min(float a, float b) { return a < b ? a : b; };
int Mathf::Min(int a, int b) { return a < b ? a : b; };
float Mathf::MoveTowards(float current, float target, float maxDelta) { return 0; }
float Mathf::MoveTowardsAngle(float current, float target, float maxDelta) { return 0; }
int Mathf::NextPowerOfTwo(int value) { return 0; }
float Mathf::PerlinNoise(float x, float y) { return 0; }
float Mathf::PingPong(float t, float length) { return 0; }
float Mathf::Pow(float f, float p) { return powf(f, p); }
float Mathf::Repeat(float t, float length) { return 0; };
float Mathf::Round(float f) { return roundf(f); };
int Mathf::RoundToInt(float f) { return (f > 0.0) ? FloorToInt(f + 0.5) : CeilToInt(f - 0.5); };
float Mathf::Sign(float f) { return 0; };
float Mathf::Sin(float f) { return sin(f); };
float Mathf::SmoothDamp(float current, float target, float& currentVelocity, float smoothTime) { return 0; };
float Mathf::SmoothDamp(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed) { return 0; };
float Mathf::SmoothDamp(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed, float deltaTime) { return 0; };
float Mathf::SmoothDampAngle(float current, float target, float& currentVelocity, float smoothTime) { return 0; };
float Mathf::SmoothDampAngle(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed) { return 0; };
float Mathf::SmoothDampAngle(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed, float deltaTime) { return 0; };
float Mathf::SmoothStep(float from, float to, float t) { return (1 - t) * from + t * to; }
float Mathf::Sqrt(float f) { return sqrt(f); }
float Mathf::Tan(float f) { return tan(f); };