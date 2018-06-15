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

#include <Xli/Buffer.h>
#include <cstdlib>
#include <cstring>

namespace Xli
{
    Buffer::Buffer(int size)
    {
        this->size = size;
        this->data = ((UInt8*)this) + sizeof(Buffer);
    }
    
    void* Buffer::operator new (size_t size, int bufSize)
    {
        return malloc(sizeof(Buffer) + bufSize);
    }

    void Buffer::operator delete (void* buf, int bufSize)
    {
        free(buf);
    }

    void Buffer::operator delete (void* buf)
    {
        free(buf);
    }
    
    Buffer* Buffer::Create(int size)
    {
        return new (size) Buffer(size);
    }

    void Buffer::Delete()
    {
        this->~Buffer();
        free(this);
    }

    Buffer* Buffer::CopyFrom(const void* data, int size)
    {
        Buffer* b = Buffer::Create(size);
        memcpy(b->Ptr(), data, size);
        return b;
    }

    int Buffer::Size() const
    {
        return size;
    }

    UInt8* Buffer::Ptr()
    {
        return data;
    }

    const UInt8* Buffer::Ptr() const
    {
        return data;
    }

    const UInt8* Buffer::GetPtr() const
    {
        return data;
    }

    int Buffer::GetSizeInBytes() const
    {
        return size;
    }

    BufferPtr::BufferPtr(void* data, int size, bool ownsData)
        : data((UInt8*)data), size(size), ownsData(ownsData) 
    {
    }

    BufferPtr::~BufferPtr() 
    { 
        if (ownsData) delete [] (UInt8*)data; 
    }

    int BufferPtr::Size() const
    {
        return size;
    }

    UInt8* BufferPtr::Ptr()
    {
        return data;
    }

    const UInt8* BufferPtr::Ptr() const
    {
        return data;
    }

    const UInt8* BufferPtr::GetPtr() const
    {
        return data;
    }

    int BufferPtr::GetSizeInBytes() const
    {
        return size;
    }

    BufferRef::BufferRef(void* data, int size, Object* owner)
        : data((UInt8*)data), size(size), owner(owner)
    {
        if (owner != 0) owner->AddRef();
    }

    BufferRef::~BufferRef()
    {
        if (owner != 0) owner->Release();
    }

    const UInt8* BufferRef::GetPtr() const
    {
        return data;
    }

    int BufferRef::GetSizeInBytes() const
    {
        return size;
    }
}
