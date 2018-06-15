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

#include <Xli/Console.h>
#include <Xli/CoreLib.h>
#include <Xli/Disk.h>
#include <Xli/File.h>
#include <Xli/Mutex.h>
#include <cstdlib>

namespace Xli
{
    StdOutAccessor Out;
    StdErrAccessor Error;
    StdInAccessor In;
    DiskAccessor Disk;
    
    NativeFileSystem* CreateDiskFileSystem();
    
    static void(*ExceptionCallback)(const Exception&, const String&);
    static TextWriter* OutWriter;
    static TextWriter* ErrWriter;
    static TextReader* InReader;
    static MutexHandle LibMutex;
    static NativeFileSystem* DiskFS;
    static bool LibInited;

    static void Terminate()
    {
        delete DiskFS;
        DiskFS = 0;

        OutWriter->GetStream()->Flush();
        ErrWriter->GetStream()->Flush();
        delete OutWriter;
        delete ErrWriter;
        delete InReader;
        OutWriter = 0;
        ErrWriter = 0;
        InReader = 0;

        DeleteMutex(LibMutex);
        LibInited = false;
    }

    void CoreLib::Init()
    {
        if (!LibInited)
        {
            LibInited = true;
            LibMutex = CreateMutex();
            atexit(Terminate);

            OutWriter = new TextWriter(ManagePtr(new File(stdout, FileFlagsCanWrite | FileFlagsIgnoreWriteErrors)));
            ErrWriter = new TextWriter(ManagePtr(new File(stderr, FileFlagsCanWrite | FileFlagsIgnoreWriteErrors)));
            InReader = new TextReader(ManagePtr(new File(stdin, FileFlagsCanRead)));

            DiskFS = CreateDiskFileSystem();
        }
    }

    void CoreLib::SetUnhandledExceptionCallback(void(*callback)(const Exception&, const String&))
    {
        ExceptionCallback = callback;
    }

    void CoreLib::OnUnhandledException(const Exception& exception, const String& where)
    {
        Error->WriteLine();
        Error->WriteLine((String)"UNHANDLED EXCEPTION (" + where + "):");
        Error->WriteLine();
        Error->WriteLine(exception.GetMessage());
        Error->WriteLine();
        Error->WriteLine((String)"Function: " + exception.GetFunction());
        Error->WriteLine((String)"Line: " + exception.GetLine());
        Error->GetStream()->Flush();

        if (ExceptionCallback)
            ExceptionCallback(exception, where);
    }

    bool CoreLib::EnterCriticalIfFalse(bool* flag)
    {
        if (*flag)
            return false;

        CoreLib::Init();
        LockMutex(LibMutex);

        if (*flag)
        {
            UnlockMutex(LibMutex);
            return false;
        }

        return *flag = true;
    }

    void CoreLib::EnterCritical()
    {
        LockMutex(LibMutex);
    }

    void CoreLib::ExitCritical()
    {
        UnlockMutex(LibMutex);
    }

    TextWriter* StdOutAccessor::operator ->()
    {
        CoreLib::Init();
        return OutWriter;
    }

    StdOutAccessor::operator TextWriter*()
    {
        CoreLib::Init();
        return OutWriter;
    }

    TextWriter* StdErrAccessor::operator ->()
    {
        CoreLib::Init();
        return ErrWriter;
    }

    StdErrAccessor::operator TextWriter*()
    {
        CoreLib::Init();
        return ErrWriter;
    }

    TextReader* StdInAccessor::operator ->()
    {
        CoreLib::Init();
        return InReader;
    }

    StdInAccessor::operator TextReader*()
    {
        CoreLib::Init();
        return InReader;
    }

    NativeFileSystem* DiskAccessor::operator ->()
    {
        CoreLib::Init();
        return DiskFS;
    }

    DiskAccessor::operator FileSystem*()
    {
        CoreLib::Init();
        return DiskFS;
    }
}
