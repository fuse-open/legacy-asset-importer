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

#include <Xli/DateTime.h>
#include <Xli/Time.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        Timestamp ConvertToTimestamp(const tm* const time, const suseconds_t useconds = 0)
        {
            DateTime dt;
            dt.Year = time->tm_year + 1900;
            dt.Month = time->tm_mon + 1;
            dt.Day = time->tm_mday;
            dt.Hour = time->tm_hour;
            dt.Minute = time->tm_min;
            dt.Second = time->tm_sec;
            dt.Millisecond = useconds / 1000;
            return dt.ToTimestamp();
        }

    }

    double GetSeconds()
    {
        struct timeval t;
        gettimeofday(&t, 0);
        return (double)t.tv_sec + (double)t.tv_usec * (1.0 / 1000000.0);
    }

    Timestamp GetTimestamp()
    {
        timeval t;
        gettimeofday(&t, NULL);
        return PlatformSpecific::ConvertToTimestamp(localtime(&t.tv_sec), t.tv_usec);
    }

    Timestamp GetTimestampUtc()
    {
        timeval t;
        gettimeofday(&t, NULL);
        return PlatformSpecific::ConvertToTimestamp(gmtime(&t.tv_sec), t.tv_usec);
    }
}
