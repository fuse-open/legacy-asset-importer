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

#ifndef __XLI_MUTEX_QUEUE_H__
#define __XLI_MUTEX_QUEUE_H__

#include <Xli/Mutex.h>
#include <Xli/Queue.h>

namespace Xli
{
    /**
        \ingroup XliContainers
    */
    template <typename T> class MutexQueue
    {
        Mutex mutex;
        Queue<T> queue;

    public:
        void Trim()
        {
            MutexLock lock(mutex);
            queue.Trim();
        }

        void Clear()
        {
            MutexLock lock(mutex);
            queue.Clear();
        }

        void Enqueue(const T& value)
        {
            MutexLock lock(mutex);
            queue.Enqueue(value);
        }

        T Dequeue()
        {
            MutexLock lock(mutex);
            return queue.Dequeue();
        }

        int Count()
        {
            MutexLock lock(mutex);
            return queue.Count();
        }

        bool IsEmpty()
        {
            MutexLock lock(mutex);
            return queue.IsEmpty();
        }
    };
}


#endif
