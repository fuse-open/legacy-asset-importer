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

#ifndef __XLI_PLATFORM_WINDOW_H__
#define __XLI_PLATFORM_WINDOW_H__

#include <Xli/String.h>
#include <Xli/Vector2.h>

namespace Xli
{
    class GLContext;
    class WindowEventHandler;

    /**
        \ingroup XliPlatform
    */
    enum WindowImplementation
    {
        WindowImplementationUnknown,
        WindowImplementationWin32,
        WindowImplementationAndroid,
        WindowImplementationSDL2,
        WindowImplementationiOS,
    };

    class Window
    {
    public:
        enum State
        {
            Uninitialized,
            Initializing,
            Visible,
            Hidden,
            Destroying,
        };

        Window()
        {
            state_ = Uninitialized;
        }

        virtual ~Window() {}

        State CurrentState() const { return state_; }

        void Initialize();
        void Show();
        void Hide();
        void Destroy();

        virtual void OnDraw() {}

        /**
            Returns the window implementation type (enum)
        */        
        WindowImplementation GetImplementation();

        void SetEventHandler(WindowEventHandler* handler)
        {
            eventHandler_ = handler;
        }

        WindowEventHandler* GetEventHandler()
        {
            return eventHandler_;
        }

        //{TODO}
        /**
           Closes the window
        */
        virtual void Close() = 0;

        /**
           Returns wether or not the window has been closed by the user or operating system.
        */
        virtual bool IsClosed() = 0;
        
        /**
            Sets the fullscreen mode of the window. 
            The window will go fullscreen on the display which contains the windows centre coordinate and stretched to that displays resolution.
        */
        virtual void SetFullscreen(bool fullscreen) = 0;

        /**
            Returns if this is a fullscreen window
        */
        virtual bool IsFullscreen() = 0;

        /**
            Minimizes the window
        */
        virtual void Minimize() = 0;

        /**
            Maximizes the window
        */
        virtual void Maximize() = 0;

        /**
            Restores the window from maximized/minimized state to normal position
        */
        virtual void Restore() = 0;

        /**
            Returns if this window is minimized
        */
        virtual bool IsMinimized() = 0;

        /**
            Returns if this window is maximized
        */
        virtual bool IsMaximized() = 0;

        /**
            Sets the windows title
        */
        virtual void SetTitle(const String& title) = 0;

        /**
            Returns the windows title
        */
        virtual String GetTitle() = 0;

        /**
            Sets the position of the window
        */
        virtual void SetPosition(Vector2i pos) = 0;

        /**
            Returns the position of the window
        */
        virtual Vector2i GetPosition() = 0;

        /**
            Sets the size of the client area of the window
        */
        virtual void SetClientSize(Vector2i size) = 0;

        /**
            Returns the size of the client area of the window
        */
        virtual Vector2i GetClientSize() = 0;

        /**
            Returns the index for the display containing this windows center coordinate
        */
        virtual int GetDisplayIndex() = 0;

        /**
            Returns the native windows handle
        */
        virtual void* GetNativeHandle() = 0;

        /**
           Returns the Window's GLContext
        */        
        virtual GLContext* GetContext() = 0;
        
        /**
            Sets the window that should be used as main window
        */
        static void SetMainWindow(Window* wnd);

        /**
            Returns the first window created or the window currently set using SetMainWindow().
            If no window is created or the main window is destroyed this function will return NULL.
        */
        static Window* GetMainWindow();

    protected:
        virtual void OnInitialize() {}
        virtual void OnShow() {}
        virtual void OnHide() {}
        virtual void OnDestroy() {}

        // Called from OnDestroy() if re-initialization is allowed
        void SetUninitialized();

    private:
        Window(Window const &);
        void operator=(Window const &);

        State state_;
        WindowEventHandler *eventHandler_;
    };

}

#endif
