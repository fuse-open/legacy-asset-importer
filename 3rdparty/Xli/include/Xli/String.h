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

#ifndef __XLI_STRING_DECL_H__
#define __XLI_STRING_DECL_H__

#include <Xli/IntTypes.h>

#ifdef XLI_PLATFORM_ANDROID 
# include <stdarg.h>
#else
# include <cstdarg>
#endif

namespace Xli
{
    /**
        \ingroup XliText
    */
    class String
    {
        friend class Unicode;
        static const int BufSize = 8;

        char* _data;
        char _buf[BufSize];
        int _length;

        void InitLength(int len);
        void Init(const char* str, int len);
        void Init(int i);
        void Init(float f);
        void Init(double d);
        void Deinit();

    public:
        String();
        String(const String& copy);
        String(const char* str);
        String(const char* str, int len);
        String(int i);
        String(float f);
        String(double d);
        ~String();

        char* CopyPtr();
        char* SwapPtr();

        char* Ptr();
        const char* Ptr() const;

        int Length() const;

        char& operator [] (int index);
        const char& operator [] (int index) const;

        const char& First() const;
        const char& Last() const;

        int IndexOf(char c, int start) const;
        int IndexOf(char c) const;

        int LastIndexOf(char c, int start) const;
        int LastIndexOf(char c) const;

        String Substring(int start, int len) const;
        String Substring(int start) const;

        String Trim() const;
        String Trim(char c) const;

        String ToLower() const;
        String ToUpper() const;

        void Split(char c, Array<String>& parts) const;
        static String Join(char c, const Array<String>& list);

        bool StartsWith(const String& str) const;
        bool EndsWith(const String& str) const;

        static String Create(int length);
        static String HexFromInt(int value);
        static String FromChar(char value);
        static String FromBool(bool value);
        
        static String Format(const char* format, va_list argList);
        static String Format(const char* format, ...);

        int HexToInt() const;
        int ToInt() const;
        float ToFloat() const;
        double ToDouble() const;
        bool ToBool() const;

        bool Equals(const char* str, int len) const;
        int CompareTo(const char* str, int len) const;
        String Add(const char* str, int len) const;
        void Append(const char* str, int len);
        
        bool operator == (const String& str) const;
        bool operator == (const char* str) const;

        bool operator != (const String& str) const;
        bool operator != (const char* str) const;

        String operator + (const String& str) const;
        String operator + (const char* str) const;

        bool operator < (const String& str) const;
        bool operator < (const char* str) const;

        bool operator <= (const String& str) const;
        bool operator <= (const char* str) const;

        bool operator > (const String& str) const;
        bool operator > (const char* str) const;

        bool operator >= (const String& str) const;
        bool operator >= (const char* str) const;

        String& operator += (const String& str);
        String& operator += (const char* str);
        
        String& operator = (const String& str);
        String& operator = (const char* str);
        String& operator = (int value);
        String& operator = (float value);
        String& operator = (double value);
    };
}

/**
    \addtogroup XliText
    @{
*/

Xli::String operator + (const char* a, const Xli::String& b);

/** @} */

#endif
