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

#ifndef __XLI_RECTANGLE_H__
#define __XLI_RECTANGLE_H__

#include <Xli/Vector2.h>
#include <Xli/Exception.h>

namespace Xli
{
    /**
        \ingroup XliDataTypes

        Represents the coordinates of a rectangle.
        The rectangle is represented in a Left, Top, Right, Bottom-notation, where the X-axis increases
        in the right direction, where the Y-axis increases in the down direction.
        The width of the rectangle is defined to be (Right-Left), and the height is (Bottom-Top). Note that
        the elements at column Right and row Bottom is not included in the rectangle.
    */
    template <typename T> class Rectanglet
    {
    public:
        union
        {
            struct
            {
                // The lowest X-coordinate of the rectangle
                T Left;

                // The lowest Y-coordinate of the rectangle
                T Top;

                // One plus the highest X-coordinate of the interior of the rectangle
                T Right;

                // One plus the highest Y-coordinate of the interior of the rectangle
                T Bottom;
            };

            T Corners[4];
        };

        operator T*()
        {
            return Corners;
        }
        
        T& operator [] (int i)
        {
#ifdef XLI_RANGE_CHECK
            if (i >= 4 || i < 0)
                XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif
            return Corners[i];
        }
        
        String ToString() const
        {
            return String(Left) + ", " + Top + ", " + Right + ", " + Bottom;
        }

        Rectanglet()
        {
        }
        
        Rectanglet(T left, T top, T right, T bottom)
        {
            Left = left;
            Top = top;
            Right = right;
            Bottom = bottom;
        }
        
        Rectanglet(const Rectanglet& v)
        {
            Left = v.Left;
            Top = v.Top;
            Right = v.Right;
            Bottom = v.Bottom;
        }
        
        template <typename U> explicit Rectanglet(const Rectanglet<U>& v)
        {
            Left = (T)v.Left;
            Top = (T)v.Top;
            Right = (T)v.Right;
            Bottom = (T)v.Bottom;
        }
        
        Rectanglet& operator = (const Rectanglet& v)
        {
            Left = v.Left;
            Top = v.Top;
            Right = v.Right;
            Bottom = v.Bottom;
            return *this;
        }

        Rectanglet(const Vector2t<T>& position, const Vector2t<T>& size)
        {
            Left = position.X;
            Top = position.Y;
            Right = Left + size.X;
            Bottom = Top + size.Y;
        }

        /**
            Returns the i'th corner of the rectangle.
            The corners are enumerated as follows: 0=upper left, 1=lower left, 2=lower right, 3=upper right.
            If the index is out of range, an IndexOutOfBoundsException is thrown.
        */
        Vector2t<T> GetCorner(int i)
        {
            switch (i)
            {
            case 0: return Vector2t<T>(Left, Top);
            case 1: return Vector2t<T>(Left, Bottom);
            case 2: return Vector2t<T>(Right, Bottom);
            case 3: return Vector2t<T>(Right, Top);
            }
        
            XLI_THROW_INDEX_OUT_OF_BOUNDS;
        }

        T Width() const
        {
            return Right - Left;
        }
        
        T Height() const
        {
            return Bottom - Top;
        }

        Vector2t<T> Position() const
        {
            return Vector2t<T>(Left, Top);
        }
        
        Vector2t<T> Size() const
        {
            return Vector2t<T>(Width(), Height());
        }

        Vector2t<T> TopLeft() const
        {
            return Vector2t<T>(Left, Top);
        }
        
        Vector2t<T> TopRight() const
        {
            return Vector2t<T>(Right, Top);
        }
        
        Vector2t<T> BottomLeft() const
        {
            return Vector2t<T>(Left, Bottom);
        }
        
        Vector2t<T> BottomRight() const
        {
            return Vector2t<T>(Right, Bottom);
        }

        Vector2t<T> Center() const
        {
            return Vector2t<T>(Left + Width() / 2, Top + Height() / 2);
        }

        T Area() const
        {
            return Width() * Height();
        }

        void Move(const Vector2t<T>& dp)
        {
            Move(dp.X, dp.Y);
        }

        void Move(const T& dX, const T& dY)
        {
            Left += dX;
            Top += dY;
            Right += dX;
            Bottom += dY;
        }
        
        void MoveTo(const Vector2t<T>& dp)
        {
            MoveTo(dp.X, dp.Y);
        }

        void MoveTo(const T& x, const T& y)
        {
            Move(x - Left, y - Top);
        }

        void Resize(const T& width, const T& height)
        {
            Right = Left + width;
            Bottom = Top + height;
        }

        bool Intersects(const T& x, const T& y) const
        {
            if (x < Left) return false;
            if (x > Right) return false;
            if (y < Top) return false;
            if (y > Bottom) return false;
            return true;
        }

        bool Intersects(const Rectanglet& rect) const
        {
            return (! (Left > rect.Right
                    || Right < rect.Left
                    || Top > rect.Bottom
                    || Bottom < rect.Top));
        }

        /**
            Return this rectangle clipped against a scissor rectangle.
        */
        Rectanglet Clip(const Rectanglet& scissor) const
        {
            Rectanglet r;
            r.Left = Max(Left, scissor.Left);
            r.Right = Min(Right, scissor.Right);
            r.Top = Max(Top, scissor.Top);
            r.Bottom = Min(Bottom, scissor.Bottom);
            return r;
        }

        Rectanglet operator + (const Vector2t<T>& offset) const
        {
            Rectanglet r;
            r.Left = Left + offset.X;
            r.Right = Right + offset.X;
            r.Top = Top + offset.Y;
            r.Bottom = Bottom + offset.Y;
            return r;
        }

        Rectanglet operator - (const Vector2t<T>& offset) const
        {
            return *this + (-offset);
        }
    };

    /**
        \addtogroup XliDataTypes
        @{
    */

    typedef Rectanglet<float> Rect;
    typedef Rectanglet<double> Rectd;
    typedef Rectanglet<int> Recti;

    /** @} */
}


#endif
