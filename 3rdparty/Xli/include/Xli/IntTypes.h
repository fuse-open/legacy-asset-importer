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

#ifndef __XLI_INT_TYPES_H__
#define __XLI_INT_TYPES_H__

#include <Xli/Config.h>

namespace Xli
{
    /**
        \addtogroup XliDataTypes
        @{
    */

    typedef signed char Int8;
    typedef signed short Int16;
    typedef signed int Int32;
    typedef signed long long Int64;
    
    typedef unsigned char UInt8;
    typedef unsigned short UInt16;
    typedef unsigned int UInt32;
    typedef unsigned long long UInt64;

    static const Int8 Int8Max = 0x7f;
    static const UInt8 UInt8Max = 0xff;
    static const Int16 Int16Max = 0x7fff;
    static const UInt16 UInt16Max = 0xffff;
    static const Int32 Int32Max = 0x7fffffff;
    static const UInt32 UInt32Max = 0xffffffff;
    static const Int64 Int64Max = 0x7fffffffffffffffll;
    static const UInt64 UInt64Max = 0xffffffffffffffffull;

    /** @} */

    /** \ingroup XliText */
#ifdef WIN32
    typedef wchar_t Utf16;
#else
    typedef UInt16 Utf16;
#endif

    /** \ingroup XliText */
    typedef UInt32 Utf32;
    
    /** \ingroup XliTime */
    typedef UInt64 Timestamp;

    /**
        \addtogroup XliContainers
        @{
    */

    template <typename T> class Array;

    /** @} */

    // Verify size of int types at compile time
    typedef char __TestInt8[sizeof(Int8) == 1 ? 1 : -1];
    typedef char __TestInt16[sizeof(Int16) == 2 ? 1 : -1];
    typedef char __TestInt32[sizeof(Int32) == 4 ? 1 : -1];
    typedef char __TestInt64[sizeof(Int64) == 8 ? 1 : -1];
    typedef char __TestUInt8[sizeof(UInt8) == 1 ? 1 : -1];
    typedef char __TestUInt16[sizeof(UInt16) == 2 ? 1 : -1];
    typedef char __TestUInt32[sizeof(UInt32) == 4 ? 1 : -1];
    typedef char __TestUInt64[sizeof(UInt64) == 8 ? 1 : -1];
    typedef char __TestUtf16[sizeof(Utf16) == 2 ? 1 : -1];
}

#endif
