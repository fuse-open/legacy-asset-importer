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

#ifndef __XLI_APPLICATION_H__
#define __XLI_APPLICATION_H__

#include <stdint.h>

#include <XliPlatform/InputEventHandler.h>
#include <XliPlatform/WindowEventHandler.h>

extern "C" int main(int argc, char** argv);

namespace Xli
{
    class Window;

    class Application: public InputEventHandler, public WindowEventHandler
    {
    public:
        enum State
        {
            Uninitialized,
            Starting,
            Foreground,
            Interactive,
            Background,
            Terminating,
        };

        // NOTE: Don't cache result beyond a single callback. After Terminate,
        // SharedApp() returns a dummy handler.
        static Application* SharedApp()
        {
            return application_;
        }

        int Run(int argc, char **argv);

        char const *GetTitle() const;
        State CurrentState() const { return state_; }

        void Start();
        void EnterForeground();
        void EnterInteractive();
        void ExitInteractive();
        void EnterBackground();
        void Terminate();

        void SetRootWindow(Window *window) { rootWindow_ = window; }
        Window* RootWindow() { return rootWindow_; }

        virtual unsigned FrameRate() const;
        virtual void SetFrameRate(unsigned frameRate);

        virtual String GetInitTitle();
        virtual Vector2i GetInitSize();

        // Keyboard and TextInput
        void BeginTextInput(TextInputHint hint);
        void EndTextInput();

#if defined(XLI_PLATFORM_ANDROID) || defined(XLI_PLATFORM_IOS)
        bool HasOnscreenKeyboardSupport() { return true; }
        bool IsOnscreenKeyboardVisible();
#endif

#if !defined(XLI_PLATFORM_ANDROID) && !defined(XLI_PLATFORM_IOS)
        void SetOnscreenKeyboardPosition(Vector2i position);
#endif

        Vector2i GetOnscreenKeyboardPosition();
        Vector2i GetOnscreenKeyboardSize();
#if defined(XLI_PLATFORM_IOS)
        Recti GetOnscreenKeyboardBounds();
#endif

        virtual void OnUpdateFrame() {}
        virtual void OnLowMemory() {}

    protected:
        virtual void OnStart() {}
        virtual void OnDidStart() {}
        virtual void OnEnterForeground() {}
        virtual void OnEnterInteractive() {}
        virtual void OnExitInteractive() {}
        virtual void OnEnterBackground() {}
        virtual void OnTerminate() {}


        Application() : state_(Uninitialized), rootWindow_(0) {}
        virtual ~Application() {}

    private:
        virtual void _FreeResources();
        Application(Application const &);
        void operator=(Application const &);

        void EmitOnStart();
        void EmitOnDidStart();
        void EmitOnEnterForeground();
        void EmitOnEnterInteractive();
        void EmitOnExitInteractive();
        void EmitOnEnterBackground();
        void EmitOnTerminate();
        void EmitOnLowMemory();

        State state_;
        Window *rootWindow_;

        static Application* application_;
        friend int ::main(int argc, char** argv);
    };
}

#endif
