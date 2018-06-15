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
#include <cstring>

namespace Xli
{
    StringBuilder::StringBuilder(int cap) 
    { 
        buffer.Reserve(cap);
    }

    StringBuilder& StringBuilder::operator += (const String& text)
    {
        Append(text);
        return *this;
    }

    StringBuilder& StringBuilder::operator += (const char* str)
    {
        Append(str);
        return *this;
    }

    StringBuilder& StringBuilder::operator += (char c)
    {
        Append(c);
        return *this;
    }

    int StringBuilder::GetLength() const
    { 
        return buffer.Length(); 
    }

    void StringBuilder::Append(const char* str)
    {
        buffer.AddRange(str, (int)strlen(str));
    }

    void StringBuilder::Append(const String& text)
    {
        buffer.AddRange(text.Ptr(), text.Length());
    }

    void StringBuilder::Append(char c)
    {
        buffer.Add(c);
    }

    void StringBuilder::AppendLine(const char* str)
    {
        Append(str);
        buffer.Add('\n');
    }

    void StringBuilder::AppendLine(const String& text)
    {
        Append(text);
        buffer.Add('\n');
    }

    void StringBuilder::AppendLine()
    {
        buffer.Add('\n');
    }

    void StringBuilder::AppendFormat(const char* format, ...)
    {
        va_list argList;
        va_start(argList, format);
        Append(String::Format(format, argList));
        va_end(argList);
    }

    void StringBuilder::Clear()
    {
        buffer.Clear();
    }

    String StringBuilder::ToString() const
    {
        return String(buffer.Ptr(), buffer.Length());
    }
}
