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

#include <XliPlatform/PlatformSpecific/Android.h>
#include <XliPlatform/Application.h>
#include <XliPlatform/Display.h>
#include <XliPlatform/Window.h>
#include <XliPlatform/PlatformLib.h>
#include <Xli/Managed.h>
#include <Xli/Thread.h>
#include <Xli/Time.h>

#include "AWindow.h"
#include "AShim.h"

#include <Xli/Console.h>

namespace Xli
{
    static PlatformSpecific::AWindow window_;
    static bool event_loop_running = true;

    int Application::Run(int argc, char** argv)
    {
        window_.SetEventHandler(this);
    }

    unsigned Application::FrameRate() const
    {
        return 0;
    }

    void Application::SetFrameRate(unsigned frameRate)
    {
    }

    void Application::EmitOnStart()
    {
        PrintLine("----------------- EmitOnStart");

        window_.SetEventHandler(this);
        SetRootWindow(&window_);

        OnStart();
    }

    void Application::EmitOnDidStart()
    {
        PrintLine("----------------- EmitOnDidStart");
        OnDidStart();
    }

    void Application::EmitOnEnterForeground()
    {
        PrintLine("----------------- EmitOnEnterForeground");
        OnEnterForeground();
    }

    void Application::EmitOnEnterInteractive()
    {
        PrintLine("----------------- EmitOnEnterInteractive");
        OnEnterInteractive();
    }

    void Application::EmitOnExitInteractive()
    {
        PrintLine("----------------- EmitOnExitInteractive");
        OnExitInteractive();
    }

    void Application::EmitOnEnterBackground()
    {
        PrintLine("----------------- EmitOnEnterBackground");
        OnEnterBackground();
        window_.Hide();
    }

    void Application::EmitOnTerminate()
    {
        PrintLine("----------------- EmitOnTerminate");
        OnTerminate();
        _FreeResources();
    }

    void Application::EmitOnLowMemory()
    {
        PrintLine("----------------- EmitOnLowMemory");
        OnLowMemory();
        if (window_.CurrentState() == Window::Hidden)
            _FreeResources();
    }

    void Application::_FreeResources()
    {
        window_.Destroy();
    }

    void Application::BeginTextInput(TextInputHint hint)
    {
        Xli::PlatformSpecific::AShim::RaiseSoftKeyboard();
    }
    void Application::EndTextInput()
    {
        Xli::PlatformSpecific::AShim::HideSoftKeyboard();
    }
    bool Application::IsOnscreenKeyboardVisible()
    {
        return Xli::PlatformSpecific::AShim::KeyboardVisible();
    }
    Vector2i Application::GetOnscreenKeyboardPosition()
    {        
        return Vector2i(0, Display::GetScreenSize().Y - Xli::PlatformSpecific::AShim::GetKeyboardSize());
    }
    Vector2i Application::GetOnscreenKeyboardSize()
    {
        return Vector2i(Display::GetScreenSize().X, PlatformSpecific::AShim::GetKeyboardSize());
    }
}
