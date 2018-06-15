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

#ifndef __XLI_PLATFORM_SPECIFIC_IOS_WINDOW_H__
#define __XLI_PLATFORM_SPECIFIC_IOS_WINDOW_H__

#include <XliPlatform/InputEventHandler.h>
#include <XliPlatform/Window.h>
#include <Xli/Shared.h>
#include <XliGL/GLContext.h>
#include "iGLContext.h"

namespace Xli
{
    namespace PlatformSpecific
    {
        class iWindow : public Window
        {
        public:
            iWindow()
            {
                window_ = 0;
            }

            void OnDraw();

            String GetTitle();
            Vector2i GetClientSize();
            Vector2i GetPosition();
            WindowImplementation GetImplementation();

            bool IsClosed();
            bool IsFullscreen();
            bool IsMaximized();
            bool IsMinimized();

            int GetDisplayIndex();

            void Close();
            void Maximize();
            void Minimize();
            void Restore();

            void SetClientSize(Vector2i size);
            void SetFullscreen(bool fullscreen);
            void SetPosition(Vector2i pos);
            void SetTitle(const String& title);

            void* GetNativeHandle();
            GLContext* GetContext();

        protected:
            void OnInitialize();
            void OnShow();
            void OnHide();
            void OnDestroy();

        private:
            iGLContext context_;
            UIWindow* window_;
        };
    }
}


#endif
