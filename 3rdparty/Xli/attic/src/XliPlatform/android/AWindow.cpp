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

#include <android/window.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <cstdlib>
#include <errno.h>
#include <unistd.h>

#include <XliPlatform/PlatformSpecific/Android.h>
#include <XliPlatform/InputEventHandler.h>
#include <XliPlatform/Application.h>
#include <XliPlatform/Display.h>
#include <XliPlatform/Window.h>
#include <XliGL/GLContext.h>
#include <Xli/Console.h>
#include "AJniHelper.h"
#include "AWindow.h"
#include "AShim.h"

#include <Xli/Console.h>

static int GlobalFlags = 0;

namespace Xli
{
    namespace PlatformSpecific
    {
        AWindow::AWindow()
        {
            PrintLine("~~~~~~~~~~~~~~~~~ Window Construct");
            _javaSurface = 0;
            _surfaceANativeWindow = 0;
        }

        WindowImplementation AWindow::GetImplementation()
        {
            return WindowImplementationAndroid;
        }

        void AWindow::OnInitialize()
        {
            PrintLine("~~~~~~~~~~~~~~~~~ Window OnInitialize");
            context_.Initialize(Xli::GLContextAttributes::Default());
            GetEventHandler()->OnInitialize(this);
        }

        void AWindow::OnShow()
        {
            PrintLine("~~~~~~~~~~~~~~~~~ Window OnShow");
            context_.MakeCurrent(this);
            GetEventHandler()->OnShow(this);
            Xli::Application::SharedApp()->EnterForeground();            
        }

        void AWindow::OnHide()
        {
            PrintLine("~~~~~~~~~~~~~~~~~ Window OnHide");
            GetEventHandler()->OnHide(this);
        }

        void AWindow::OnDraw()
        {
            context_.SwapBuffers();
        }

        void AWindow::OnDestroy()
        {
            PrintLine("~~~~~~~~~~~~~~~~~ Window OnDestroy");
            GetEventHandler()->OnDestroy(this);
        }

        String AWindow::GetTitle()
        {
            return "";
        }
        void AWindow::SetTitle(const String& title) {}

        Vector2i AWindow::GetClientSize()
        {
            return Display::GetScreenSize();
        }
        void AWindow::SetClientSize(Vector2i size) {}

        Vector2i AWindow::GetPosition()
        {
            return Vector2i(0, 0); //{TODO} how does this relate to nativeui?
        }
        void AWindow::SetPosition(Vector2i pos) {}

        void AWindow::SetFullscreen(bool fullscreen) {}

        void AWindow::Maximize() {}
        void AWindow::Minimize() {}
        void AWindow::Restore() {}
        void AWindow::Close() {}

        bool AWindow::IsClosed() { return CurrentState() == Hidden; }
        bool AWindow::IsFullscreen() { return CurrentState() == Visible; }
        bool AWindow::IsMaximized() { return CurrentState() == Visible; }
        bool AWindow::IsMinimized() { return CurrentState() == Hidden; }

        int AWindow::GetDisplayIndex() { return 0; }

        void* AWindow::GetNativeHandle()
        {
            if (_surfaceANativeWindow==0) {                
                AJniHelper jni;
                _javaSurface = reinterpret_cast<jobject>(jni->NewGlobalRef(AShim::GetUnoSurface()));
                _surfaceANativeWindow = ANativeWindow_fromSurface(jni.GetEnv(), _javaSurface);
            }
            return _surfaceANativeWindow;
        }
        GLContext* AWindow::GetContext() { return &context_; }
    }
    Window* Window::GetMainWindow()
    {
        return Xli::Application::SharedApp()->RootWindow();
    }
    void Window::SetMainWindow(Window* wnd) {}

    void InitWindow()
    {
    }

    void TerminateWindow()
    {
    }
}


// Window* Window::Create(int width, int height, const String& title, int flags)
// {
//     if (GlobalWindow != 0)
//         XLI_THROW("Only one window instance is allowed on the Android platform");

//     GlobalWidth = width;
//     GlobalHeight = height;
//     GlobalFlags = flags;
//     GlobalWindow = new PlatformSpecific::AWindow();

//     return GlobalWindow;
// }


// virtual void Close()
// {
//     if (GlobalAndroidApp->destroyRequested == 1 ||
//         (GlobalEventHandler != 0 &&
//          GlobalEventHandler->OnClosing(this)))
//         return;

//     GlobalAndroidApp->destroyRequested = 1;

//     if (GlobalEventHandler != 0)
//         GlobalEventHandler->OnClosed(this);
// }

// virtual SystemCursor GetSystemCursor()
// {
//     return SystemCursorNone;
// }

// virtual void SetSystemCursor(SystemCursor cursor)
// {
// }
