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

#ifndef __XLI_GL_EGL_CONTEXT_H__
#define __XLI_GL_EGL_CONTEXT_H__

#include <Xli/Vector4.h>
#include <Xli/Object.h>
#include <Xli/Shared.h>
#include <XliGL.h>

#include <android/native_window.h>
#include <EGL/egl.h>
#include <stdlib.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        class AGLContext : public Xli::GLContext
        {
        public:
            AGLContext() {}
            virtual ~AGLContext();

            void Initialize(const GLContextAttributes& attribs);

            virtual GLContext* CreateSharedContext();

            virtual void GetAttributes(GLContextAttributes& result);
            virtual Vector2i GetDrawableSize();

            virtual void MakeCurrent(Window* window);
            virtual bool IsCurrent();
            virtual void Destroy();
            
            virtual void SwapBuffers();

            virtual void SetSwapInterval(int value);
            virtual int GetSwapInterval();
        private:
            Shared<Window> window;
            ANativeWindow* handle;
            EGLDisplay display;
            EGLSurface surface;
            EGLContext context;
            EGLConfig config;
            int swapInterval;
        };
    }
}

#endif
