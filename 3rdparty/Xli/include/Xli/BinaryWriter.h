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

#ifndef __XLI_BINARY_WRITER_H__
#define __XLI_BINARY_WRITER_H__

#include <Xli/Stream.h>
#include <Xli/Vector4.h>

namespace Xli
{
    /**
        \ingroup XliIO
    */
    class BinaryWriter : public StreamWriterBase
    {
    public:
        BinaryWriter(Stream* stream);
        virtual ~BinaryWriter();

        template <typename T> 
        void Write(const T& elm)
        {
            _stream->Write((const void*)&elm, sizeof(T), 1);
        }

        template <typename T> void Write(const Vector2t<T>& vec) { _stream->Write(vec.Ptr(), sizeof(T), 2); }
        template <typename T> void Write(const Vector3t<T>& vec) { _stream->Write(vec.Ptr(), sizeof(T), 3); }
        template <typename T> void Write(const Vector4t<T>& vec) { _stream->Write(vec.Ptr(), sizeof(T), 4); }

        void WriteFloat(const float& elm) { Write<float>(elm); }
        void WriteDouble(const double& elm) { Write<double>(elm); }
        void WriteInt64(const Int64& elm) { Write<Int64>(elm); }
        void WriteInt32(const Int32& elm) { Write<Int32>(elm); }
        void WriteInt16(const Int16& elm) { Write<Int16>(elm); }
        void WriteInt8(const Int8& elm) { Write<Int8>(elm); }
        void WriteUInt64(const UInt64& elm) { Write<UInt64>(elm); }
        void WriteUInt32(const UInt32& elm) { Write<UInt32>(elm); }
        void WriteUInt16(const UInt16& elm) { Write<UInt16>(elm); }
        void WriteUInt8(const UInt8& elm) { Write<UInt8>(elm); }
        void WriteBool(const bool& elm) { Write<UInt8>(elm? 1: 0); }

        void Write(const void* data, int elmSize, int elmCount);
        void WriteCStr(const char* str);

        void Write7BitEncodedInt32(Int32 value);
        
        /// .NET compatible string writer
        void WriteString(const String& str);
    };
}

#endif
