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

#include <Xli/Traits.h>
#include <cstdio>

#ifndef XLI_COMPILER_MSVC
# define sprintf_s snprintf
#endif

namespace Xli
{
    UInt32 DefaultTraits::Hash(const UInt8* data, int size)
    {
        // djb2
        UInt32 hash = 5381;

        for (int i = 0; i < size; i++)
            hash = ((hash << 5) + hash) ^ (UInt32)data[i];

        return hash;
    }

    UInt32 DefaultTraits::Hash(bool b)
    {
        return b ? 1 : 0; 
    }
    
    UInt32 DefaultTraits::Hash(const void* p)
    { 
        return DefaultTraits::Hash((UInt8*)&p, sizeof(void*));
    }
    
    UInt32 DefaultTraits::Hash(float f)
    { 
        return *(UInt32*)&f; 
    }
    
    UInt32 DefaultTraits::Hash(double d)
    { 
        UInt32 hash = 27;
        hash = (13 * hash) + ((UInt32*)&d)[0];
        hash = (13 * hash) + ((UInt32*)&d)[1];
        return hash;
    }

    UInt32 DefaultTraits::Hash(Int8 i)
    {
        return (UInt32)i; 
    }
    
    UInt32 DefaultTraits::Hash(UInt8 i)
    {
        return (UInt32)i; 
    }
    
    UInt32 DefaultTraits::Hash(Int16 i)
    {
        return (UInt32)i; 
    }
    
    UInt32 DefaultTraits::Hash(UInt16 i)
    {
        return (UInt32)i; 
    }
    
    UInt32 DefaultTraits::Hash(Int32 i)
    {
        return (UInt32)i; 
    }
    
    UInt32 DefaultTraits::Hash(UInt32 i)
    {
        return i; 
    }
    
    UInt32 DefaultTraits::Hash(Int64 i)
    {
        UInt32 hash = 27;
        hash = (13 * hash) + ((UInt32*)&i)[0];
        hash = (13 * hash) + ((UInt32*)&i)[1];
        return hash;
    }
    
    UInt32 DefaultTraits::Hash(UInt64 i)
    {
        UInt32 hash = 27;
        hash = (13 * hash) + ((UInt32*)&i)[0];
        hash = (13 * hash) + ((UInt32*)&i)[1];
        return hash;
    }

    UInt32 DefaultTraits::Hash(const String& str)
    {
        return Hash((const UInt8*)str.Ptr(), str.Length());
    }

    String DefaultTraits::ToString(void* p)
    {
        char buf[64];
        int len = sprintf_s(buf, 64, "0x%llx", (unsigned long long int)p);
        return String(buf, len);
    }

    String DefaultTraits::ToString(char c)
    {
        return (int)c;
    }

    String DefaultTraits::ToString(unsigned char c)
    {
        return (int)c;
    }

    String DefaultTraits::ToString(short i)
    {
        char buf[64];
        int len = sprintf_s(buf, 64, "%hd", i);
        return String(buf, len);
    }

    String DefaultTraits::ToString(unsigned short i)
    {
        char buf[64];
        int len = sprintf_s(buf, 64, "%hu", i);
        return String(buf, len);
    }

    String DefaultTraits::ToString(int i)
    {
        return i;
    }

    String DefaultTraits::ToString(unsigned int i)
    {
        char buf[64];
        int len = sprintf_s(buf, 64, "%u", i);
        return String(buf, len);
    }

    String DefaultTraits::ToString(long i)
    {
        char buf[64];
        int len = sprintf_s(buf, 64, "%ld", i);
        return String(buf, len);
    }

    String DefaultTraits::ToString(unsigned long i)
    {
        char buf[64];
        int len = sprintf_s(buf, 64, "%lu", i);
        return String(buf, len);
    }

    String DefaultTraits::ToString(long long i)
    {
        char buf[64];
        int len = sprintf_s(buf, 64, "%lld", i);
        return String(buf, len);
    }

    String DefaultTraits::ToString(unsigned long long i)
    {
        char buf[64];
        int len = sprintf_s(buf, 64, "%llu", i);
        return String(buf, len);
    }

    String DefaultTraits::ToString(float f)
    {
        return f;
    }

    String DefaultTraits::ToString(double d)
    {
        return d;
    }

    String DefaultTraits::ToString(bool b)
    {
        return String::FromBool(b);
    }
}
