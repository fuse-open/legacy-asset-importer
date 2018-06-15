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

#ifndef __XLI_THREAD_H__
#define __XLI_THREAD_H__

#include <Xli/Object.h>
#include <Xli/Shared.h>

namespace Xli
{
    struct __ThreadImpl;
    struct __TlsImpl;

    /**
        \addtogroup XliThreading
        @{
    */
    
    typedef __ThreadImpl* ThreadHandle;
    typedef __TlsImpl* TlsHandle;

    ThreadHandle CreateThread(void (*entrypoint)(void*), void* arg);
    void JoinThread(ThreadHandle handle);

    void SetCurrentThreadName(const String& name);
    void* GetCurrentThread();

    void Sleep(int ms);

    TlsHandle CreateTls(void (*destructor)(void*));
    void DeleteTls(TlsHandle handle);
    
    void SetTlsValue(TlsHandle handle, void* data);
    void* GetTlsValue(TlsHandle handle);

    /** @} */

    /**
        \ingroup XliThreading
    */
    class ThreadTask: public Object
    {
        friend class Thread;
        volatile bool _stopRequested;

    public:
        ThreadTask();
        virtual ~ThreadTask() {}

        bool IsStopRequested();

        virtual void Run() = 0;

        virtual String ToString() const;
    };

    /**
        \ingroup XliThreading
    */
    enum ThreadState
    {
        ThreadStateUnstarted,
        ThreadStateRunning,
        ThreadStateStopRequested,
        ThreadStateStopped,
    };

    /**
        \ingroup XliThreading
    */
    class Thread
    {
        ThreadHandle _handle;
        Shared<ThreadTask> _task;
        volatile ThreadState _state;

        static void thread_func(void* arg);

        Thread(const Thread& copy);
        Thread& operator = (const Thread& copy);

    public:
        /**
            Creates a new Thread.
            @param task Optional task to be executed by thread. If specified, the thread will start immediately.
        */
        Thread(Managed<ThreadTask> task = 0);
        ~Thread();

        ThreadState GetState();

        void Start(Managed<ThreadTask> task);
        void Join();
    };
}

#endif
