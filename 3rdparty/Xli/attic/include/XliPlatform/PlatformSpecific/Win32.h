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

#ifndef __XLI_PLATFORM_SPECIFIC_WIN32_H__
#define __XLI_PLATFORM_SPECIFIC_WIN32_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Undefine conflicting names
#undef MessageBox
#undef GetSystemDirectory
#undef GetCurrentDirectory
#undef SetCurrentDirectory
#undef CreateDirectory
#undef ChangeDirectory
#undef DeleteFile
#undef MoveFile
#undef CreateMutex
#undef GetCurrentThread

#include <Xli/String.h>

namespace Xli
{
    class Window;

    namespace PlatformSpecific
    {
        /**
            \ingroup XliPlatform
        */
        class Win32
        {
        public:
            /**
                Returns a String describing the error code returned from ::GetLastError()
            */
            static String GetLastErrorString();

            static void RegisterTouchWindow(Window* wnd);
            static HWND GetWindowHandle(Window* wnd);
            static void SetWindowIconByID(Window* wnd, int id);
        };
    }
}

#endif
