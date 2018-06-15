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

#ifndef __XLI_HASH_H__
#define __XLI_HASH_H__

#include <Xli/IntTypes.h>
#include <Xli/String.h>

namespace Xli
{
    /**
        \ingroup XliTraits
    */
    struct DefaultTraits
    {
        template <typename T>
        static bool Equals(const T& a, const T& b)
        {
            return a == b;
        }

        static UInt32 Hash(const UInt8* data, int size);

        static UInt32 Hash(bool b);
        static UInt32 Hash(const void* p);
        static UInt32 Hash(float f);
        static UInt32 Hash(double d);

        static UInt32 Hash(Int8 i);
        static UInt32 Hash(UInt8 i);
        static UInt32 Hash(Int16 i);
        static UInt32 Hash(UInt16 i);
        static UInt32 Hash(Int32 i);
        static UInt32 Hash(UInt32 i);
        static UInt32 Hash(Int64 i);
        static UInt32 Hash(UInt64 i);
        static UInt32 Hash(const String& str);

        template <typename T> 
        static UInt32 Hash(const T* t) 
        { 
            return Hash((const void*)t); 
        }
        
        template <typename T> 
        static UInt32 Hash(T* t) 
        { 
            return Hash((const void*)t); 
        }

        template <typename T> 
        static UInt32 Hash(const T& t) 
        { 
            return Hash((const UInt8*)&t, (int)sizeof(T)); 
        }

        static String ToString(void* p);
        static String ToString(int i);
        static String ToString(unsigned int i);
        static String ToString(short i);
        static String ToString(unsigned short i);
        static String ToString(long i);
        static String ToString(unsigned long i);
        static String ToString(long long i);
        static String ToString(unsigned long long i);
        static String ToString(float f);
        static String ToString(double d);
        static String ToString(char c);
        static String ToString(unsigned char c);
        static String ToString(bool b);

        template <typename T> 
        static String ToString(const T& t)
        {
            return t.ToString();
        }
    };

    /**
        \ingroup XliTraits
    */
    template <typename T> struct Traits
    {
        static bool Equals(const T& a, const T& b) 
        {
            return DefaultTraits::Equals(a, b);
        }

        static UInt32 Hash(const T& t) 
        {
            return DefaultTraits::Hash(t);
        }

        static String ToString(const T& t)
        {
            return DefaultTraits::ToString(t);
        }
    };
}

#endif
