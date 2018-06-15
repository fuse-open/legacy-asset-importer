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

#ifndef __XLI_SORT_H__
#define __XLI_SORT_H__

namespace Xli
{
    /**
        \ingroup XliUtils
    */
    template <typename T> struct ComparatorLessThan
    {
        static bool Compare(const T& a, const T& b)
        {
            return a < b;
        }
    };

    /**
        \ingroup XliUtils
    */
    template <typename T> struct ComparatorGreaterThan
    {
        static bool Compare(const T& a, const T& b)
        {
            return a > b;
        }
    };

    /**
        \addtogroup XliUtils
        @{
    */

    template <typename T> 
    void Swap(T& a, T& b)
    {
        T temp = a;
        a = b;
        b = temp;
    }

    template <typename T, typename TComparator> 
    void ShellSort(T* data, int left, int right)
    {
        const float s = 2.8f;

        int size = right - left;
        int increment = size / 2;

        while (increment > 0)
        {
            for (int i = left + increment; i < size; i++)
            {
                int j = i;
                T temp = data[i];

                while ((j >= increment) && TComparator::Compare(temp, data[j - increment]))
                {
                    Swap(data[j], data[j - increment]);
                    j -= increment;
                }

                data[j] = temp;
            }

            if (increment < s && increment > 1)
                increment = 1;
            else 
                increment = (int)((float)increment / s);
        }
    }

    template <typename T, typename TComparator> 
    void Sort(T* data, int left, int right)
    {
        if (right - left > 280)
        {
            int rightIndex = right - 1;
            int pivotIndex = left;

            T pivotValue = data[pivotIndex];
            Swap(data[pivotIndex], data[rightIndex]);

            for (int i = left; i < rightIndex; i++)
            {
                if (TComparator::Compare(data[i], pivotValue))
                {
                    Swap(data[i], data[pivotIndex]);
                    pivotIndex++;
                }
            }

            Swap(data[pivotIndex], data[rightIndex]);

            Sort<T, TComparator>(data, left, pivotIndex);
            Sort<T, TComparator>(data, pivotIndex + 1, right);
        }
        else
        {
            ShellSort<T, TComparator>(data, left, right);
        }
    }

    /** @} */
}

#endif
