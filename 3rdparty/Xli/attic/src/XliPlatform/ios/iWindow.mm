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

#include <Xli.h>
#include <XliGL/GLContext.h>
#include <XliPlatform/Application.h>
#include <XliPlatform/Display.h>
#include <XliPlatform/Window.h>
#include <XliPlatform/InputEventHandler.h>

#include "AppDelegate.h"
#include "iWindow.h"

#include <UIKit/UIKit.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <QuartzCore/QuartzCore.h>

@interface Xli_GLView : UIView
@end

@implementation Xli_GLView
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}
@end

namespace Xli
{
    namespace PlatformSpecific
    {
        WindowImplementation iWindow::GetImplementation()
        {
            return WindowImplementationiOS;
        }

        void iWindow::OnInitialize()
        {
            assert(window_ == 0);

            CGRect screenBounds = [[UIScreen mainScreen] bounds];

            window_ = [[UIWindow alloc] initWithFrame:screenBounds];
            window_.rootViewController = (UIViewController *)
                [UIApplication sharedApplication].delegate;
            window_.rootViewController.view = [[[Xli_GLView alloc]
                initWithFrame:screenBounds] autorelease];

            window_.rootViewController.view.contentScaleFactor
                = [[UIScreen mainScreen] scale];
            // TODO: Is this something we want on by default? Is there a setting
            // for it somewhere?
            window_.rootViewController.view.multipleTouchEnabled = YES;

            context_.Initialize(
                (CAEAGLLayer *) window_.rootViewController.view.layer);

            window_.hidden = false;
            [window_ makeKeyWindow];
            GetEventHandler()->OnInitialize(this);
        }

        void iWindow::OnShow()
        {
            context_.AllocateBuffersAndMakeCurrent();
            GetEventHandler()->OnShow(this);
        }

        void iWindow::OnHide()
        {
            context_.FreeBuffers();
            GetEventHandler()->OnHide(this);
        }

        void iWindow::OnDraw()
        {
            context_.SwapBuffers();
        }

        void iWindow::OnDestroy()
        {
            GetEventHandler()->OnDestroy(this);
            context_.Destroy();
            [window_ release];

            window_ = 0;

            Window::SetUninitialized();
        }

        String iWindow::GetTitle() { return "";}

        Vector2i iWindow::GetClientSize() { return context_.GetDrawableSize(); }
        Vector2i iWindow::GetPosition() { return Vector2i(0, 0); }

        bool iWindow::IsClosed() { return CurrentState() == Hidden; }
        bool iWindow::IsFullscreen() { return CurrentState() == Visible; }
        bool iWindow::IsMaximized() { return CurrentState() == Visible; }
        bool iWindow::IsMinimized() { return CurrentState() == Hidden; }

        int iWindow::GetDisplayIndex() { return 0; }

        void iWindow::Close() { Destroy(); }
        void iWindow::Maximize() { Show(); }
        void iWindow::Minimize() { Hide(); }
        void iWindow::Restore() { Show(); }

        void iWindow::SetClientSize(Vector2i size) {}
        void iWindow::SetFullscreen(bool fullscreen) {}
        void iWindow::SetPosition(Vector2i pos) {}
        void iWindow::SetTitle(const String& title) {}

        void *iWindow::GetNativeHandle() { return (void *)window_; }
        GLContext* iWindow::GetContext() { return &context_; }
    }

    Window* Window::GetMainWindow() { return (Window*)0; }

    void Window::SetMainWindow(Window* wnd) {}
}
