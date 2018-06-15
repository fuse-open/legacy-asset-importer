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

#include <Xli/BufferStream.h>
#include <cstring>

namespace Xli
{
    BufferStream::BufferStream(DataAccessor* buf, bool canRead, bool canWrite)
    {
        if (!buf) 
            XLI_THROW_NULL_POINTER;
        
        this->buf = buf;
        this->buf->AddRef();
        this->pos = 0;
        this->read = canRead;
        this->write = canWrite;
        this->closed = false;
    }

    BufferStream::~BufferStream()
    {
        buf->Release();
    }

    void BufferStream::Close()
    {
        closed = true;
    }

    bool BufferStream::IsClosed() const
    {
        return closed;
    }

    bool BufferStream::AtEnd() const
    {
        if (closed) 
            XLI_THROW_STREAM_CLOSED;
        
        return pos == buf->GetSizeInBytes();
    }

    bool BufferStream::CanRead() const
    {
        return read;
    }

    bool BufferStream::CanWrite() const
    {
        return write;
    }

    bool BufferStream::CanSeek() const
    {
        return true;
    }

    int BufferStream::GetLength() const
    {
        if (closed) 
            XLI_THROW_STREAM_CLOSED;
        
        return buf->GetSizeInBytes();
    }

    int BufferStream::GetPosition() const
    {
        if (closed) 
            XLI_THROW_STREAM_CLOSED;
        
        return pos;
    }

    int BufferStream::Read(void* data, int elementSize, int elementCount)
    {
        if (!read) 
            XLI_THROW_STREAM_CANT_READ;
        
        if (closed) 
            XLI_THROW_STREAM_CLOSED;

        int bytes = elementCount * elementSize;
        int length = buf->GetSizeInBytes();

        if (bytes > length - pos)
        {
            bytes = length - pos;
            bytes -= bytes % elementSize;
            elementCount = bytes / elementSize;
        }
        else if (bytes < 0) 
        {
            bytes = 0;
            elementCount = 0;
        }
        
        memcpy(data, buf->GetPtr() + pos, bytes);
        pos += bytes;

        return elementCount;
    }

    void BufferStream::Write(const void* data, int elementSize, int elementCount)
    {
        if (!write) 
            XLI_THROW_STREAM_CANT_READ;

        if (closed) 
            XLI_THROW_STREAM_CLOSED;

        int bytes = elementCount * elementSize;
        int length = buf->GetSizeInBytes();

        if (bytes > length - pos)
        {
            bytes = length - pos;
            bytes -= bytes % elementSize;
            elementCount = bytes / elementSize;
        }
        else if (bytes < 0) 
        {
            bytes = 0;
            elementCount = 0;
        }
        
        memcpy(const_cast<UInt8*>(buf->GetPtr()) + pos, data, bytes);
        pos += bytes;
    }

    void BufferStream::Seek(int offset, SeekOrigin origin)
    {
        if (closed) 
            XLI_THROW_STREAM_CLOSED;

        int len = buf->GetSizeInBytes();

        switch (origin)
        {
        case SeekOriginBegin:
            pos = offset;
            break;

        case SeekOriginCurrent:
            pos += offset;
            break;

        case SeekOriginEnd:
            pos = len + offset;
            break;
        }

        if (pos < 0) 
            pos = 0;
        else if (pos > len)
            pos = len;
    }

	DataAccessor* BufferStream::CreateDataAccessor()
	{
		return this->buf;
	}
}
