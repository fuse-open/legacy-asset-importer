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

#include <Xli/Thread.h>
#include <Xli/TextWriter.h>
#include <Xli/CoreLib.h>

namespace Xli
{
    void Thread::thread_func(void* arg)
    {
        Thread* thread = (Thread*)arg;
        ThreadTask* task = thread->_task;
        
        task->_stopRequested = false;
        SetCurrentThreadName(task->ToString());

        try
        {
            task->Run();
        }
        catch (const Exception& e)
        {
            CoreLib::OnUnhandledException(e, task->ToString());
        }
        catch (...)
        {
            Xli::Exception e("An unsupported C++ exception was thrown");
            CoreLib::OnUnhandledException(e, task->ToString());
        }

        thread->_state = ThreadStateStopped;
    }

    Thread::Thread(Managed<ThreadTask> task)
    {
        _handle = 0;
        _state = ThreadStateUnstarted;

        if (!task.IsNull())
            Start(task);
    }

    Thread::~Thread()
    {
        Join();
    }

    ThreadState Thread::GetState()
    {
        return _state;
    }

    void Thread::Start(Managed<ThreadTask> task)
    {
        if (task.IsNull())
            XLI_THROW_NULL_POINTER;

        if (_handle)
            XLI_THROW("Thread is already started");

        _task = task;
        _state = ThreadStateRunning;
        _handle = CreateThread(thread_func, (void*)this);
    }

    void Thread::Join()
    {
        if (!_handle)
            return;

        if (_state == ThreadStateRunning)
        {
            _task->_stopRequested = true;
            _state = ThreadStateStopRequested;
        }
        
        JoinThread(_handle);
        _handle = 0;
        _task = 0;
    }

    ThreadTask::ThreadTask()
    {
        _stopRequested = false;
    }

    bool ThreadTask::IsStopRequested()
    {
        return _stopRequested;
    }

    String ThreadTask::ToString() const
    {
        return "<ThreadTask " + DefaultTraits::ToString((void*)this) + ">";
    }
}
