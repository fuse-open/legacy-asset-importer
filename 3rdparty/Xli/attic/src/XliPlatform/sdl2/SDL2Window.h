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

#ifndef __XLI_PLATFORM_SPECIFIC_SDL2_WINDOW_H__
#define __XLI_PLATFORM_SPECIFIC_SDL2_WINDOW_H__

#include <XliPlatform/InputEventHandler.h>
#include <XliPlatform/Window.h>
#include <Xli/Shared.h>
#include <SDL.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        /**
            \ingroup XliPlatform
        */
        class SDL2Window: public Window
        {
        public:            
            Shared<InputEventHandler> eventHandler;
            SDL_Window* window;
            bool closed, fullscreen;
            int x, y, w, h, buttons;
            Vector2i keyboardSize;

            SDL2Window(int width, int height, const Xli::String& title, int flags);
            SDL2Window(const void* nativeHandle);
            virtual ~SDL2Window();

            virtual void SDL2WindowInit();

            virtual WindowImplementation GetImplementation();

            virtual void SetEventHandler(InputEventHandler* handler);
            virtual InputEventHandler* GetEventHandler();

            virtual bool IsClosed();
            virtual bool IsVisible();
            virtual bool IsFullscreen();
            virtual bool IsMinimized();
            virtual bool IsMaximized();

            virtual int GetDisplayIndex();
            virtual String GetTitle();
            virtual Vector2i GetPosition();
            virtual Vector2i GetClientSize();
            virtual void* GetNativeHandle();

            virtual void SetTitle(const String& title);
            virtual void SetFullscreen(bool fullscreen);
            virtual void SetPosition(Vector2i pos);
            virtual void SetClientSize(Vector2i size);

            virtual void Close();

            virtual void Minimize();
            virtual void Maximize();
            virtual void Restore();

            virtual bool GetKeyState(Key key);
            virtual bool GetMouseButtonState(MouseButton button);
            virtual Vector2i GetMousePosition();
            virtual void SetMousePosition(Vector2i position);

            virtual SystemCursor GetSystemCursor();
            virtual void SetSystemCursor(SystemCursor cursor);

            virtual void BeginTextInput(TextInputHint hint);
            virtual void EndTextInput();
            virtual bool IsTextInputActive();
            
            virtual bool HasOnscreenKeyboardSupport();
            virtual bool IsOnscreenKeyboardVisible();

            virtual void SetOnscreenKeyboardPosition(Vector2i position);
            virtual Vector2i GetOnscreenKeyboardPosition();
            virtual Vector2i GetOnscreenKeyboardSize();

            virtual bool IsStatusBarVisible();
            virtual Vector2i GetStatusBarPosition();
            virtual Vector2i GetStatusBarSize();
        };
    }
}


#endif
