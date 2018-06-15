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

#include <Xli/CoreLib.h>
#include <Xli/StringBuilder.h>
#include <XliPlatform/Bundle.h>
#include <XliPlatform/Disk.h>
#include <XliPlatform/MessageBox.h>
#include <XliPlatform/PlatformLib.h>
#include <XliPlatform/Window.h>
#include <cstdlib>
#include <cstring>

namespace Xli
{
    BundleAccessor Bundle;
    DiskAccessor Disk;

    void InitWindow();
    void TerminateWindow();
    
    FileSystem* CreateBundleFileSystem();
    NativeFileSystem* CreateNativeFileSystem();

    static FileSystem* as;
    static NativeFileSystem* fs;
    static bool LibInited;

    static void ExceptionCallback(const Exception& e, const String& where)
    {
        StringBuilder sb(2048);
        sb.Append(e.GetMessage());
        sb.Append("\n\nFunction: ");
        sb.Append(e.GetFunction());
        sb.Append("\nLine: ");
        sb.Append(String(e.GetLine()));

        if (where.Length() > 0)
            sb.Append("\n\n(from: " + where + ")");

        MessageBox::Show(Window::GetMainWindow(), sb.ToString(), "Unhandled Exception", DialogButtonsOK, DialogHintsError);
    }

    static void Terminate()
    {
        TerminateWindow();

        as->Release();
        fs->Release();

        LibInited = false;
    }

    void PlatformLib::Init()
    {
        if (CoreLib::EnterCriticalIfFalse(&LibInited))
        {
            CoreLib::SetUnhandledExceptionCallback(ExceptionCallback);

            fs = CreateNativeFileSystem();
            as = CreateBundleFileSystem();
            
            InitWindow();

            CoreLib::ExitCritical();
            atexit(Terminate);
        }
    }

    void BundleAccessor::SetFilesystem(FileSystem* fs)
    {
        PlatformLib::Init();
        as->Release();
        fs->AddRef();
        as = fs;
    }

    FileSystem* BundleAccessor::operator ->()
    {
        PlatformLib::Init();
        return as;
    }

    BundleAccessor::operator FileSystem*()
    {
        PlatformLib::Init();
        return as;
    }

    NativeFileSystem* DiskAccessor::operator ->()
    {
        PlatformLib::Init();
        return fs;
    }

    DiskAccessor::operator FileSystem*()
    {
        PlatformLib::Init();
        return fs;
    }
}
