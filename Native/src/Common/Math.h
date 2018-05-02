#ifndef __CRUNCH_MATH_H__
#define __CRUNCH_MATH_H__

#include <Xli/Vector4.h>
#include <cmath>
#include <cfloat>
#include <climits>

/**
    \addtogroup CrunchMath
    @{
*/

static const double PI = 3.1415926535897932384626433832795;
static const float PIf = 3.1415926535897932384626433832795f;
static const float FloatZeroTolerance = 1e-4f;

template <typename T> 
T SignPositiveZero(const T& x)
{
    return (x < (T)0) ? (T)-1 : (T)1;
}

template <typename T> 
T Sign(const T& x)
{
    return x == (T)-0 || x == (T)0 ? x : SignPositiveZero(x);
}

template <typename T> 
T Min(const T& a, const T& b)
{
    return (a < b) ? a : b;
}

template <typename T> 
T Max(const T& a, const T& b)
{
    return (a > b) ? a : b;
}

template <typename T> 
T Abs(const T& a)
{
    return (a < (T)0) ? -a : a;
}

template <typename T> 
T Clamp(const T& x, const T& minimum, const T& maximum)
{
    return Max(minimum, Min(maximum, x));
}

template <typename T> 
T Saturate(const T& x)
{
    return Clamp(x, T(0), T(1));
}


XLI_INLINE float Sin(float x)
{
    return sinf(x);
}

XLI_INLINE float Cos(float x)
{
    return cosf(x);
}

XLI_INLINE float Tan(float x)
{
    return tanf(x);
}

XLI_INLINE float ArcSin(float x)
{
    return asinf(x);
}

XLI_INLINE float ArcCos(float x)
{
    return acosf(x);
}

XLI_INLINE float ArcTan(float x)
{
    return atanf(x);
}

XLI_INLINE float Log(float x)
{
    return logf(x);
}

XLI_INLINE float Log10(float x)
{
    return log10f(x);
}

XLI_INLINE float Sqrt(float x)
{
    return sqrtf(x);
}

XLI_INLINE float Pow(float base, float exponent)
{
    return powf(base, exponent);
}

XLI_INLINE float Exp(float x)
{
    return expf(x);
}

XLI_INLINE float Fmod(float x, float y)
{
    return fmodf(x, y);
}


XLI_INLINE double Sin(double x)
{
    return sin(x);
}

XLI_INLINE double Cos(double x)
{
    return cos(x);
}

XLI_INLINE double Tan(double x)
{
    return tan(x);
}

XLI_INLINE double ArcSin(double x)
{
    return asin(x);
}

XLI_INLINE double ArcCos(double x)
{
    return acos(x);
}

XLI_INLINE double ArcTan(double x)
{
    return atan(x);
}

XLI_INLINE double Log(double x)
{
    return log(x);
}

XLI_INLINE double Log10(double x)
{
    return log10(x);
}

XLI_INLINE double Sqrt(double x)
{
    return sqrt(x);
}

XLI_INLINE double Pow(double base, double exponent)
{
    return pow(base, exponent);
}

XLI_INLINE double Exp(double x)
{
    return exp(x);
}

XLI_INLINE double Fmod(double x, double y)
{
    return fmod(x, y);
}


template <typename T> 
T Log2(const T& x)
{
    return Log(x) / Log((T)2.0);
}

template <typename T> 
T Log(const T& x, const T& base)
{
    return Log(x) / Log(base);
}

template <typename T> 
T Floor(const T& x)
{
    return (T)((x >= (T)0.0)? (int)x: (int)x - 1);
}

template <typename T> 
T Ceil(const T& x)
{
    return (T)((x > (T)0.0)? (int)x + 1: (int)x);
}

template <typename T> 
T Round(const T& x)
{
    return Floor(x + (T)0.5);
}

template <typename T> 
T Frac(const T& x)
{
    return x - Floor(x);
}


XLI_INLINE float RadToDeg(float x)
{
    return x * 180.0f / (float)PI;
}

XLI_INLINE float DegToRad(float x)
{
    return x * (float)PI / 180.0f;
}


template <typename T> 
float MapRange(const T& a, const T& b, const T& t)
{
    return float(t - a) / float(b - a);
}

template <typename T> 
T Lerp(const T& a, const T& b, float t)
{
    return a + (b - a) * t;
}

