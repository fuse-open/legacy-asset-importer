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

#ifndef __XLI_MEDIA_IMAGE_H__
#define __XLI_MEDIA_IMAGE_H__

#include <Xli/Buffer.h>
#include <XliImage/Format.h>

namespace Xli
{
    class Bitmap;

    /**
        \ingroup XliImage
    */
    class Image: public DataAccessor
    {
    public:
        virtual int GetWidth() const = 0;
        virtual int GetHeight() const = 0;
        virtual int GetDepth() const = 0;
        virtual Format GetFormat() const = 0;
        virtual Bitmap* ToBitmap() = 0;

        virtual int GetSizeInBytes() const = 0;
        virtual const UInt8* GetPtr() const = 0;
    };
}

#endif
