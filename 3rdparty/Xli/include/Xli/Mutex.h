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

#ifndef __XLI_MUTEX_H__
#define __XLI_MUTEX_H__

#include <Xli/Object.h>
#include <Xli/Exception.h>

namespace Xli
{
    struct __MutexImpl;

    /**
        \addtogroup XliThreading
        @{
    */
    typedef __MutexImpl* MutexHandle;

    MutexHandle CreateMutex();
    void DeleteMutex(MutexHandle mutex);

    void LockMutex(MutexHandle mutex);
    void UnlockMutex(MutexHandle mutex);

    /** @} */

    /**
        \ingroup XliThreading
    */
    class Mutex
    {
        MutexHandle handle;

        Mutex(const Mutex& copy);
        Mutex& operator = (const Mutex& copy);

    public:
        Mutex()
        {
            handle = CreateMutex();
        }

        ~Mutex()
        {
            DeleteMutex(handle);
        }

        void Lock()
        {
            LockMutex(handle);
        }

        void Unlock()
        {
            UnlockMutex(handle);
        }

        operator MutexHandle()
        {
            return handle;
        }
    };

    /**
        \ingroup XliThreading
    */
    class MutexLock
    {
        MutexHandle handle;
    
        MutexLock(const MutexLock& copy);
        MutexLock& operator = (const MutexLock& copy);

    public:
        MutexLock(MutexHandle handle)
        {
            LockMutex(this->handle = handle);
        }

        ~MutexLock()
        {
            UnlockMutex(this->handle);
        }
    };
}

#endif