template <typename T> 
T SCurve(const T& a, const T& b, float t)
{
    return a + (b - a) * (0.5f - Cos(t * (float)PI) * 0.5f);
}

template <typename T> 
T Ramp(const T& a, const T& b, float t)
{
    return a + (b - a) * (t*t);
}

template <typename T> 
T InterpolateCatmullRom(const T& a, const T& b, const T& c, const T& d, float t)
{
    T p = b*3.0f - c*3.0f + d - a;
    T q = a*2.0f - b*5.0f + c*4.0f - d;
    T r = c - a;
    T s = b;
    return (p*(t*t*t) + q*(t*t) + r*t)*0.5f + s;
}

template <typename T> 
T InterpolateCubic(const T& a, const T& b, const T& c, const T& d, float t)
{
    T p = (d - c) - (a - b);
    T q = (a - b) - p;
    T r = c - a;
    T s = b;
    return p*(t*t*t) + q*(t*t) + r*t + s;
}


XLI_INLINE int UpperPow2(int x)
{
    int y = x - 1;
    y = y | (y >> 1);
    y = y | (y >> 2);
    y = y | (y >> 4);
    y = y | (y >> 8);
    y = y | (y >> 16);
    return y + 1;
}

XLI_INLINE bool IsPow2(int x)
{
    return (x == (x & -x));
}

XLI_INLINE float Dot(const Xli::Vector2& v1, const Xli::Vector2& v2)
{
    return v1.X * v2.X + v1.Y * v2.Y;
}

XLI_INLINE float LengthSquared(const Xli::Vector2& v)
{
    return Dot(v, v);
}

XLI_INLINE float Length(const Xli::Vector2& v)
{
    return Sqrt(Dot(v, v));
}

XLI_INLINE Xli::Vector2 Normalize(const Xli::Vector2& v)
{
    return v * (1.0f / Length(v));
}

XLI_INLINE Xli::Vector2 Rotate(const Xli::Vector2& v, float angleRadians)
{
    float c = Cos(angleRadians);
    float s = Sin(angleRadians);

    Xli::Vector2 w;
    w.X = c * v.X + s * v.Y;
    w.Y = c * v.Y - s * v.X;

    return Normalize(w) * Length(v);
}

XLI_INLINE Xli::Vector2 Vector2FromAngle(float angleRadians)
{
    return Xli::Vector2(Cos(angleRadians), Sin(angleRadians));
}

XLI_INLINE float Vector2ToAngle(const Xli::Vector2& v)
{
    float linv = 1.0f / Length(v);
    return v.Y > 0.0f ? ArcCos(v.X * linv) : 2.0f * PIf - ArcCos(v.X * linv);
}

XLI_INLINE float AngleBetween(const Xli::Vector2& v1, const Xli::Vector2& v2)
{
    return ArcCos(Dot(Normalize(v1), Normalize(v2)));
}

XLI_INLINE Xli::Vector3 Cross(const Xli::Vector3& v1, const Xli::Vector3& v2)
{
	return Xli::Vector3(v1.Y * v2.Z - v1.Z * v2.Y,
                        v1.Z * v2.X - v1.X * v2.Z,
                        v1.X * v2.Y - v1.Y * v2.X);
}

XLI_INLINE float Dot(const Xli::Vector3& v1, const Xli::Vector3& v2)
{
    return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z;
}

XLI_INLINE float LengthSquared(const Xli::Vector3& v)
{
    return Dot(v, v);
}

XLI_INLINE float Length(const Xli::Vector3& v)
{
    return Sqrt(Dot(v, v));
}

XLI_INLINE Xli::Vector3 Normalize(const Xli::Vector3& v)
{
    return v / Length(v);
}

XLI_INLINE float Dot(const Xli::Vector4& v1, const Xli::Vector4& v2)
{
    return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z + v1.W * v2.W;
}

XLI_INLINE float LengthSquared(const Xli::Vector4& v)
{
    return Dot(v, v);
}

XLI_INLINE float Length(const Xli::Vector4& v)
{
    return Sqrt(Dot(v, v));
}

XLI_INLINE Xli::Vector4 Normalize(const Xli::Vector4& v)
{
    return v / Length(v);
}

/** @} */

#endif
