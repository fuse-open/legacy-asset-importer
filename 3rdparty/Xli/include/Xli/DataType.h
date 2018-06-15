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

#ifndef __XLI_DATA_TYPE_H__
#define __XLI_DATA_TYPE_H__

#include <Xli/IntTypes.h>
#include <Xli/String.h>

namespace Xli
{
    /**
        \ingroup XliDataTypes
    */
    enum DataType
    {
        DataTypeHalf = 1,
        DataTypeFloat = 2,
        DataTypeDouble = 3,
        DataTypeUInt64 = 4,
        DataTypeUInt32 = 5,
        DataTypeUInt24 = 6,
        DataTypeUInt16 = 7,
        DataTypeUInt8 = 8,
        DataTypeInt64 = 9,
        DataTypeInt32 = 10,
        DataTypeInt24 = 11,
        DataTypeInt16 = 12,
        DataTypeInt8 = 13
    };

    /**
        \ingroup XliDataTypes
    */
    class DataTypeInfo
    {
    public:
        static int SizeOf(DataType dt);
        static bool IsSigned(DataType dt);
        static String ToString(DataType dt);
    };
}


#endif
