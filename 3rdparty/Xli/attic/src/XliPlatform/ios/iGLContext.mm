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
#include <XliPlatform/PlatformSpecific/iOS.h>
#include <Xli/Shared.h>
#include "iGLContext.h"

namespace Xli
{
    namespace PlatformSpecific
    {
        void iGLContext::Initialize(CAEAGLLayer *layer)
        {
            layer_ = layer;
            context_
                = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

            layer_.opaque = YES;

            //  NOTE: Unretained backing -- the default.
            //  Differs from prior SDL implementation.
            layer_.drawableProperties = [NSDictionary
                dictionaryWithObject:kEAGLColorFormatRGBA8
                forKey:kEAGLDrawablePropertyColorFormat];
        }

        void iGLContext::AllocateBuffersAndMakeCurrent()
        {
            assert(renderBuffer_ == 0);
            assert(frameBuffer_ == 0);
            assert(depthStencilBuffer_ == 0);

            CGSize layerSize = layer_.bounds.size;
            size_.X = layerSize.width;
            size_.Y = layerSize.height;
            size_ *= layer_.contentsScale;

            [EAGLContext setCurrentContext:context_];

            glGenRenderbuffers(1, &renderBuffer_);
            glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer_);
            [context_ renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer_];

            glGenFramebuffers(1, &frameBuffer_);
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer_);
            glFramebufferRenderbuffer(
                GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER,
                renderBuffer_);

            glGenRenderbuffers(1, &depthStencilBuffer_);
            glBindRenderbuffer(GL_RENDERBUFFER, depthStencilBuffer_);
            glRenderbufferStorage(
                GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, size_.X, size_.Y);
            glFramebufferRenderbuffer(
                GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
                depthStencilBuffer_);
            glFramebufferRenderbuffer(
                GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                depthStencilBuffer_);

            glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer_);
        }

        void iGLContext::FreeBuffers()
        {
            glFinish();

            glDeleteRenderbuffers(1, &renderBuffer_);
            glDeleteFramebuffers(1, &frameBuffer_);
            glDeleteRenderbuffers(1, &depthStencilBuffer_);

            renderBuffer_ = 0;
            frameBuffer_ = 0;
            depthStencilBuffer_ = 0;
        }

        void iGLContext::Destroy()
        {
            if ([EAGLContext currentContext] == context_)
                [EAGLContext setCurrentContext:nil];

            [context_ release];

            layer_ = 0;
            context_ = 0;
        }

        void iGLContext::GetAttributes(Xli::GLContextAttributes& result)
        {
            result = Xli::GLContextAttributes::Default();
        }

        Xli::Vector2i iGLContext::GetDrawableSize()
        {
            return Xli::Vector2i(size_);
        }

        void iGLContext::MakeCurrent(Xli::Window *)
        {
            [EAGLContext setCurrentContext:context_];
            glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer_);
        }

        bool iGLContext::IsCurrent()
        {
            return context_ == [EAGLContext currentContext];
        }

        void iGLContext::SwapBuffers()
        {
            [context_ presentRenderbuffer:GL_RENDERBUFFER];
        }

        GLContext* iGLContext::CreateSharedContext()
        {
            // TODO: What does a shared context mean in this context?

            // EAGLContext has the concept of a sharegroup, allowing textures
            // and renderbuffers to be shared. Currently, we create a shared
            // group per context, but we also only create one context.

            XLI_THROW_NOT_SUPPORTED(XLI_FUNCTION);
        }

        void iGLContext::SetSwapInterval(int value)
        {
            // iOS v-syncs, regardless.
        }

        int iGLContext::GetSwapInterval()
        {
            return 1;
        }
    }

    GLContext* GLContext::Create(Window* wnd, const GLContextAttributes& attribs)
    {
        return new PlatformSpecific::iGLContext();
    }
}
