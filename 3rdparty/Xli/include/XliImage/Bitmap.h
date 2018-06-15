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

#ifndef __XLI_MEDIA_BITMAP_H__
#define __XLI_MEDIA_BITMAP_H__

#include <Xli/Buffer.h>
#include <Xli/Vector2.h>
#include <Xli/Vector4.h>
#include <Xli/Rectangle.h>
#include <XliImage/Image.h>

namespace Xli
{
    /**
        \ingroup XliImage
    */
    enum RotateAngle
    {
        RotateAngle90CW,
        RotateAngle90CCW,
        RotateAngle180,
    };

    /**
        \ingroup XliImage
    */
    class Bitmap: public Image
    {
        Format format;
        int width, height, pitch;
        DataAccessor* dataOwner;
        UInt8* data;

    public:
        Bitmap(int width, int height, Format format);
        Bitmap(int width, int height, Format format, DataAccessor* data, int pitch, int offset);
        virtual ~Bitmap();

        Format GetFormat() const;
        int GetWidth() const;
        int GetHeight() const;
        int GetDepth() const;
        int GetPitch() const;
        int GetComponentCount() const;
        int GetBytesPerPixel() const;
        Recti GetRect() const;

        Vector4u8 GetPixelColor(int x, int y);
        void SetPixelColor(int x, int y, const Vector4u8& color);

        /**
            Returns a version of this bitmap where pixels are packed (pitch = pixelsize*width).
            If this bitmap is already packed, then a new reference is added and this is returned.
        */
        Bitmap* Pack();
        Bitmap* ToBitmap();
        Bitmap* SubBitmap(const Recti& rect);
        Bitmap* ConvertTo(Format format);
        Bitmap* Rotate(RotateAngle angle);

        Bitmap* DownSample2x2();
        void GenerateMipMaps(Array<Bitmap*>& chain);

        void Clear();
        void Blit(Bitmap* src, Recti srcRect, Vector2i dstPos);

        virtual int GetSizeInBytes() const;
        
        UInt8* GetPtr();
        virtual const UInt8* GetPtr() const;

        UInt8* GetScanlinePtr(int y);
        const UInt8* GetScanlinePtr(int y) const;

        UInt8* GetPixelPtr(int x, int y);
        const UInt8* GetPixelPtr(int x, int y) const;

        DataAccessor* GetDataAccessor();
    };
}

#endif
