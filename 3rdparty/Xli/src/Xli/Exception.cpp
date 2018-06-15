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

#include <Xli/Exception.h>

namespace Xli
{
    Exception::Exception()
    {
        this->_func = "<unknown>";
        this->_line = 0;
    }

    Exception::Exception(const String& message)
    {
        this->_message = message;
        this->_func = "<unknown>";
        this->_line = 0;
    }

    Exception::Exception(const String& message, const char* func, int line)
    {
        this->_message = message;
        this->_func = func;
        this->_line = line;
    }

    Exception::~Exception() XLI_NOEXCEPT
    {
        // empty
    }

    const String& Exception::GetMessage() const XLI_NOEXCEPT
    {
        return _message;
    }

    const char* Exception::GetFunction() const XLI_NOEXCEPT
    {
        return _func;
    }

    int Exception::GetLine() const XLI_NOEXCEPT
    {
        return _line;
    }

    const char* Exception::what() const XLI_NOEXCEPT
    { 
        return "Xli::Exception"; 
    }
}
