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

#include <Xli/StringBuilder.h>
#include <Xli/Uri.h>
#include <cstdio>
#include <cctype>
#include <cstring>

namespace Xli
{
    static const char* HexLits = "0123456789ABCDEF";
    static const char* DoNotEncode = "!*'();:@&=+$,/?#[]-_.~";

    bool ShouldEncode(int c, bool encodeParenthesis);
    String EncodeInternal(const String& uri, bool encodeParenthesis);

    String Uri::Encode(const String& uri)
    {
        return EncodeInternal(uri, true);
    }

    String Uri::AutoEncodeUri(const String& uri)
    {
        return EncodeInternal(uri, false);
    }

    String EncodeInternal(const String& uri, bool encodeParenthesis)
    {
        StringBuilder sb;

        for (int i = 0; i < uri.Length(); i++)
        {
            int c = (int)(unsigned char)uri[i];

            if (ShouldEncode(c, encodeParenthesis))
            {
                sb.Append('%');
                sb.Append(HexLits[(c >> 4) & 0xf]);
                sb.Append(HexLits[(c >> 0) & 0xf]);
            }
            else
            {
                sb.Append(uri[i]);
            }
        }

        return sb.ToString();
    }

    String Uri::Decode(const String& uri)
    {
        // TODO
        return uri;
    }

    bool ShouldEncode(int c, bool encodeParenthesis)
    {
        if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
            return false;

        for (int i = 0; i < std::strlen(DoNotEncode); ++i)
            if (c == (int)DoNotEncode[i])
                return false;

        if (!encodeParenthesis && c == '%')
            return false;

        return true;
    }
}
