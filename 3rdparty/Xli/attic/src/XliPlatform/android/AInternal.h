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

#ifndef __XLI_PLATFORM_SPECIFIC_ANDROID_INTERNAL_H__
#define __XLI_PLATFORM_SPECIFIC_ANDROID_INTERNAL_H__

#include <android/log.h>
#include <android/native_activity.h>
#include <cstdio>
#include <cstdlib>
#include <jni.h>

#include <XliPlatform/PlatformSpecific/Android.h>
#include <XliPlatform/Application.h>
#include <XliPlatform/Window.h>
#include "AKeyEvent.h"

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, Xli::PlatformSpecific::AGetAppName(), __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, Xli::PlatformSpecific::AGetAppName(), __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, Xli::PlatformSpecific::AGetAppName(), __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, Xli::PlatformSpecific::AGetAppName(), __VA_ARGS__))
#define LOGF(...) ((void)__android_log_print(ANDROID_LOG_FATAL, Xli::PlatformSpecific::AGetAppName(), __VA_ARGS__))

struct android_app;

namespace Xli
{
    typedef int JObjRef;

    namespace PlatformSpecific
    {
        extern ANativeActivity* AndroidActivity;
 
        XLI_INLINE const char* AGetAppName()
        {
            const char* name = getenv("XLI_APP_NAME");
            return name ? name : "XliApp";
        }

        class CTAction : public Object
        {
        public:
            virtual void Execute() = 0;
        };

        class CTKeyAction : public CTAction
        {
        public:
            Xli::Key KeyEvent;
            bool KeyDown;
            CTKeyAction(AKeyEvent keyEvent, bool keyDown)
            {
                this->KeyEvent = AndroidToXliKeyEvent(keyEvent);
                this->KeyDown = keyDown;
            }
            void Execute()
            {
                Application* app = Xli::Application::SharedApp();
                Window* win = app->RootWindow();
                if (this->KeyDown)
                {
                    app->OnKeyDown(win, this->KeyEvent);
                } else {
                    app->OnKeyUp(win, this->KeyEvent);
                }
            }
        };
        
        class CTTextAction : public CTAction
        {
        public:
            String Text;
            CTTextAction(String text) { this->Text = text; }
            void Execute()
            {
                Application* app = Xli::Application::SharedApp();
                Window* win = app->RootWindow();
                app->OnTextInput(win, this->Text);
            }
        };
        
        class CTKeyboardResize : public CTAction
        {
        public:
            CTKeyboardResize() {}
            void Execute()
            {
                Application* app = Xli::Application::SharedApp();
                Window* win = app->RootWindow();
                app->OnKeyboardResized(win);
            }
        };        

        class CTTouchEvent : public CTAction
        {
        public:
            int id;
            int x;
            int y;
            int type;
            CTTouchEvent(int pointerID, int x, int y, int type)
            {
                this->id = pointerID;
                this->x = x;
                this->y = y;
                this->type = type;
            }
            virtual void Execute()
            {
                Application* app = Xli::Application::SharedApp();
                Window* win = app->RootWindow();
                switch (type)
                {
                case 0:                    
                    app->OnTouchMove(win, Vector2(x, y), id);
                    break;
                case 1:
                    app->OnTouchDown(win, Vector2(x, y), id);
                    break;
                case 2:
                    app->OnTouchUp(win, Vector2(x, y), id);
                    break;
                }
                
            }
        };
    
        class CTSurfaceSizeChanged : public CTAction
        {
        public:
            virtual void Execute()
            {
                Application* app = Xli::Application::SharedApp();
                app->OnSizeChanged(app->RootWindow());
            }
        };
        
        class CTSurfaceReady : public CTAction
        {
        public:
            virtual void Execute()
            {
                // give to context via window
                Application* app = Application::SharedApp();
                Window* window = app->RootWindow();
                app->OnNativeHandleChanged(window);
                window->Show();
            }
        };
        
        class CTError : public CTAction
        {
        public:
            String message;
            int errorCode;
            CTError(String message, int errorCode)
            {
                this->message = message;
                this->errorCode = errorCode;
            }
            virtual void Execute()
            {
                String finalMessage = "XLiError (" + String(errorCode)+ ") - " + message;
                XLI_THROW(finalMessage.Ptr());
            }
        };
    }
}


#endif
