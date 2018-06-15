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

#include <Xli/String.h>
#include <Xli/Array.h>
#include <Xli/Exception.h>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifndef XLI_COMPILER_MSVC
# define sprintf_s snprintf
# define sscanf_s sscanf
# define vsnprintf_s(buf, bufSize, maxCount, format, argList) vsnprintf(buf, bufSize, format, argList)
#endif

namespace Xli
{
    static int GetFloatLength(const char* str, int len)
    {
        while (len > 1 && str[len - 1] == '0')
            len--;

        if (len > 1 && str[len - 1] == '.')
            len--;

        return len;
    }

    void String::InitLength(int len)
    {
        _data = len < BufSize - 1 ? _buf : (char *)malloc(len + 1);
        _data[len] = 0;
        _length = len;
    }

    void String::Init(const char* str, int len)
    {
        InitLength(len);
        memcpy(_data, str, len);
    }

    void String::Init(int i)
    {
        char buf[32];
        int len = sprintf_s(buf, 32, "%d", i);
        Init(buf, len);
    }

    void String::Init(float f)
    {
        char buf[64];
        int len = sprintf_s(buf, 64, "%f", f);
        Init(buf, GetFloatLength(buf, len));
    }

    void String::Init(double d)
    {
        char buf[128];
        int len = sprintf_s(buf, 128, "%lf", d);
        Init(buf, GetFloatLength(buf, len));
    }

    void String::Deinit()
    {
        if (_data != _buf)
            free(_data);
    }

    String::String()
    {
        _length = 0;
        _data = _buf;
        _buf[0] = 0;
    }

    String::String(const String& str)
    {
        Init(str._data, str._length);
    }

    String::String(const char* str)
    {
        Init(str, !str ? 0 : (int)strlen(str));
    }

    String::String(const char* str, int len)
    {
        Init(str, len);
    }

    String::String(int i)
    {
        Init(i);
    }

    String::String(float f)
    {
        Init(f);
    }

    String::String(double d)
    {
        Init(d);
    }

    String::~String()
    {
        Deinit();
    }

    char* String::CopyPtr()
    {
        char* buf = (char *)malloc(_length + 1);
        buf[_length] = 0;
        memcpy(buf, _data, _length);
        return buf;
    }

    char* String::SwapPtr()
    {
        if (_data == _buf)
            return CopyPtr();

        char *result = _data;

        _buf[0] = '\0';
        _length = 0;
        _data = _buf;

        return result;
    }

    char* String::Ptr()
    {
        return _data;
    }

    const char* String::Ptr() const
    {
        return _data;
    }

    int String::Length() const
    {
        return _length;
    }

    char& String::operator [] (int index)
    {
#ifdef XLI_RANGE_CHECK
        if (index >= _length)
            XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif

        return _data[index];
    }

    const char& String::operator [] (int index) const
    {
#ifdef XLI_RANGE_CHECK
        if (index >= _length)
            XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif

        return _data[index];
    }

    const char& String::First() const
    {
        return (*this)[0];
    }

    const char& String::Last() const
    {
        return (*this)[_length - 1];
    }

    int String::IndexOf(char c, int start) const
    {
        for (int i = start; i < _length; i++)
            if (_data[i] == c) 
                return i;

        return -1;
    }

    int String::IndexOf(char c) const
    {
        return IndexOf(c, 0);
    }

    int String::LastIndexOf(char c, int start) const
    {
#ifdef XLI_RANGE_CHECK
        if (start >= _length)
            XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif

        for (int i = start; i >= 0; i--)
            if (_data[i] == c) 
                return i;

        return -1;
    }

    int String::LastIndexOf(char c) const
    {
        return LastIndexOf(c, _length - 1);
    }

    String String::Substring(int start, int len) const
    {
#ifdef XLI_RANGE_CHECK
        if (start + len > _length || start < 0)
            XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif

        return String(_data + start, len);
    }

    String String::Substring(int start) const 
    { 
        return Substring(start, _length - start); 
    }

    String String::Trim() const
    {
        int start = 0;
        int end = _length;

        while (start < end) 
            if (isspace((int)(unsigned char)_data[start])) 
                start++; 
            else 
                break;

        while (end > start) 
            if (isspace((int)(unsigned char)_data[end - 1])) 
                end--; 
            else 
                break;

        return Substring(start, end - start);
    }
        
