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

#ifndef __XLI_EXCEPTION_H__
#define __XLI_EXCEPTION_H__

#include <Xli/Config.h>
#include <Xli/String.h>
#include <exception>

#if XLI_PLATFORM_UNIX
# define XLI_EXPORT_EXCEPTION __attribute__((visibility("default")))
#else
# define XLI_EXPORT_EXCEPTION
#endif

namespace Xli
{
    /**
        \ingroup XliExceptions
    */
    class XLI_EXPORT_EXCEPTION Exception : public std::exception
    {
    public:
        String _message;
        const char* _func;
        int _line;

        Exception();
        Exception(const String& message);
        Exception(const String& message, const char* func, int line);
        
        virtual ~Exception() XLI_NOEXCEPT;
        virtual const char* what() const XLI_NOEXCEPT;

        const String& GetMessage() const XLI_NOEXCEPT;
        const char* GetFunction() const XLI_NOEXCEPT;
        int GetLine() const XLI_NOEXCEPT;
    };

    /**
        \addtogroup XliExceptions
        @{
    */

#define XLI_DECLARE_EXCEPTION_SUB(BaseClass, ClassName, Message) \
    class XLI_EXPORT_EXCEPTION ClassName: public BaseClass \
    { \
    public: \
        ClassName(const Xli::String& msg) { _message = ::Xli::String(Message) + ": " + msg; } \
        ClassName() { _message = Message; } \
    }

#define XLI_DECLARE_EXCEPTION(ClassName, Message)   XLI_DECLARE_EXCEPTION_SUB(Xli::Exception, ClassName, Message)
#define XLI_THROW_EX(ClassName, Message)            do { ClassName e(Message); e._func = XLI_FUNCTION; e._line = XLI_LINE; throw e; } while(0)
#define XLI_THROW_E(ClassName)                      do { ClassName e; e._func = XLI_FUNCTION; e._line = XLI_LINE; throw e; } while(0)
#define XLI_THROW(Message)                          XLI_THROW_EX(::Xli::Exception, Message)

    /** @} */

    /** \ingroup XliExceptions */
    XLI_DECLARE_EXCEPTION(IndexOutOfBoundsException, "Index out of bounds");
    /** \ingroup XliExceptions */
    XLI_DECLARE_EXCEPTION(NullPointerException, "Null pointer access attempt");
    /** \ingroup XliExceptions */
    XLI_DECLARE_EXCEPTION(BadDeleteException, "Attemt to delete object with refcount != 1");
    /** \ingroup XliExceptions */
    XLI_DECLARE_EXCEPTION(FormatException, "Invalid format");
    /** \ingroup XliExceptions */
    XLI_DECLARE_EXCEPTION(ArgumentException, "Illegal argument");
    /** \ingroup XliExceptions */
    XLI_DECLARE_EXCEPTION(NotSupportedException, "Not supported");

    /** \ingroup XliIO */
    class IOException: public Exception {};
    /** \ingroup XliIO */
    XLI_DECLARE_EXCEPTION_SUB(IOException, StreamClosedException, "Stream has closed");
    /** \ingroup XliIO */
    XLI_DECLARE_EXCEPTION_SUB(IOException, StreamCantReadException, "Stream not readable");
    /** \ingroup XliIO */
    XLI_DECLARE_EXCEPTION_SUB(IOException, StreamCantWriteException, "Stream not writeable");
    /** \ingroup XliIO */
    XLI_DECLARE_EXCEPTION_SUB(IOException, StreamCantSeekException, "Stream not seekable");
    /** \ingroup XliIO */
    XLI_DECLARE_EXCEPTION_SUB(IOException, EndOfStreamException, "End of Stream reached");
    /** \ingroup XliIO */
    XLI_DECLARE_EXCEPTION_SUB(IOException, FileCantOpenException, "Couldn't open file");
    /** \ingroup XliIO */
    XLI_DECLARE_EXCEPTION_SUB(IOException, FileNotFoundException, "File not found");
}

/**
    \addtogroup XliExceptions
    @{
*/

#define XLI_THROW_INDEX_OUT_OF_BOUNDS       XLI_THROW_E(::Xli::IndexOutOfBoundsException)
#define XLI_THROW_NULL_POINTER              XLI_THROW_E(::Xli::NullPointerException)
#define XLI_THROW_BAD_DELETE                XLI_THROW_E(::Xli::BadDeleteException)
#define XLI_THROW_INVALID_FORMAT(msg)       XLI_THROW_EX(::Xli::FormatException, msg)
#define XLI_THROW_INVALID_ARGUMENT(msg)     XLI_THROW_EX(::Xli::ArgumentException, msg)
#define XLI_THROW_NOT_SUPPORTED(msg)        XLI_THROW_EX(::Xli::NotSupportedException, msg)

#define XLI_THROW_STREAM_CLOSED             XLI_THROW_E(::Xli::StreamClosedException)
#define XLI_THROW_STREAM_CANT_READ          XLI_THROW_E(::Xli::StreamCantReadException)
#define XLI_THROW_STREAM_CANT_WRITE         XLI_THROW_E(::Xli::StreamCantWriteException)
#define XLI_THROW_STREAM_CANT_SEEK          XLI_THROW_E(::Xli::StreamCantSeekException)
#define XLI_THROW_END_OF_STREAM             XLI_THROW_E(::Xli::EndOfStreamException)
#define XLI_THROW_CANT_OPEN_FILE(msg)       XLI_THROW_EX(::Xli::FileCantOpenException, msg)
#define XLI_THROW_FILE_NOT_FOUND(msg)       XLI_THROW_EX(::Xli::FileNotFoundException, msg)

/** @} */

#endif
