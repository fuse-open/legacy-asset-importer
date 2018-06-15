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

#ifndef __XLI_PLATFORM_SPECIFIC_WIN32_WINDOW_H__
#define __XLI_PLATFORM_SPECIFIC_WIN32_WINDOW_H__

#include <XliPlatform/PlatformSpecific/Win32.h>
#include <XliPlatform/InputEventHandler.h>
#include <XliPlatform/Window.h>
#include <Xli/Shared.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        /**
            \ingroup XliPlatform
        */
        class Win32Window : public Window
        {
        public:
            HWND hWnd;
            bool closed, ownsHwnd;

            Shared<InputEventHandler> eventHandler;

            DWORD dwStyle;
            RECT rect;
            bool fullscreen;

            static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

            Win32Window(int width, int height, const Xli::String& title, int flags);
            Win32Window(HWND hWnd);
            virtual ~Win32Window();

            virtual WindowImplementation GetImplementation();

            virtual void SetEventHandler(InputEventHandler* handler);
            virtual InputEventHandler* GetEventHandler();

            virtual void Close();

            virtual bool IsClosed();
            virtual bool IsVisible();
            virtual bool IsFullscreen();
            virtual bool IsMinimized();
            virtual bool IsMaximized();

            virtual int GetDisplayIndex();

            virtual String GetTitle();
            virtual Vector2i GetPosition();
            virtual Vector2i GetClientSize();
            virtual void* GetNativeHandle() { return (void*)hWnd; }

            virtual void SetTitle(const String& title);
            virtual void SetFullscreen(bool fullscren);
            virtual void SetPosition(Vector2i pos);
            virtual void SetClientSize(Vector2i size);

            virtual void Minimize();
            virtual void Maximize();
            virtual void Restore();

            virtual bool GetKeyState(Key key);
            virtual bool GetMouseButtonState(MouseButton button);
            virtual Vector2i GetMousePosition();
            virtual void SetMousePosition(Vector2i position);

            virtual SystemCursor GetSystemCursor();
            virtual void SetSystemCursor(SystemCursor cursor);
        };
    }
}


#endif
