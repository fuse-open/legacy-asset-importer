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

#include <Xli/Utf16String.h>
#include <Xli/Exception.h>
#include <cstring>

namespace Xli
{
    int Utf16StrLen(const Utf16* str)
    {
        int len = 0;
        
        while (*str++) 
            len++;
        
        return len;
    }

    Utf16String::Utf16String()
    {
        data = 0;
        length = 0;
    }

    Utf16String::Utf16String(const Utf16String& copy)
    {
        this->length = copy.length;
        this->data = new Utf16[copy.length + 1];
        this->data[copy.length] = 0;
        memcpy(this->data, copy.data, copy.length * 2);
    }

    Utf16String& Utf16String::operator = (const Utf16String& copy)
    {
        if (this->data != 0) delete [] this->data;
        this->length = copy.length;
        this->data = new Utf16[copy.length + 1];
        this->data[copy.length] = 0;
        memcpy(this->data, copy.data, copy.length * 2);
        return *this;
    }

    Utf16String::Utf16String(const Utf16* str, int len)
    {
        this->length = len;
        this->data = new Utf16[len + 1];
        this->data[len] = 0;
        memcpy(this->data, str, len * 2);
    }

    Utf16String::Utf16String(const Utf16* str)
    {
        this->length = Utf16StrLen(str);
        this->data = new Utf16[this->length + 1];
        this->data[this->length] = 0;
        memcpy(this->data, str, this->length * 2);
    }

    Utf16String::~Utf16String()
    {
        if (data != 0) delete [] data;
    }

    Utf16String Utf16String::Create(int len)
    {
        Utf16String r;
        r.length = len;
        r.data = new Utf16[len + 1];
        r.data[len] = 0;
        return r;
    }
    
    int Utf16String::Length() const
    {
        return length;
    }

    Utf16& Utf16String::operator[] (int index)
    {
        return data[index];
    }
    
    const Utf16& Utf16String::operator[] (int index) const
    {
        return data[index];
    }

    Utf16* Utf16String::Ptr()
    {
        return data;
    }
    
    const Utf16* Utf16String::Ptr() const
    {
        return data;
    }

    Utf16String Utf16String::operator + (const Utf16String& str) const
    {
        Utf16String r = Utf16String::Create(length + str.length);
        memcpy(r.data, data, length * sizeof(Utf16));
        memcpy(r.data + length, str.data, str.length * sizeof(Utf16));
        return r;
    }
}
