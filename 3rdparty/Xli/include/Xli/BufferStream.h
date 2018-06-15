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

#ifndef __XLI_BUFFER_STREAM_H__
#define __XLI_BUFFER_STREAM_H__

#include <Xli/Stream.h>

namespace Xli
{
    /**
        \ingroup XliIO
    */
    class BufferStream: public Stream
    {
        DataAccessor* buf;
        bool read, write, closed;
        int pos;

    public:
        BufferStream(DataAccessor* buf, bool canRead, bool canWrite);
        virtual ~BufferStream();

        virtual void Close();

        virtual bool IsClosed() const;
        virtual bool AtEnd() const;

        virtual bool CanRead() const;
        virtual bool CanWrite() const;
        virtual bool CanSeek() const;

        virtual int Read(void* dst, int elmSize, int elmCount);
        virtual void Write(const void* dst, int elmSize, int elmCount);
        virtual void Seek(int offset, SeekOrigin origin);

        virtual int GetPosition() const;
        virtual int GetLength() const;

		virtual DataAccessor* CreateDataAccessor();
    };
}

#endif
