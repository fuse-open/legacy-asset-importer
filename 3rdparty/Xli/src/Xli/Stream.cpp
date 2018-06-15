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

#include <Xli/Stream.h>
#include <Xli/Console.h>
#include <Xli/File.h>

namespace Xli
{
    Stream::~Stream() 
    {
    }

    void Stream::Flush() 
    {
    }
    
    void Stream::Close() 
    {
    }

    bool Stream::AtEnd() const 
    { 
        return IsClosed() || (CanSeek() ? GetPosition() == GetLength() : false);
    }
    
    bool Stream::IsClosed() const 
    { 
        return false; 
    }

    bool Stream::CanRead() const 
    {
        return false; 
    }
    
    bool Stream::CanWrite() const 
    { 
        return false; 
    }
    
    bool Stream::CanSeek() const 
    { 
        return false; 
    }

    int Stream::Read(void* dst, int elmSize, int elmCount) 
    { 
        XLI_THROW_STREAM_CANT_READ; 
    }
    
    void Stream::Write(const void* src, int elmSize, int elmCount) 
    { 
        XLI_THROW_STREAM_CANT_WRITE;
    }

    void Stream::Seek(int offset, SeekOrigin origin) 
    { 
        XLI_THROW_STREAM_CANT_SEEK; 
    }
    
    int Stream::GetPosition() const 
    { 
        XLI_THROW_STREAM_CANT_SEEK; 
    }
    
    int Stream::GetLength() const 
    { 
        XLI_THROW_STREAM_CANT_SEEK; 
    }

    void Stream::ReadSafe(void* dst, int elmSize, int elmCount)
    {
        int r = Read(dst, elmSize, elmCount);

        if (r != elmCount)
        {
            if (AtEnd()) 
                XLI_THROW_END_OF_STREAM;
            
            XLI_THROW_STREAM_CANT_READ;
        }
    }

    void Stream::WriteStream(Stream* source)
    {
        // TODO: Use constant size buffer
        Managed<Buffer> buf = Buffer::Create(source->GetLength());
        source->Read(buf->Ptr(), 1, buf->Size());
        Write(buf->Ptr(), 1, buf->Size());
    }

    StreamWriterBase::StreamWriterBase(Stream* stream)
    {
        _stream = 0;
        SetStream(stream);
    }

    StreamWriterBase::~StreamWriterBase()
    {
        _stream->Release();
    }

    void StreamWriterBase::SetStream(Stream* stream)
    {
        if (!stream)
            XLI_THROW_NULL_POINTER;

        if (!stream->CanWrite())
            XLI_THROW_STREAM_CANT_WRITE;

        stream->AddRef();
        
        if (_stream) 
            _stream->Release();

        _stream = stream;
    }

    Stream* StreamWriterBase::GetStream()
    {
        return _stream;
    }

    StreamReaderBase::StreamReaderBase(Stream* stream)
    {
        _stream = 0;
        SetStream(stream);
    }

    StreamReaderBase::~StreamReaderBase()
    {
        _stream->Release();
    }

    void StreamReaderBase::SetStream(Stream* stream)
    {
        if (!stream)
            XLI_THROW_NULL_POINTER;

        if (!stream->CanRead())
            XLI_THROW_STREAM_CANT_READ;

        stream->AddRef();
        
        if (_stream) 
            _stream->Release();
        
        _stream = stream;
    }

    Stream* StreamReaderBase::GetStream()
    {
        return _stream;
    }
}
