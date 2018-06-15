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

#ifndef __XLI_STRING_BUILDER_H__
#define __XLI_STRING_BUILDER_H__

#include <Xli/String.h>
#include <Xli/Array.h>

namespace Xli
{
    /**
        \ingroup XliText
    */
    class StringBuilder
    {
        Array<char> buffer;

    public:
        StringBuilder(int cap = 1024);

        StringBuilder& operator += (const String& text);
        StringBuilder& operator += (const char* str);
        StringBuilder& operator += (char c);

        int GetLength() const;

        void Append(const char* str);
        void Append(const String& text);
        void Append(char c);

        void AppendLine(const char* str);
        void AppendLine(const String& text);
        void AppendLine();

        void AppendFormat(const char* format, ...);

        void Clear();
        
        String ToString() const;
    };
}

#endif
