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

#ifndef __XLI_UTF16_STRING_H__
#define __XLI_UTF16_STRING_H__

#include <Xli/String.h>

namespace Xli
{
    /**
        \addtogroup XliText
        @{
    */

    int Utf16StrLen(const Utf16* str);

    /** @} */

    /**
        \ingroup XliText
    */
    class Utf16String
    {
        friend class Unicode;
        
        Utf16* data;
        int length;

    public:
        Utf16String();
        Utf16String(const Utf16String& copy);
        Utf16String(const Utf16* str, int len);
        Utf16String(const Utf16* str);
        ~Utf16String();

        static Utf16String Create(int len);

        int Length() const;

        Utf16& operator[] (int index);
        const Utf16& operator[] (int index) const;

        Utf16* Ptr();
        const Utf16* Ptr() const;

        Utf16String& operator = (const Utf16String& copy);

        Utf16String operator + (const Utf16String& str) const;
    };
}

#endif
