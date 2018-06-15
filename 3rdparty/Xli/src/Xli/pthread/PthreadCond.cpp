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

#include <Xli/ConditionVariable.h>
#include <pthread.h>

namespace Xli
{
    CondHandle CreateCond()
    {
        pthread_cond_t* handle = new pthread_cond_t;
        pthread_cond_init(handle, NULL);
        return (CondHandle)handle;
    }

    void DeleteCond(CondHandle handle)
    {
        pthread_cond_destroy((pthread_cond_t*)handle);
        delete (pthread_cond_t*)handle;
    }

    void CondWait(CondHandle handle, MutexHandle mutex)
    {
        pthread_cond_wait((pthread_cond_t*)handle, (pthread_mutex_t*)mutex);
    }

    void CondSignal(CondHandle handle)
    {
        pthread_cond_signal((pthread_cond_t*)handle);
    }

    void CondBroadcast(CondHandle handle)
    {
        pthread_cond_broadcast((pthread_cond_t*)handle);
    }
}
