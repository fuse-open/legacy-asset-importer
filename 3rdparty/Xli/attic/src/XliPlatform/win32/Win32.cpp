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

#include <XliPlatform/PlatformSpecific/Win32.h>
#include <XliPlatform/Window.h>
#include <Xli/Unicode.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        String Win32::GetLastErrorString()
        {
            LPWSTR lpMsgBuf;
            DWORD dw = GetLastError();

            FormatMessageW(
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                dw,
                MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
                (LPWSTR)&lpMsgBuf, // Cast because callee is allocating buffer
                0, NULL);

            String msg = Unicode::Utf16To8(lpMsgBuf);
            LocalFree(lpMsgBuf);

            return msg;
        }

        HWND Win32::GetWindowHandle(Window* wnd)
        {
            if (!wnd || wnd->GetImplementation() != WindowImplementationWin32)
                return NULL;

            return (HWND)wnd->GetNativeHandle();
        }

        void Win32::RegisterTouchWindow(Window* wnd)
        {
            ::RegisterTouchWindow(GetWindowHandle(wnd), 0);
        }

        void Win32::SetWindowIconByID(Window* wnd, int id)
        {
            SendMessage(GetWindowHandle(wnd), WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(id)));
            SendMessage(GetWindowHandle(wnd), WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(id)));
        }
    }
}
