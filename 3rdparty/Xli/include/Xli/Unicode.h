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

#ifndef __XLI_UNICODE_H__
#define __XLI_UNICODE_H__

#include <Xli/String.h>
#include <Xli/Utf16String.h>

namespace Xli
{
    /**
        \ingroup XliText
    */
    enum UnicodeFlags
    {
        UnicodeFlagsModifiedUtf8 = 1 << 0,
        UnicodeFlagsIgnoreError = 1 << 1,
    };

    /**
        \ingroup XliText
    */
    class Unicode
    {
    public:
        static bool IsLegalUtf8(const char* str, int len, int flags = 0);
        static bool IsLegalUtf8(const String& str, int flags = 0);

        static Utf16String Utf8To16(const char* str, int len, int flags = 0);
        static Utf16String Utf8To16(const String& str, int flags = 0);

        static String Utf16To8(const Utf16* str, int len, int flags = 0);
        static String Utf16To8(const Utf16String& str, int flags = 0);

        static Utf16 ToUpper(Utf16 chr);
        static Utf16 ToLower(Utf16 chr);
    };
}

#endif
