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

#ifndef __XLI_PATH_H__
#define __XLI_PATH_H__

#include <Xli/String.h>

namespace Xli
{
    /**
        \ingroup XliIO
    */
    class Path
    {
    public:
        static String Combine(const String& path1, const String& path2);

        static String GetFilename(const String& path);
        static String GetFilenameWithoutExtension(const String& path);

        /**
            Returns the extension part of the path, including the prefixed dot. An empty string is returned if the path has no extension part
        */
        static String GetExtension(const String& path);

        /**
            Returns the directory part of the path, without the trailing slash. '.' is returned if the path has no directory part
        */
        static String GetDirectoryName(const String& path);
    };
}


#endif