    String String::Trim(char c) const
    {
        int start = 0;
        int end = _length;

        while (start < end) 
            if (_data[start] == c) 
                start++; 
            else 
                break;

        while (end > start) 
            if (_data[end - 1] == c) 
                end--; 
            else 
                break;

        return Substring(start, end - start);
    }

    String String::ToLower() const
    {
        String r(_data, _length);

        for (int i = 0; i < _length; i++) 
            r._data[i] = (char)(unsigned char)tolower((int)(unsigned char)r._data[i]);

        return r;
    }

    String String::ToUpper() const
    {
        String r(_data, _length);

        for (int i = 0; i < _length; i++) 
            r._data[i] = (char)(unsigned char)toupper((int)(unsigned char)r._data[i]);

        return r;
    }

    void String::Split(char c, Array<String>& parts) const
    {
        int start = 0;

        for (int i = 0; i < _length; i++)
        {
            if (_data[i] == c)
            {
                parts.Add(Substring(start, i - start));
                start = i+1;
            }
        }

        if (start < _length)
            parts.Add(Substring(start, _length - start));
    }

    String String::Join(char c, const Array<String>& list)
    {
        String r;
        r._length = -1;

        for (int i = 0; i < list.Length(); i++) 
            r._length += list[i].Length() + 1;

        if (r._length == -1) 
            return String();

        r._data = (char *)malloc(r._length + 1);
        int p = 0;

        for (int i = 0; i < list.Length(); i++)
        {
            memcpy(r._data + p, list[i].Ptr(), list[i].Length());
            p += list[i].Length();
            r._data[p++] = c;
        }

        r._data[r._length] = 0;
        return r;
    }

    bool String::StartsWith(const String& str) const
    {
        if (str._length > _length) 
            return false;

        for (int i = 0; i < str._length; i++)
            if (str._data[i] != _data[i]) 
                return false;

        return true;
    }

    bool String::EndsWith(const String& str) const
    {
        if (str._length > _length)
            return false;

        for (int i = _length - str._length; i < _length; i++)
            if (str._data[i - _length + str._length] != _data[i]) 
                return false;

        return true;
    }

    String String::Create(int _length)
    {
        String r;
        r.InitLength(_length);
        return r;
    }

    String String::HexFromInt(int h)
    {
        char buf[32];
        sprintf_s(buf, 32, "0x%x", h);
        return buf;
    }

    String String::FromChar(char c)
    {
        return String(&c, 1);
    }

    String String::FromBool(bool b)
    {
        return b ? "True" : "False";
    }

    String String::Format(const char* format, va_list argList)
    {
        // TODO: Verify code

        const int BufferSize = 2048;

        char buf[BufferSize];
        int i = vsnprintf_s(buf, BufferSize, _TRUNCATE, format, argList);

        if (i >= BufferSize || i < 0) 
            XLI_THROW_INVALID_FORMAT("Result too long");

        return String(buf, i);
    }

    String String::Format(const char* format, ...)
    {
        va_list argList;
        va_start(argList, format);
        String result = Format(format, argList);
        va_end(argList);
        return result;
    }

    int String::HexToInt() const
    {
        int i;
        if (sscanf_s(_data, "%x", &i) == 1) return i;
        if (sscanf_s(_data, "%X", &i) == 1) return i;
        XLI_THROW_INVALID_FORMAT("Unable to convert string(hex) to integer");
    }

    int String::ToInt() const
    {
        int i;
        if (sscanf_s(_data, "%d", &i) == 1) return i;
        XLI_THROW_INVALID_FORMAT("Unable to convert string to int");
    }

    float String::ToFloat() const
    {
        float i;
        if (sscanf_s(_data, "%f", &i) == 1) return i;
        if (sscanf_s(_data, "%e", &i) == 1) return i;
        if (sscanf_s(_data, "%E", &i) == 1) return i;
        XLI_THROW_INVALID_FORMAT("Unable to convert string to float");
    }

    double String::ToDouble() const
    {
        double i;
        if (sscanf_s(_data, "%lf", &i) == 1) return i;
        if (sscanf_s(_data, "%le", &i) == 1) return i;
        if (sscanf_s(_data, "%lE", &i) == 1) return i;
        XLI_THROW_INVALID_FORMAT("Unable to convert string to double");
    }

    bool String::ToBool() const
    {
        if (!strcmp(_data, "true")) return true;
        if (!strcmp(_data, "false")) return false;
        if (!strcmp(_data, "TRUE")) return true;
        if (!strcmp(_data, "FALSE")) return false;
        if (!strcmp(_data, "True")) return true;
        if (!strcmp(_data, "False")) return false;
        XLI_THROW_INVALID_FORMAT("Unable to convert string to bool");
    }

