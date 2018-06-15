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

#ifndef __XLI_ZIP_H__
#define __XLI_ZIP_H__

#include <Xli/Object.h>
#include <Xli/FileSystem.h>
#include <Xli/File.h>

namespace Xli
{
    /**
        \ingroup XliZip
    */
    class Zip
    {
        static FileSystem* Open(Stream* file, const char* password);

    public:
        static XLI_INLINE FileSystem* Open(Stream* file) 
        { 
            return Open(file, 0); 
        }
        
        static XLI_INLINE FileSystem* Open(Stream* file, const String& password) 
        { 
            return Open(file, password.Ptr()); 
        }
        
        static XLI_INLINE FileSystem* Open(const String& filename) 
        { 
            Managed<File> f = new File(filename, FileModeRead); 
            return Open(f); 
        }
        
        static XLI_INLINE FileSystem* Open(const String& filename, const String& password) 
        { 
            Managed<File> f = new File(filename, FileModeRead); 
            return Open(f, password.Ptr()); 
        }
    };
}

#endif
