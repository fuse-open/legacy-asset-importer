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

#ifndef __XLI_ARRAY_H__
#define __XLI_ARRAY_H__

#include <Xli/Config.h>
#include <Xli/Object.h>
#include <Xli/Exception.h>
#include <Xli/Sort.h>

namespace Xli
{
    /**
        \ingroup XliContainers
    */
    template <typename T> class Array
    {
        static const int BufSize = 4;

        T* data;
        T buf[BufSize];
        int used;
        int capacity;

        Array(const Array& copy);
        Array& operator = (const Array& copy);

    public:
        Array()
        {
            used = 0;
            capacity = BufSize;
            data = buf;
        }

        explicit Array(int size)
        {
            used = 0;
            capacity = BufSize;
            data = buf;
            Resize(size);
        }

        explicit Array(int count, const T* initItems)
        {
            used = 0;
            capacity = BufSize;
            data = buf;
            Resize(count);

            for (int i = 0; i < count; i++) 
                data[i] = initItems[i];
        }

        ~Array()
        {
            if (data != buf) 
                delete [] data;
        }

        void Reserve(int newCapacity)
        {
            if (newCapacity > capacity)
            {
                T* newData = new T[newCapacity];
                
                for (int i = 0; i < used; i++) 
                    newData[i] = data[i];
                
                if (data != buf) 
                    delete [] data;
                
                data = newData;
                capacity = newCapacity;
            }
        }

        void Trim()
        {
            if (data != buf)
            {
                if (used <= BufSize)
                {
                    for (int i = 0; i < used; i++)
                        buf[i] = data[i];

                    delete [] data;
                    data = buf;
                    capacity = BufSize;
                }
                else if (used < capacity)
                {
                    T* newData = new T[used];

                    for (int i = 0; i < used; i++)
                        newData[i] = data[i];

                    delete [] data;
                    data = newData;
                    capacity = used;
                }
            }
        }

        void Resize(int newSize)
        {
            Reserve(newSize);
            used = newSize;
        }

        void Expand()
        {
            if (used > capacity / 2) 
                Reserve(capacity * 2);
        }

        int Length() const
        {
            return used;
        }

        int FirstIndex() const
        {
            return 0;
        }

        int LastIndex() const
        {
            return used - 1;
        }

        int Add()
        {
            if (capacity <= used) 
                Reserve(capacity * 2);
            
            return used++;
        }

        int Add(const T& item)
        {
            if (capacity <= used) 
                Reserve(capacity * 2);

            data[used] = item;
            return used++;
        }

        int AddRange(const T* items, int count)
        {
            int res = used;
            
            for (int i = 0; i < count; i++) 
                Add(items[i]);
            
            return res;
        }

        int AddRange(const Array<T>& values)
        {
            return AddRange(values.data, values.used);
        }

        int Insert(int index, const T& item)
        {
#ifdef XLI_RANGE_CHECK
            if (index > used || index < 0)
                XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif
            Add(item);

            for (int i = used - 1; i > index; i--) 
                data[i] = data[i - 1];

            data[index] = item;
            return index;
        }

        int IndexOf(const T& value) const
        {
            for (int i = 0; i < used; i++) 
                if (data[i] == value) 
                    return i;

            return -1;
        }

        int LastIndexOf(const T& elm) const
        {
            for (int i = used-1; i >= 0; i--) 
                if (data[i] == elm) 
                    return i;

            return -1;
        }

        bool Contains(const T& value) const
        {
            int i = IndexOf(value);
            return i != -1;
        }

        void RemoveAt(int index)
        {
#ifdef XLI_RANGE_CHECK
            if (index >= used || index < 0)
                XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif
            for (int i = index; i < used - 1; i++) 
                data[i] = data[i + 1];

            used--;
        }

        bool Remove(const T& item)
        {
            int i = IndexOf(item);

            if (i != -1)
            {
                RemoveAt(i);
                return true;
            }

            return false;
        }

        void RemoveAll(const T& item)
        {
            while (Remove(item));
        }

        void SwapRemoveAt(int i)
        {
            Swap(Get(i), Last());
            used--;
        }

        void RemoveRange(int start, int count)
        {
#ifdef XLI_RANGE_CHECK
            if (start >= used || start < 0)
                XLI_THROW_INDEX_OUT_OF_BOUNDS;

            if (start + count > used || count < 0)
                XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif

            for (int i = start; i < used - count; i++) 
                data[i] = data[i + count];

            used -= count;
        }

        T RemoveFirst()
        {
            T temp = First();
            RemoveAt(0);
            return temp;
        }

        T RemoveLast()
        {
            T temp = Last();
            RemoveAt(used-1);
            return temp;
        }

        T& Get(int index)
        {
#ifdef XLI_RANGE_CHECK
            if (index >= used || index < 0)
                XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif

            return data[index];
        }

        const T& Get(int index) const
        {
#ifdef XLI_RANGE_CHECK
            if (index >= used || index < 0)
                XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif

            return data[index];
        }

        void Clear()
        {
            used = 0;
        }

        T& operator [] (int index)
        {
            return Get(index);
        }

        const T& operator [] (int index) const
        {
            return Get(index);
        }

        T* Ptr()
        {
            return data;
        }

        const T* Ptr() const
        {
            return data;
        }

        T& First()
        {
            return Get(0);
        }

        const T& First() const
        {
            return Get(0);
        }

        T& Last()
        {
            return Get(used - 1);
        }

        const T& Last() const
        {
            return Get(used - 1);
        }

        void Reverse()
        {
            for (int a = 0, b = used; a < --b; a++)
                Swap(data[a], data[b]);
        }

        template <typename TComparator> 
        void Sort()
        {
            Xli::Sort<T, TComparator>(data, 0, used);
        }

        void Sort()
        {
            Sort<ComparatorLessThan<T> >();
        }

        void SortReversed()
        {
            Sort<ComparatorGreaterThan<T> >();
        }
    };

    /**
        \ingroup XliContainers
    */
    template <typename T> class ArrayRef: public Object
    {
    public:
        Array<T> Data;
    };
}


#endif
