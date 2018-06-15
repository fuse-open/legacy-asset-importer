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

#ifndef __XLI_CONFIG_H__
#define __XLI_CONFIG_H__

#if defined(WIN32)
# define XLI_PLATFORM_WIN32 1
# if defined(_MSC_VER) && (_MSC_VER >= 1400)
#  define XLI_COMPILER_MSVC
# endif

#elif defined(ANDROID)
# define XLI_PLATFORM_ANDROID 1

#elif defined(__APPLE__)
# include <TargetConditionals.h>
# if TARGET_OS_IPHONE
#   define XLI_PLATFORM_IOS 1
# else
#   define XLI_PLATFORM_OSX 1
# endif

#elif defined(__linux)
# define XLI_PLATFORM_LINUX 1

#endif

#if !XLI_PLATFORM_WIN32
# define XLI_PLATFORM_UNIX 1
#endif

#ifndef XLI_DEBUG
# ifndef NDEBUG
#   define XLI_DEBUG
# endif
#endif

#ifdef XLI_DEBUG
# define XLI_RANGE_CHECK 1
# define XLI_NULL_POINTER_CHECK 1
#endif

#define XLI_INLINE inline
#define XLI_NOEXCEPT throw()

#define XLI_FILE __FILE__
#define XLI_LINE __LINE__

#ifdef XLI_COMPILER_MSVC // Visual C++ specific
# define XLI_FUNCTION __FUNCTION__ //__FUNCSIG__
# define XLI_NORETURN __declspec(noreturn)
#else
# define XLI_FUNCTION __PRETTY_FUNCTION__
# define XLI_NORETURN __attribute__((noreturn))
#endif

#endif
