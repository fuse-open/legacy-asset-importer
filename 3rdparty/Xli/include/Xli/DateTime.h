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

#ifndef __XLI_DATE_TIME_H__
#define __XLI_DATE_TIME_H__

#include <Xli/IntTypes.h>
#include <Xli/String.h>

namespace Xli
{
    /**
        \ingroup XliTime
    */
    class DateTime
    {
    public:
        static const UInt64 PerMillisecond = 10000;
        static const UInt64 PerSecond = PerMillisecond * 1000;
        static const UInt64 PerMinute = PerSecond * 60;
        static const UInt64 PerHour = PerMinute * 60;
        static const UInt64 PerDay = PerHour * 24;

        static bool IsLeapYear(int year);
        static int GetDaysInYear(int year);
        static int GetDaysInMonth(int month, int year);

        int Year;
        int Month;
        int Day;
        int Hour;
        int Minute;
        int Second;
        int Millisecond;

        DateTime();
        DateTime(Timestamp timestamp);

        static DateTime Now();
        static DateTime NowUtc();
        static DateTime FromString(const String& str);

        Timestamp ToTimestamp() const;
        String ToString() const;

        DateTime operator - (const DateTime& dt) const;
        DateTime operator + (const DateTime& dt) const;
        
        DateTime& operator = (const DateTime& dt);
    };

}

#endif
