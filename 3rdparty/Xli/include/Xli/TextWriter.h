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

#ifndef __XLI_TEXT_WRITER_H__
#define __XLI_TEXT_WRITER_H__

#include <Xli/Stream.h>
#include <Xli/String.h>
#include <Xli/File.h>
#include <Xli/Traits.h>

namespace Xli
{
    /**
        \ingroup XliIO
    */
    class TextWriter: public StreamWriterBase
    {
    public:
        TextWriter(Stream* stream);

        void Write(const char* str, int len);
        void WriteFormat(const char* format, ...);
        void Write(const char* str);
        void Write(const String& str);
        void WriteLine();

        template <typename T> 
        void Write(const T& t)
        {
            Write(Xli::Traits<T>::ToString(t));
        }

        template <typename T> 
        void WriteLine(const T& t)
        {
            Write(t);
            WriteLine();
        }
    };
}

#endif
