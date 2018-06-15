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

#include <Xli/BinaryReader.h>

namespace Xli
{
    BinaryReader::BinaryReader(Stream* stream)
        : StreamReaderBase(stream)
    {
    }

    BinaryReader::~BinaryReader()
    {
    }

    void BinaryReader::Read(void* dst, int elmSize, int elmCount)
    {
        _stream->ReadSafe(dst, elmSize, elmCount);
    }

    String BinaryReader::ReadCStr(int len)
    {
        String str = String::Create(len);
        _stream->ReadSafe((void*)str.Ptr(), 1, len);
        return str;
    }

    String BinaryReader::ReadString()
    {
        return ReadCStr(Read7BitEncodedInt32());
    }

    Buffer* BinaryReader::ReadAll()
    {
        _stream->Seek(0, SeekOriginBegin);
        int size = _stream->GetLength();
        Buffer* buf = Buffer::Create(size);
        _stream->ReadSafe(buf->Ptr(), 1, size);
        return buf;
    }

    Int32 BinaryReader::Read7BitEncodedInt32()
    {
        // TODO: Protect against forever while loop (ref: .NET impl)

        int count = 0;
        int shift = 0;
        int b;
        
        do 
        {
            b = ReadUInt8();
            count |= (b & 0x7F) << shift;
            shift += 7;
        }
        while ((b & 0x80) != 0);
        
        return count;
    }
}
