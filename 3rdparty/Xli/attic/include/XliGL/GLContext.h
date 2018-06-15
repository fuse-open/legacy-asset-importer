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

#ifndef __XLI_GL_GL_CONTEXT_H__
#define __XLI_GL_GL_CONTEXT_H__

#include <Xli/Object.h>
#include <Xli/Vector4.h>

namespace Xli
{
    class Window;

    /**
        \ingroup XliGL
     */
    class GLContextAttributes
    {
    public:
        Vector4i ColorBits;
        int DepthBits;
        int StencilBits;
        int Samples;
        Vector4i AccumBits;
        int Buffers;
        bool Stereo;

        GLContextAttributes(const Vector4i& colorBits, int depthBits, int stencilBits, int samples, const Vector4i& accumBits, int buffers, bool stereo);

        static const GLContextAttributes& Default();
    };

    /**
        \ingroup XliGL
     */
    class GLContext: public Object
    {
    public:
        static GLContext* Create(Window* window, const GLContextAttributes& attribs);

        virtual GLContext* CreateSharedContext() = 0;

        virtual void GetAttributes(GLContextAttributes& result) = 0;
        virtual Vector2i GetDrawableSize() = 0;

        virtual void MakeCurrent(Window* window) = 0;
        virtual bool IsCurrent() = 0;

        virtual void SwapBuffers() = 0;

        virtual void SetSwapInterval(int value) = 0;
        virtual int GetSwapInterval() = 0;
    };
}

#endif
