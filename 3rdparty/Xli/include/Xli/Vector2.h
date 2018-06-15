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

#ifndef __XLI_VECTOR2_H__
#define __XLI_VECTOR2_H__

#include <Xli/IntTypes.h>
#include <Xli/Exception.h>
#include <Xli/String.h>

namespace Xli
{
    /**
        \ingroup XliDataTypes
    */
    template <typename T> class Vector2t
    {
    public:
        union
        {
            struct { T X, Y; };
            struct { T R, G; };
            T Data[2];
        };

        String ToString() const
        {
            return String(X) + ", " + Y;
        }

        Vector2t()
        {
        }

        Vector2t(const T& x, const T& y)
        {
            X = x;
            Y = y;
        }

        Vector2t(const Vector2t& v)
        {
            X = v.X;
            Y = v.Y;
        }

        template <typename U> 
        explicit Vector2t(const Vector2t<U>& v)
        {
            X = (T)v.X;
            Y = (T)v.Y;
        }

        Vector2t& operator = (const Vector2t& v)
        {
            X = v.X;
            Y = v.Y;
            return *this;
        }

        bool operator == (const Vector2t& v) const
        {
            return X == v.X && Y == v.Y;
        }

        bool operator != (const Vector2t& v) const
        {
            return !(*this == v);
        }

        Vector2t operator - () const
        {
            Vector2t r;
            r.X = -X;
            r.Y = -Y;
            return r;
        }

        Vector2t operator + (const Vector2t& v) const
        {
            Vector2t r;
            r.X = X + v.X;
            r.Y = Y + v.Y;
            return r;
        }

        Vector2t operator - (const Vector2t& v) const
        {
            Vector2t r;
            r.X = X - v.X;
            r.Y = Y - v.Y;
            return r;
        }

        Vector2t operator * (const Vector2t& v) const
        {
            Vector2t r;
            r.X = X * v.X;
            r.Y = Y * v.Y;
            return r;
        }

        Vector2t operator / (const Vector2t& v) const
        {
            Vector2t r;
            r.X = X / v.X;
            r.Y = Y / v.Y;
            return r;
        }

        Vector2t operator + (const T& s) const
        {
            Vector2t r;
            r.X = X + s;
            r.Y = Y + s;
            return r;
        }

        Vector2t operator - (const T& s) const
        {
            Vector2t r;
            r.X = X - s;
            r.Y = Y - s;
            return r;
        }

        Vector2t operator * (const T& s) const
        {
            Vector2t r;
            r.X = X * s;
            r.Y = Y * s;
            return r;
        }

        Vector2t operator / (const T& s) const
        {
            Vector2t r;
            r.X = X / s;
            r.Y = Y / s;
            return r;
        }

        Vector2t& operator += (const Vector2t& v)
        {
            X += v.X;
            Y += v.Y;
            return *this;
        }

        Vector2t& operator -= (const Vector2t& v)
        {
            X -= v.X;
            Y -= v.Y;
            return *this;
        }

        Vector2t& operator *= (const Vector2t& v)
        {
            X *= v.X;
            Y *= v.Y;
            return *this;
        }

        Vector2t& operator /= (const Vector2t& v)
        {
            X /= v.X;
            Y /= v.Y;
            return *this;
        }

        Vector2t& operator += (const T& s)
        {
            X += s;
            Y += s;
            return *this;
        }

        Vector2t& operator -= (const T& s)
        {
            X -= s;
            Y -= s;
            return *this;
        }

        Vector2t& operator *= (const T& s)
        {
            X *= s;
            Y *= s;
            return *this;
        }

        Vector2t& operator /= (const T& s)
        {
            X /= s;
            Y /= s;
            return *this;
        }
    };

    /**
        \addtogroup XliDataTypes
        @{
    */

    typedef Vector2t<float> Vector2;
    typedef Vector2t<double> Vector2d;
    typedef Vector2t<int> Vector2i;
    typedef Vector2t<unsigned int> Vector2u;

    typedef Vector2t<Int64> Vector2i64;
    typedef Vector2t<Int32> Vector2i32;
    typedef Vector2t<Int16> Vector2i16;
    typedef Vector2t<Int8> Vector2i8;
    typedef Vector2t<UInt64> Vector2u64;
    typedef Vector2t<UInt32> Vector2u32;
    typedef Vector2t<UInt16> Vector2u16;
    typedef Vector2t<UInt8> Vector2u8;

    /** @} */
}

#endif
