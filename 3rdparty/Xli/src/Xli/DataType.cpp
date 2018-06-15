//
// Copyright (C) 2010-2014 Outracks Technologies
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial
// portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
// OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include <Xli/DataType.h>
#include <Xli/Exception.h>

namespace Xli
{
    String DataTypeInfo::ToString(DataType dt)
    {
        switch (dt)
        {
        case DataTypeHalf: return "Half";
        case DataTypeFloat: return "Float";
        case DataTypeDouble: return "Double";
        case DataTypeUInt64: return "UInt64";
        case DataTypeUInt32: return "UInt32";
        case DataTypeUInt24: return "UInt24";
        case DataTypeUInt16: return "UInt16";
        case DataTypeUInt8: return "UInt8";
        case DataTypeInt64: return "Int64";
        case DataTypeInt32: return "Int32";
        case DataTypeInt24: return "Int24";
        case DataTypeInt16: return "Int16";
        case DataTypeInt8: return "Int8";
        default: return "<Unknown DataType>";
        }
    }

    int DataTypeInfo::SizeOf(DataType dt)
    {
        switch (dt)
        {
        case DataTypeHalf: return 2;
        case DataTypeDouble: return 8;
        case DataTypeFloat: return 4;
        case DataTypeUInt64: return 4;
        case DataTypeUInt32: return 4;
        case DataTypeUInt24: return 3;
        case DataTypeUInt16: return 2;
        case DataTypeUInt8: return 1;
        case DataTypeInt64: return 8;
        case DataTypeInt32: return 4;
        case DataTypeInt24: return 3;
        case DataTypeInt16: return 2;
        case DataTypeInt8: return 1;
        default: XLI_THROW("Unknown size for " + (String)DataTypeInfo::ToString(dt));
        }
    }

    bool DataTypeInfo::IsSigned(DataType dt)
    {
        switch (dt)
        {
        case DataTypeHalf:
        case DataTypeDouble:
        case DataTypeFloat:
        case DataTypeInt64:
        case DataTypeInt32:
        case DataTypeInt24:
        case DataTypeInt16:
        case DataTypeInt8:
            return true;

        case DataTypeUInt64:
        case DataTypeUInt32:
        case DataTypeUInt24:
        case DataTypeUInt16:
        case DataTypeUInt8:
            return false;

        default: 
            XLI_THROW("Unknown sign for " + (String)DataTypeInfo::ToString(dt));
        }
    }
}
