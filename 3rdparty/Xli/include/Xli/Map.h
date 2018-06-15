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

#ifndef __XLI_MAP_H__
#define __XLI_MAP_H__

#include <Xli/Array.h>

namespace Xli
{
    /**
        \ingroup XliContainers

        Map which preserves the order of the keys. This implementation has slower look up than HashMap and is not meant to be used in performance critical code.
    */
    template <typename TKey, typename TValue> class Map
    {
        Array<TKey> keys;
        Array<TValue> values;

    public:
        /**
            Gives an integer iterator to the beginning of the map.
        */
        int Begin() const
        {
            return 0;
        }

        /**
            Returns the iterator value that indicates the end of the map.
        */
        int End() const
        {
            return keys.Length();
        }

        /**
            Advances an iterator to the next element.
            @see Begin
            @param iterator The current iterator
            @return An iterator pointing to the succeeding element, or End().
        */
        int Next(int iterator) const
        {
            return iterator + 1;
        }

        /**
            Returns the value at the given iterator
            Use Begin(), Next() and End() to iterate through the map.
            @param iterator The iterator at which to look up the value
            @return The value at the given iterator.
        */
        const TValue& GetValue(int iterator) const
        {
            return values[iterator];
        }

        /**
            Returns the value at the given iterator
            Use Begin(), Next() and End() to iterate through the map.
            @param iterator The iterator at which to look up the value
            @return The value at the given iterator.
        */
        TValue& GetValue(int iterator)
        {
            return values[iterator];
        }

        /**
            Sets a new value at the given iterator
            Use Begin(), Next() and End() to iterate through the map.
            @param iterator The iterator at which to look up the value
            @param value The new value to be set at the given iterator.
        */
        void SetValue(int iterator, const TValue& value)
        {
            values[iterator] = value;
        }

        /**
            Returns the key at the given iterator
            Use Begin(), Next() and End() to iterate through the map.
            @param iterator The iterator at which to look up the key
            @return The key at the given iterator.
        */
        const TKey& GetKey(int iterator) const
        {
            return keys[iterator];
        }

        void Clear()
        {
            keys.Clear();
            values.Clear();
        }

        TValue& operator [] (const TKey& key)
        {
            int i = keys.IndexOf(key);
            
            if (i == -1) 
                XLI_THROW("Map does not contain the given key");
            
            return values[i];
        }

        void Add(const TKey& key, const TValue& value)
        {
            if (keys.Contains(key))
                XLI_THROW("Map already contains the given key");

            keys.Add(key);
            values.Add(value);
        }

        void AddRange(const Map& map)
        {
            for (int it = map.Begin(); it != map.End(); it = map.Next(it))
                Add(map.GetKey(it), map.GetValue(it));
        }

        bool Remove(const TKey& key)
        {
            int i = keys.IndexOf(key);
            
            if (i == -1) 
                return false;
            
            TValue value = values[i];
            keys.RemoveAt(i);
            values.RemoveAt(i);
            return true;
        }

        bool ContainsKey(const TKey& key) const
        {
            return keys.Contains(key);
        }

        bool TryGetValue(const TKey& key, TValue& value) const
        {
            int i = keys.IndexOf(key);
            
            if (i == -1) 
                return false;
            
            value = values[i];
            return true;
        }

        int Count() const
        {
            return keys.Length();
        }
    };
}

#endif
