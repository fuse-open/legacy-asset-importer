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
#include <Xli/Console.h>
#include <pthread.h>
#include <time.h>
#include <cstdlib>
#include <inttypes.h>

namespace Xli
{
    struct ThreadData
    {
        void (*Entrypoint)(void*);
        void* Arg;
    };

    void* ThreadFunc(void* arg)
    {
        ThreadData* data = (ThreadData*)arg;
        data->Entrypoint(data->Arg);
        free(data);
        return NULL;
    }

    ThreadHandle CreateThread(void (*entrypoint)(void*), void* arg)
    {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        ThreadData* data = (ThreadData*)malloc(sizeof(ThreadData));
        data->Entrypoint = entrypoint;
        data->Arg = arg;

        pthread_t handle;
        if (pthread_create(&handle, &attr, ThreadFunc, data))    
            XLI_THROW("Failed to create thread");

        pthread_attr_destroy(&attr);
        return (ThreadHandle)handle;
    }

    void JoinThread(ThreadHandle handle)
    {
        int result = pthread_join((pthread_t)handle, NULL);
        
        if (result) 
            Error->WriteLine("XLI ERROR: pthread_join failed: " + String::HexFromInt(result));
    }

    void SetCurrentThreadName(const String& name)
    {
#ifdef __APPLE__
        pthread_setname_np(name.Ptr());
#else
        pthread_setname_np(pthread_self(), name.Ptr());
#endif
    }

    void* GetCurrentThread()
    {
        return (void*)pthread_self();
    }

    void Sleep(int ms)
    {
        struct timespec t, r;
        t.tv_sec = ms / 1000;
        t.tv_nsec = (ms % 1000) * 1000 * 1000;
        
        int result = nanosleep(&t, &r);
        
        if (result)
            Error->WriteLine("XLI ERROR: nanosleep failed: " + String::HexFromInt(result));
    }

    TlsHandle CreateTls(void (*destructor)(void*))
    {
        pthread_key_t handle;
        if (pthread_key_create(&handle, destructor))
            XLI_THROW("Failed to create TLS");

        return (TlsHandle)(intptr_t)handle;
    }

    void DeleteTls(TlsHandle handle)
    {
        pthread_key_delete((pthread_key_t)(intptr_t)handle);
    }

    void SetTlsValue(TlsHandle handle, void* data)
    {
        pthread_setspecific((pthread_key_t)(intptr_t)handle, data);
    }

    void* GetTlsValue(TlsHandle handle)
    {
        return pthread_getspecific((pthread_key_t)(intptr_t)handle);
    }
}
