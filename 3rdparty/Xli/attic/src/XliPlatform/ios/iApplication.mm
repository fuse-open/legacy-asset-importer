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

#include <XliPlatform/Application.h>
#include <XliPlatform/Window.h>

#include "iWindow.h"
#include "AppDelegate.h"

#include <Xli/Console.h>

namespace Xli
{
    static Xli_AppDelegate *appDelegate_ = 0;

    static PlatformSpecific::iWindow window_;
    static CADisplayLink* displayLink_ = 0;

    int Application::Run(int argc, char** argv)
    {
        struct Finalizer
        {
            Finalizer()
            {
                pool_ = [[NSAutoreleasePool alloc] init];
            }

            ~Finalizer()
            {
                Xli::Application::SharedApp()->Terminate();
                [pool_ drain];
            }

        private:
            NSAutoreleasePool *pool_;
        };

        Finalizer finalizer;
        return UIApplicationMain(
            argc, argv, nil, NSStringFromClass([Xli_AppDelegate class]));
    }

    unsigned Application::FrameRate() const
    {
        if (!displayLink_ || displayLink_.paused)
            return 0;
        return 1.0 / (displayLink_.frameInterval * displayLink_.duration);
    }

    void Application::SetFrameRate(unsigned frameRate)
    {
        if (!displayLink_)
            return;

        displayLink_.frameInterval
            = 1.0 / frameRate / displayLink_.duration;
    }

    void Application::EmitOnStart()
    {
        PrintLine("----------------- EmitOnStart");

        window_.SetEventHandler(this);
        SetRootWindow(&window_);

        window_.Initialize();

        appDelegate_ = (Xli_AppDelegate *)
            [UIApplication sharedApplication].delegate;

        displayLink_ = [CADisplayLink displayLinkWithTarget:appDelegate_
            selector:@selector(Xli_OnUpdateFrame:)];

        [displayLink_ addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
        displayLink_.paused = YES;
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
        displayLink_.frameInterval = 3;
        displayLink_.paused = NO;

        OnEnterForeground();
    }

    void Application::EmitOnEnterInteractive()
    {
        PrintLine("----------------- EmitOnEnterInteractive");
        displayLink_.frameInterval = 1;
        OnEnterInteractive();
    }

    void Application::EmitOnExitInteractive()
    {
        PrintLine("----------------- EmitOnExitInteractive");
        OnExitInteractive();
        displayLink_.frameInterval = 3;
    }

    void Application::EmitOnEnterBackground()
    {
        PrintLine("----------------- EmitOnEnterBackground");
        displayLink_.paused = YES;
        OnEnterBackground();
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
        [displayLink_ invalidate];
        displayLink_ = 0;

        window_.Destroy();
    }

    ////////////////////////////////////////////////////////////////////////////
    // Keyboard + TextInput
    ////////////////////////////////////////////////////////////////////////////

    void Application::BeginTextInput(TextInputHint hint)
    {
        [appDelegate_ Xli_showKeyboardWithHint:hint];
    }

    void Application::EndTextInput()
    {
        [appDelegate_ Xli_hideKeyboard];
    }

    bool Application::IsOnscreenKeyboardVisible()
    {
        return [appDelegate_ Xli_isKeyboardVisible];
    }

    Recti Application::GetOnscreenKeyboardBounds()
    {
        return appDelegate_->_Xli_keyboardBounds;
    }

    Vector2i Application::GetOnscreenKeyboardPosition()
    {
        Recti bounds = appDelegate_->_Xli_keyboardBounds;
        return bounds.Position();
    }

    Vector2i Application::GetOnscreenKeyboardSize()
    {
        Recti bounds = appDelegate_->_Xli_keyboardBounds;
        return bounds.Size();
    }
}
