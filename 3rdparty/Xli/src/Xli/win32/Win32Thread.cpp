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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>

// From: http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
    DWORD dwType; // Must be 0x1000.
    LPCSTR szName; // Pointer to name (in user addr space).
    DWORD dwThreadID; // Thread ID (-1=caller thread).
    DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

void SetThreadName(DWORD dwThreadID, const char* threadName)
{
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = threadName;
    info.dwThreadID = dwThreadID;
    info.dwFlags = 0;

    __try
    {
        RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
    }
}

#include <Xli/Thread.h>
#include <Xli/Console.h>

namespace Xli
{
    struct Win32ThreadData
    {
        void (*Entrypoint)(void*);
        void* Arg;
    };

    unsigned int __stdcall Win32ThreadFunc(void* data)
    {
        Win32ThreadData* win32Data = reinterpret_cast<Win32ThreadData*>(data);
        win32Data->Entrypoint(win32Data->Arg);
        delete win32Data;
        return 0;
    }

    ThreadHandle CreateThread(void (*entrypoint)(void*), void* arg)
    {
        Win32ThreadData* data = new Win32ThreadData;
        data->Entrypoint = entrypoint;
        data->Arg = arg;

        unsigned threadId;
        HANDLE thread = (HANDLE)_beginthreadex(NULL, 0, &Win32ThreadFunc, data, 0, &threadId);
        
        if (!thread) 
            XLI_THROW("Failed to create thread");
        
        return (ThreadHandle)thread;
    }

    void JoinThread(ThreadHandle handle)
    {
        DWORD res = ::WaitForSingleObject((HANDLE)handle, INFINITE);

        if (res != WAIT_OBJECT_0)
            Error->WriteLine("XLI ERROR: WaitForSingleObject failed");
        else
            ::CloseHandle((HANDLE)handle);
    }

    void SetCurrentThreadName(const String& name)
    {
        ::SetThreadName(-1, name.Ptr());
    }

    void* GetCurrentThread()
    {
        return (void*)::GetCurrentThread();
    }

    void Sleep(int ms)
    {
        ::Sleep(ms);
    }

    TlsHandle CreateTls(void (*destructor)(void*))
    {
        return (TlsHandle)::TlsAlloc();
    }

    void DeleteTls(TlsHandle handle)
    {
        ::TlsFree((DWORD)handle);
    }
    
    void SetTlsValue(TlsHandle handle, void* data)
    {
        ::TlsSetValue((DWORD)handle, data);
    }

    void* GetTlsValue(TlsHandle handle)
    {
        return ::TlsGetValue((DWORD)handle);
    }
}
