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

#ifndef __XLI_MEDIA_JSON_H__
#define __XLI_MEDIA_JSON_H__

#include <Xli/File.h>
#include <Xli/TextReader.h>
#include <XliJson/Value.h>

namespace Xli
{
    /**
        \ingroup XliJson
    */
    class Json
    {
    public:
        /**
            Formats a string to be outputted to a JSON document.
            Converts to Utf8, adds quotes and escapes special characters.
        */
        static String ToStringLiteral(const String& str);

        static Value Parse(const String& code, bool preserveOrder = false);

        static XLI_INLINE Value Load(Stream* stream, bool preserveOrder = false)
        {
            return Parse(TextReader(stream).ReadAll(), preserveOrder);
        }

        static XLI_INLINE Value Load(const String& filename, bool preserveOrder = false)
        {
            File f(filename, FileModeRead);
            return Load(&f, preserveOrder);
        }

        static void Save(Stream* stream, const Value& value);

        static XLI_INLINE void Save(const String& filename, const Value& value)
        {
            File f(filename, FileModeWrite);
            Save(&f, value);
        }

        static String Stringify(const Value& value);
    };
}

#endif
