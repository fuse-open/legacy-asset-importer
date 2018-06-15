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

#include <XliGL.h>
#include <XliPlatform/Window.h>
#include <Xli/Console.h>
#include <Xli/Shared.h>

#include <android/native_window.h>
#include <EGL/egl.h>
#include <stdlib.h>

#include "EGLContext.h"
#include "AWindow.h"

namespace Xli
{
    namespace PlatformSpecific
    {
        AGLContext::~AGLContext()
        {
            Destroy();
        }

        void AGLContext::Initialize(const GLContextAttributes& attribs)
        {
            swapInterval = -1;
            handle = NULL;
            context = EGL_NO_CONTEXT;
            surface = EGL_NO_SURFACE;

            display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
            eglInitialize(display, 0, 0);

            const EGLint iattribs[] =
                {
                    EGL_RED_SIZE, 5,
                    EGL_GREEN_SIZE, 6,
                    EGL_BLUE_SIZE, 5,
                    EGL_ALPHA_SIZE, 0,
                    EGL_DEPTH_SIZE, 16,
                    EGL_STENCIL_SIZE, 0,
                    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                    //EGL_RENDER_BUFFER, attribs.Buffers <= 1 ? EGL_SINGLE_BUFFER : EGL_BACK_BUFFER,
                    EGL_NONE
                };

            EGLint numConfigs;
            EGLConfig configs[128];

            if (!eglChooseConfig(display, iattribs, configs, 128, &numConfigs) || numConfigs == 0)
                XLI_THROW("Unable to choose EGL config");

            EGLint cs = 0, cd = 0, cb = 0;
            int cc = 0;

            for (int i = 0; i < numConfigs; i++)
            {
                EGLint samples, depth, stencil, buffer, r, g, b, a, render;
                eglGetConfigAttrib(display, configs[i], EGL_RED_SIZE, &r);
                eglGetConfigAttrib(display, configs[i], EGL_GREEN_SIZE, &g);
                eglGetConfigAttrib(display, configs[i], EGL_BLUE_SIZE, &b);
                eglGetConfigAttrib(display, configs[i], EGL_ALPHA_SIZE, &a);
                eglGetConfigAttrib(display, configs[i], EGL_BUFFER_SIZE, &buffer);
                eglGetConfigAttrib(display, configs[i], EGL_DEPTH_SIZE, &depth);
                eglGetConfigAttrib(display, configs[i], EGL_STENCIL_SIZE, &stencil);
                eglGetConfigAttrib(display, configs[i], EGL_SAMPLES, &samples);

#ifdef XLI_DEBUG
                Error->WriteLine(String::Format("DEBUG: EGLConfig[%d]:  M %d  D %d  S %d  B %d  R %d  G %d  B %d  A %d", i, samples, depth, stencil, buffer, r, g, b, a));
#endif

                if (samples >= cs && depth >= cd && buffer >= cb &&
                    samples <= attribs.Samples && r <= attribs.ColorBits.R && g <= attribs.ColorBits.G && b <= attribs.ColorBits.B && a <= attribs.ColorBits.A)
                {
                    cs = samples;
                    cd = depth;
                    cb = buffer;
                    cc = i;
                }
            }

            config = configs[cc];

#ifdef XLI_DEBUG
            Error->WriteLine((String)"DEBUG: Selected EGLConfig[" + (int)cc + "]");
#endif
        }

        void AGLContext::Destroy()
        {
            if (display != EGL_NO_DISPLAY)
            {
                eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

                if (context != EGL_NO_CONTEXT)
                    eglDestroyContext(display, context);

                if (surface != EGL_NO_SURFACE)
                    eglDestroySurface(display, surface);

                eglTerminate(display);
            }
        }

        GLContext* AGLContext::CreateSharedContext()
        {
            XLI_THROW_NOT_SUPPORTED(XLI_FUNCTION);
        }

        void AGLContext::MakeCurrent(Window* wnd)
        {
            if (wnd)
                window = wnd;

            if (wnd && (ANativeWindow*)wnd->GetNativeHandle() != handle) //{TODO} cache handle?
            {
                if (((AWindow*)wnd)->GetImplementation() == WindowImplementationAndroid) //{TODO} cast is ugly hack, fix
                {
                    EGLint format;
                    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
                    ANativeWindow_setBuffersGeometry((ANativeWindow*)wnd->GetNativeHandle(), 0, 0, format);
                }

                if (surface != EGL_NO_SURFACE)
                    eglDestroySurface(display, surface);

                handle = (ANativeWindow*)wnd->GetNativeHandle();
                surface = eglCreateWindowSurface(display, config, handle, NULL);

                if (surface == EGL_NO_SURFACE)
                    XLI_THROW("Unable to create EGL Surface");

                if (context == EGL_NO_CONTEXT)
                {
                    const EGLint context_attribs[] =
                        {
                            EGL_CONTEXT_CLIENT_VERSION, 2,
                            EGL_NONE
                        };

                    context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attribs);

                    if (context == EGL_NO_CONTEXT)
                        XLI_THROW("Unable to create EGL Context");
                }
            }

            if (eglMakeCurrent(display, surface, surface, wnd != 0 ? context : 0) == EGL_FALSE)
            {
                XLI_THROW("Unable to make EGL context current");
            }
        }

        bool AGLContext::IsCurrent()
        {
            return eglGetCurrentContext() == context;
        }

        void AGLContext::SwapBuffers()
        {
            eglSwapBuffers(display, surface);
        }

        void AGLContext::SetSwapInterval(int value)
        {
            if (eglSwapInterval(display, value))
                swapInterval = value;
        }

        int AGLContext::GetSwapInterval()
        {
            return swapInterval;
        }

        Vector2i AGLContext::GetDrawableSize()
        {
            return window->GetClientSize();
        }

        void AGLContext::GetAttributes(GLContextAttributes& result)
        {
            memset(&result, 0, sizeof(GLContextAttributes));
            eglGetConfigAttrib(display, config, EGL_RED_SIZE, &result.ColorBits.R);
            eglGetConfigAttrib(display, config, EGL_GREEN_SIZE, &result.ColorBits.G);
            eglGetConfigAttrib(display, config, EGL_BLUE_SIZE, &result.ColorBits.B);
            eglGetConfigAttrib(display, config, EGL_ALPHA_SIZE, &result.ColorBits.A);
            eglGetConfigAttrib(display, config, EGL_DEPTH_SIZE, &result.DepthBits);
            eglGetConfigAttrib(display, config, EGL_STENCIL_SIZE, &result.StencilBits);
            eglGetConfigAttrib(display, config, EGL_SAMPLES, &result.Samples);
/*
  EGLint renderBuffer;
  eglGetConfigAttrib(display, config, EGL_RENDER_BUFFER, &renderBuffer);
  result.Buffers = renderBuffer == EGL_SINGLE_BUFFER ? 1 : 2;
*/
            result.Buffers = 2;
        }
    }
    GLContext* GLContext::Create(Window* window, const GLContextAttributes& attribs)
    {
        return new PlatformSpecific::AGLContext();
    }
}
