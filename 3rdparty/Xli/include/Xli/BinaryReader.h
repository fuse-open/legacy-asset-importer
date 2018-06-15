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

#ifndef __XLI_BINARY_READER_H__
#define __XLI_BINARY_READER_H__

#include <Xli/Stream.h>
#include <Xli/Vector4.h>

namespace Xli
{
    /**
        \ingroup XliIO
    */
    class BinaryReader : public StreamReaderBase
    {
    public:
        BinaryReader(Stream* stream);
        virtual ~BinaryReader();

        template <typename T> 
        void Read(T& elm)
        {
            _stream->ReadSafe((void*)&elm, sizeof(T), 1);
        }

        template <typename T> void Read(Vector2t<T>& vec) { _stream->ReadSafe(vec.Ptr(), sizeof(T), 2); }
        template <typename T> void Read(Vector3t<T>& vec) { _stream->ReadSafe(vec.Ptr(), sizeof(T), 3); }
        template <typename T> void Read(Vector4t<T>& vec) { _stream->ReadSafe(vec.Ptr(), sizeof(T), 4); }

        template <typename T> T Read()
        {
            T elm;
            Read(elm);
            return elm;
        }

        float ReadFloat() { return Read<float>(); }
        double ReadDouble() { return Read<double>(); }
        Int64 ReadInt64() { return Read<Int64>(); }
        Int32 ReadInt32() { return Read<Int32>(); }
        Int16 ReadInt16() { return Read<Int16>(); }
        Int8 ReadInt8() { return Read<Int8>(); }
        UInt64 ReadUInt64() { return Read<UInt64>(); }
        UInt32 ReadUInt32() { return Read<UInt32>(); }
        UInt16 ReadUInt16() { return Read<UInt16>(); }
        UInt8 ReadUInt8() { return Read<UInt8>(); }
        bool ReadBool() { return Read<UInt8>() != 0; }

        template <typename T> Vector2t<T> ReadVector2t() { Vector2t<T> vec; Read(vec); return vec; }
        template <typename T> Vector3t<T> ReadVector3t() { Vector3t<T> vec; Read(vec); return vec; }
        template <typename T> Vector4t<T> ReadVector4t() { Vector4t<T> vec; Read(vec); return vec; }

        void Read(void* dst, int elmSize, int elmCount);
        String ReadCStr(int len);

        Buffer* ReadAll();

        Int32 Read7BitEncodedInt32();

        /// .NET compatible string reader
        String ReadString();
    };
}

#endif
