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

#include <XliPlatform/Application.h>
#include <XliPlatform/Window.h>
#include <Xli/Console.h>
#include <Xli/Exception.h>

#include <assert.h>

namespace Xli
{
    void Window::Initialize()
    {
        if (state_ != Uninitialized)
        {
            if (state_ == Destroying)
                XLI_THROW("Initialize() called on destroyed Window");
            return;
        }

        state_ = Initializing;
        OnInitialize();

        assert(state_ == Initializing);
    }

    void Window::Show()
    {
        switch (state_)
        {
        case Destroying:
            XLI_THROW("Show() called on destroyed Window");

        case Uninitialized:
            Initialize();

        case Initializing:
        case Hidden:
            state_ = Visible;
            OnShow();

        case Visible:
            // On it!
            break;
        }

        assert(state_ == Visible);
    }

    void Window::Hide()
    {
        switch (state_)
        {
        case Destroying:
            XLI_THROW("Hide() called on destroyed Window");

        case Uninitialized:
            Initialize();
            state_ = Hidden;
            break;

        case Visible:
            OnHide();

        case Initializing:
            // Implicitly Hidden once initialization completes.
            state_ = Hidden;

        case Hidden:
            // On it!
            break;
        }

        assert(state_ == Hidden);
    }

    void Window::Destroy()
    {
        switch (state_)
        {
        case Visible:
            Hide();

        case Initializing:
        case Hidden:
            state_ = Destroying;
            OnDestroy();

            if (state_ == Destroying)
            {
                // Don't generate events on destroyed instance
                static WindowEventHandler _postDestroyDummy;
                eventHandler_ = &_postDestroyDummy;

                Application *app = Application::SharedApp();
                if (app->RootWindow() == this)
                    app->SetRootWindow(0);
            }

        case Destroying:
            // On it!
        case Uninitialized:
            // Nothing to do
            break;
        }

        assert(state_ == Uninitialized
            || state_ == Destroying);
    }

    void Window::SetUninitialized()
    {
        if (state_ != Destroying)
            XLI_THROW("SetUninitialized() called on live Window");
        state_ = Uninitialized;
    }
}
