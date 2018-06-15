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

#include <XliZip/GZip.h>
#include <Xli/Console.h>
#include <Xli/BufferStream.h>
#include <Xli/Shared.h>
#include <cstring>
#include <zlib.h>

#define DEF_MEM_LEVEL 8
#define Z_BUFSIZE 0x4000

namespace Xli
{
    class GZipWriter: public Stream
    {
        Shared<Stream> dst;
        Bytef buf[Z_BUFSIZE];
        z_stream stream;
        int pos;

    public:
        GZipWriter(Stream* targetStream, int level, bool header)
        {
            if (!targetStream) 
                XLI_THROW_NULL_POINTER;

            this->pos = 0;
            this->dst = targetStream;

            memset(&stream, 0, sizeof(z_stream));

            int windowBits = header ? MAX_WBITS + 16 : -MAX_WBITS;

            if (deflateInit2(&stream, level, Z_DEFLATED, windowBits, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY) != Z_OK) 
                XLI_THROW("Failed to create GZip stream");

            stream.next_out = buf;
            stream.avail_out = Z_BUFSIZE;
        }

        virtual ~GZipWriter()
        {
            Close();
        }

        virtual void Flush()
        {
            if (!dst) 
                XLI_THROW_STREAM_CLOSED;

            // TODO: Flush gzip buffer

            dst->Flush();
        }

        virtual void Close()
        {
            if (!dst) 
                return;

            while (true)
            {
                int err = deflate(&stream, Z_FINISH);

                if (err == Z_STREAM_END)
                    break;
                else if (err != Z_OK || stream.avail_out != 0)
                    XLI_THROW("Failed to finish GZip writer");

                stream.next_out = buf;
                stream.avail_out = Z_BUFSIZE;
                dst->Write(buf, 1, Z_BUFSIZE);
            }
            
            if (deflateEnd(&stream) != Z_OK)
                XLI_THROW("Failed to close GZip writer");

            dst->Write(buf, 1, Z_BUFSIZE - stream.avail_out);
            dst = 0;
        }

        virtual bool IsClosed() const
        {
            return !dst;
        }

        virtual bool CanWrite() const
        {
            return true;
        }

        virtual int GetLength() const
        {
            return pos;
        }

        virtual void Write(const void* data, int elmSize, int elmCount)
        {
            if (!dst) 
                XLI_THROW_STREAM_CLOSED;

            int len = elmSize * elmCount;

            stream.next_in = (Bytef*)data;
            stream.avail_in = len;

            pos += len;

            do
            {
                if (deflate(&stream, Z_NO_FLUSH) != Z_OK)
                    XLI_THROW("Failed to write to GZip stream");

                if (!stream.avail_out)
                {
                    stream.next_out = buf;
                    stream.avail_out = Z_BUFSIZE;
                    dst->Write(buf, 1, Z_BUFSIZE);
                }
            } 
            while (stream.avail_in > 0);
        }
    };
/*
    class GZipReader: public Stream
    {

    };
*/    
    Stream* GZip::CreateWriter(Stream* targetStream, int level, bool header)
    {
        return new GZipWriter(targetStream, level, header);
    }
    
    Stream* GZip::CreateReader(Stream* sourceStream)
    {
        if (sourceStream == 0) 
            XLI_THROW_NULL_POINTER;

        int sp = sourceStream->GetPosition();
        int sl = sourceStream->GetLength();

        uLong expected_crc;
        int expected_len;

        sourceStream->Seek(-8, SeekOriginEnd);
        sourceStream->ReadSafe(&expected_crc, 4, 1);
        sourceStream->ReadSafe(&expected_len, 4, 1);
        sourceStream->Seek(sp, SeekOriginBegin);

        int input_size = sl - sp;

        Managed<Buffer> src = Buffer::Create(input_size);
        Managed<Buffer> dst = Buffer::Create(expected_len);

        sourceStream->ReadSafe(src->Ptr(), 1, src->Size());

        z_stream stream;
        memset(&stream, 0, sizeof(z_stream));

        stream.next_in = src->Ptr();
        stream.avail_in = src->Size();
        stream.next_out = dst->Ptr();
        stream.avail_out = dst->Size();

        int err = inflateInit2(&stream, 16 + MAX_WBITS);

        if (err != Z_OK) 
            XLI_THROW("Failed to create GZip stream");

        while (true)
        {
            err = inflate(&stream, Z_FINISH);

            if (err == Z_OK)
                continue;
            else if (err == Z_STREAM_END)
                break;
            else if (err == Z_DATA_ERROR)
                XLI_THROW("Failed to create GZip stream: Data error");
            else
                XLI_THROW("Failed to create GZip stream");
        }

        if (stream.avail_out != 0 || stream.avail_in != 0)
            Error->WriteLine("GZIP WARNING: Buffer sizes inconsistent");

        return new BufferStream(dst, true, false);
    }
}
