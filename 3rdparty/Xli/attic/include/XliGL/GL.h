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

#ifndef __XLI_GL_GL_H__
#define __XLI_GL_GL_H__

#include <Xli/Config.h>

#if defined(XLI_PLATFORM_WIN32)
# ifndef GLEW_STATIC
#   define GLEW_STATIC
# endif
# include <XliGL/3rdparty/glew.h>
# include <GL/gl.h>
# define XLI_GL_DESKTOP

#elif defined(XLI_PLATFORM_ANDROID)
# include <GLES2/gl2.h>
# include <GLES2/gl2ext.h>
# define XLI_GL_ES2

#elif defined(XLI_PLATFORM_IOS)
# include <OpenGLES/ES2/gl.h>
# include <OpenGLES/ES2/glext.h>
# define XLI_GL_ES2

#elif defined(XLI_PLATFORM_OSX)
# ifndef GLEW_STATIC
#   define GLEW_STATIC
# endif
# include <XliGL/3rdparty/glew.h>
# include <OpenGL/gl.h>
# define XLI_GL_DESKTOP

#elif defined(XLI_PLATFORM_LINUX)
# ifdef __arm__
#  include <GLES2/gl2.h>
#  include <GLES2/gl2ext.h>
#  define XLI_GL_ES2
# else
#  include <GL/glew.h>
#  include <GL/gl.h>
#  define XLI_GL_DESKTOP
# endif

#else
#error "Unsupported platform"

#endif


#endif