    bool String::Equals(const char* str, int len) const
    {
        if (_length != len)
            return false;

        for (int i = 0; i < _length; i++)
            if (_data[i] != str[i]) 
                return false;

        return true;
    }

    int String::CompareTo(const char* str, int len) const
    {
        int l = _length;
        if (len < l) l = len;

        for (int i = 0; i < l; i++)
        {
            if (_data[i] == str[i]) 
                continue;
            else if (_data[i] < str[i]) 
                return -1;

            return 1;
        }

        if (_length == len) 
            return 0;
        else if (_length < len) 
            return -1;
        
        return 1;
    }

    String String::Add(const char* str, int len) const
    {
        String r;
        r.InitLength(_length + len);
        memcpy(r._data, _data, _length);
        memcpy(r._data + _length, str, len);
        return r;
    }

    void String::Append(const char* str, int len)
    {
        if (!len) 
            return;
        
        String temp = Add(str, len);

        if (temp._data != temp._buf)
        {    
            // Hack:
            // 1. Swap _data with temp 
            // 2. Let temp free old _data when it goes out of scope, as long as old _data does not point to buf which is stack allocated
            char* newData = temp._data;
            temp._data = _data != _buf ? _data : temp._buf;
            _data = newData;
            _length = temp._length;
        }
        else
        {
            // If hack fails we must reallocate the _data
            Deinit();
            Init(temp._data, temp._length);
        }
    }
    
    bool String::operator == (const String& str) const
    {
        return Equals(str._data, str._length);
    }

    bool String::operator == (const char* str) const
    {
        return Equals(str, !str ? 0 : (int)strlen(str));
    }

    bool String::operator != (const String& str) const
    {
        return !Equals(str._data, str._length);
    }

    bool String::operator != (const char* str) const
    {
        return !Equals(str, !str ? 0 : (int)strlen(str));
    }

    String String::operator + (const String& str) const
    {
        return Add(str._data, str._length);
    }

    String String::operator + (const char* str) const
    {
        return Add(str, !str ? 0 : (int)strlen(str));
    }

    bool String::operator < (const String& str) const
    {
        return CompareTo(str._data, str._length) < 0;
    }

    bool String::operator < (const char* str) const
    {
        return CompareTo(str, !str ? 0 : (int)strlen(str)) < 0;
    }

    bool String::operator <= (const String& str) const
    {
        return CompareTo(str._data, str._length) <= 0;
    }

    bool String::operator <= (const char* str) const
    {
        return CompareTo(str, !str ? 0 : (int)strlen(str)) <= 0;
    }

    bool String::operator > (const String& str) const
    {
        return CompareTo(str._data, str._length) > 0;
    }

    bool String::operator > (const char* str) const
    {
        return CompareTo(str, !str ? 0 : (int)strlen(str)) > 0;
    }

    bool String::operator >= (const String& str) const
    {
        return CompareTo(str._data, str._length) >= 0;
    }

    bool String::operator >= (const char* str) const
    {
        return CompareTo(str, !str ? 0 : (int)strlen(str)) >= 0;
    }

    String& String::operator += (const String& str)
    {
        Append(str._data, str._length);
        return *this;
    }
    
    String& String::operator += (const char* str)
    {
        Append(str, !str ? 0 : (int)strlen(str));
        return *this;
    }
    
    String& String::operator = (const String& str)
    {
        if (this == &str) 
            return *this;

        Deinit();
        Init(str._data, str._length);
        return *this;
    }

    String& String::operator = (const char* str)
    {
        if (this->_data == str) 
            return *this;

        Deinit();
        Init(str, !str ? 0 : (int)strlen(str));
        return *this;
    }

    String& String::operator = (int i)
    {
        Deinit();
        Init(i);
        return *this;
    }

    String& String::operator = (float f)
    {
        Deinit();
        Init(f);
        return *this;
    }

    String& String::operator = (double d)
    {
        Deinit();
        Init(d);
        return *this;
    }
}

Xli::String operator + (const char* a, const Xli::String& b)
{
    int len = !a ? 0 : (int)strlen(a);
    Xli::String r = Xli::String::Create(len + b.Length());
    memcpy(r.Ptr(), a, len);
    memcpy(r.Ptr() + len, b.Ptr(), b.Length());
    return r;
}
