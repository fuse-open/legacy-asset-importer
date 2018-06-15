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

#include "AInternal.h"
#include "ALogStream.h"

namespace Xli
{
    namespace PlatformSpecific
    {
        ALogStream::ALogStream(int prio)
        {
            this->prio = prio;
        }

        ALogStream::~ALogStream()
        {
            if (buf.Length())
            {
                buf.Add(0);
                __android_log_write(prio, AGetAppName(), buf.Ptr());
                buf.Clear();
            }
        }

        bool ALogStream::CanWrite() const
        {
            return true;
        }

        void ALogStream::Write(const void* src, int elmSize, int elmCount)
        {
            for (int i = 0; i < elmSize * elmCount; i++)
            {
                char c = ((const char*)src)[i];

                switch (c)
                {
                case '\n':
                    buf.Add(0);
                    __android_log_write(prio, AGetAppName(), buf.Ptr());
                    buf.Clear();
                    continue;

                case 0:
                    buf.Add((char)(unsigned char)0xC0);
                    buf.Add((char)(unsigned char)0x80);
                    continue;
                }

                buf.Add(c);
            }
        }
    }
}
