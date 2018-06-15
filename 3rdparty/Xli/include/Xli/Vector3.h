/* * Copyright (C) 2010-2014 Outracks Technologies
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __XLI_VECTOR3_H__
#define __XLI_VECTOR3_H__

#include <Xli/Vector2.h>

namespace Xli
{
    /**
        \ingroup XliDataTypes
    */
    template <typename T> class Vector3t
    {
    public:
        union
        {
            struct { T X, Y, Z; };
            struct { T R, G, B; };
            T Data[3];
        };

        String ToString() const
        {
            return String(X) + ", " + Y + ", " + Z;
        }

        Vector2t<T> XY() const
        {
            return Vector2t<T>(X, Y);
        }

        Vector3t(const Vector2t<T>& xy, T z)
        {
            X = xy.X;
            Y = xy.Y;
            Z = z;
        }

        Vector3t()
        {
        }

        Vector3t(const T& x, const T& y, const T& z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        Vector3t(const Vector3t& v)
        {
            X = v.X;
            Y = v.Y;
            Z = v.Z;
        }

        template <typename U> 
        explicit Vector3t(const Vector3t<U>& v)
        {
            X = (T)v.X;
            Y = (T)v.Y;
            Z = (T)v.Z;
        }

        bool operator == (const Vector3t& v) const
        {
            return (X == v.X) && (Y == v.Y) && (Z == v.Z);
        }

        bool operator != (const Vector3t& v) const
        {
            return !(*this == v);
        }

        Vector3t& operator = (const Vector3t& v)
        {
            X = v.X;
            Y = v.Y;
            Z = v.Z;
            return *this;
        }

        Vector3t operator - () const
        {
            Vector3t r;
            r.X = -X;
            r.Y = -Y;
            r.Z = -Z;
            return r;
        }

        Vector3t operator + (const Vector3t& v) const
        {
            Vector3t r;
            r.X = X + v.X;
            r.Y = Y + v.Y;
            r.Z = Z + v.Z;
            return r;
        }

        Vector3t operator - (const Vector3t& v) const
        {
            Vector3t r;
            r.X = X - v.X;
            r.Y = Y - v.Y;
            r.Z = Z - v.Z;
            return r;
        }

        Vector3t operator * (const Vector3t& v) const
        {
            Vector3t r;
            r.X = X * v.X;
            r.Y = Y * v.Y;
            r.Z = Z * v.Z;
            return r;
        }

        Vector3t operator / (const Vector3t& v) const
        {
            Vector3t r;
            r.X = X / v.X;
            r.Y = Y / v.Y;
            r.Z = Z / v.Z;
            return r;
        }

        Vector3t operator + (const T& s) const
        {
            Vector3t r;
            r.X = X + s;
            r.Y = Y + s;
            r.Z = Z + s;
            return r;
        }

        Vector3t operator - (const T& s) const
        {
            Vector3t r;
            r.X = X - s;
            r.Y = Y - s;
            r.Z = Z - s;
            return r;
        }

        Vector3t operator * (const T& s) const
        {
            Vector3t r;
            r.X = X * s;
            r.Y = Y * s;
            r.Z = Z * s;
            return r;
        }

        Vector3t operator / (const T& s) const
        {
            Vector3t r;
            r.X = X / s;
            r.Y = Y / s;
            r.Z = Z / s;
            return r;
        }

        Vector3t& operator += (const Vector3t& v)
        {
            X += v.X;
            Y += v.Y;
            Z += v.Z;
            return *this;
        }

        Vector3t& operator -= (const Vector3t& v)
        {
            X -= v.X;
            Y -= v.Y;
            Z -= v.Z;
            return *this;
        }

        Vector3t& operator *= (const Vector3t& v)
        {
            X *= v.X;
            Y *= v.Y;
            Z *= v.Z;
            return *this;
        }

        Vector3t& operator /= (const Vector3t& v)
        {
            X /= v.X;
            Y /= v.Y;
            Z /= v.Z;
            return *this;
        }

        Vector3t& operator += (const T& s)
        {
            X += s;
            Y += s;
            Z += s;
            return *this;
        }

        Vector3t& operator -= (const T& s)
        {
            X -= s;
            Y -= s;
            Z -= s;
            return *this;
        }

        Vector3t& operator *= (const T& s)
        {
            X *= s;
            Y *= s;
            Z *= s;
            return *this;
        }

        Vector3t& operator /= (const T& s)
        {
            X /= s;
            Y /= s;
            Z /= s;
            return *this;
        }
    };

    /**
        \addtogroup XliDataTypes
        @{
    */

    typedef Vector3t<float> Vector3;
    typedef Vector3t<double> Vector3d;
    typedef Vector3t<int> Vector3i;
    typedef Vector3t<unsigned int> Vector3u;

    typedef Vector3t<Int64> Vector3i64;
    typedef Vector3t<Int32> Vector3i32;
    typedef Vector3t<Int16> Vector3i16;
    typedef Vector3t<Int8> Vector3i8;
    typedef Vector3t<UInt64> Vector3u64;
    typedef Vector3t<UInt32> Vector3u32;
    typedef Vector3t<UInt16> Vector3u16;
    typedef Vector3t<UInt8> Vector3u8;

    /** @} */
}

#endif
