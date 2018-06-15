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

#include <XliImage/Bitmap.h>
#include <Xli/Shared.h>
#include <Xli/Array.h>
#include <cstring>

namespace Xli
{
    Bitmap::Bitmap(int width, int height, Format format)
    {
        this->width = width;
        this->height = height;
        this->format = format;
        this->pitch = width * FormatInfo::SizeOf(format);
        this->dataOwner = Buffer::Create(pitch * height);
        this->data = const_cast<UInt8*>(dataOwner->GetPtr());
    }

    Bitmap::Bitmap(int width, int height, Format format, DataAccessor* dataAccessor, int pitch, int offset)
    {
        if (!dataAccessor) XLI_THROW_NULL_POINTER;

        this->width = width;
        this->height = height;
        this->format = format;
        this->pitch = pitch;
        this->dataOwner = dataAccessor;
        this->dataOwner->AddRef();
        this->data = const_cast<UInt8*>(dataOwner->GetPtr()) + offset;
    }

    Bitmap::~Bitmap()
    {
        dataOwner->Release();
    }

    Format Bitmap::GetFormat() const
    {
        return format;
    }

    int Bitmap::GetWidth() const
    {
        return width;
    }

    int Bitmap::GetHeight() const
    {
        return height;
    }

    int Bitmap::GetDepth() const
    {
        return 0;
    }

    int Bitmap::GetPitch() const
    {
        return pitch;
    }

    int Bitmap::GetComponentCount() const
    {
        return FormatInfo::ComponentCount(format);
    }

    int Bitmap::GetBytesPerPixel() const
    {
        return FormatInfo::SizeOf(format);
    }

    Recti Bitmap::GetRect() const
    {
        return Recti(0, 0, width, height);
    }

    int Bitmap::GetSizeInBytes() const
    {
        return height * pitch;
    }

    UInt8* Bitmap::GetPtr()
    {
        return data;
    }

    const UInt8* Bitmap::GetPtr() const
    {
        return data;
    }

    UInt8* Bitmap::GetScanlinePtr(int y)
    {
        return data + pitch*y;
    }

    const UInt8* Bitmap::GetScanlinePtr(int y) const
    {
        return data + pitch*y;
    }

    UInt8* Bitmap::GetPixelPtr(int x, int y)
    {
        return data + pitch * y + GetBytesPerPixel() * x;
    }

    const UInt8* Bitmap::GetPixelPtr(int x, int y) const
    {
        return data + pitch * y + GetBytesPerPixel() * x;
    }

    DataAccessor* Bitmap::GetDataAccessor()
    {
        return dataOwner;
    }

    Bitmap* Bitmap::Pack()
    {
        if (pitch == FormatInfo::SizeOf(format) * width)
            return SharePtr(this);

        // TODO: Implement this
        XLI_THROW_NOT_SUPPORTED(XLI_FUNCTION);
    }

    Bitmap* Bitmap::ToBitmap()
    {
        return SharePtr(this);
    }

    Bitmap* Bitmap::SubBitmap(const Recti& rect)
    {
        if (rect.Left >= 0 && rect.Right <= width && rect.Top >= 0 && rect.Bottom <= height)
        {
            int offset = rect.Top * pitch + rect.Left * GetBytesPerPixel() + (int)(dataOwner->GetPtr() - data);
            return new Bitmap(rect.Width(), rect.Height(), format, dataOwner, pitch, offset);
        }

        XLI_THROW_INDEX_OUT_OF_BOUNDS;
    }

    Vector4u8 Bitmap::GetPixelColor(int x, int y)
    {
        switch (format)
        {
        case FormatRGBA_8_8_8_8_UInt_Normalize: 
            return *(Vector4u8*)GetPixelPtr(x, y);

        case FormatRGB_8_8_8_UInt_Normalize: 
            return Vector4u8(Vector3u8(*(Vector3u8*)GetPixelPtr(x, y)), (UInt8)0xff);

        default: 
            XLI_THROW("Bitmap::GetPixelColor: unsupported pixel format");
        }
    }

    void Bitmap::SetPixelColor(int x, int y, const Vector4u8& color)
    {
        switch (format)
        {
        case FormatRGBA_8_8_8_8_UInt_Normalize: 
            *(Vector4u8*)GetPixelPtr(x,y) = color;
            break;

        case FormatRGB_8_8_8_UInt_Normalize: 
            *(Vector3u8*)GetPixelPtr(x, y) = color.XYZ();
            break;

        default: 
            XLI_THROW("Bitmap::SetPixelColor: unsupported pixel format");
        }
    }
    
    Bitmap* Bitmap::ConvertTo(Format dstFormat)
    {
        if (dstFormat == this->format)
        {
            return SharePtr(this);
        }
        else if (this->format == FormatL_8_UInt_Normalize && dstFormat == FormatRGBA_8_8_8_8_UInt_Normalize)
        {
            Bitmap* bmp = new Bitmap(width, height, FormatRGBA_8_8_8_8_UInt_Normalize);
            Vector4u8* dst = (Vector4u8*)bmp->GetPtr();

            for (int y = 0; y < height; y++)
            {
                UInt8* src = GetScanlinePtr(y);

                for (int x = 0; x < width; x++)
                {
                    UInt8 c = *src++;
                    (*dst).R = c;
                    (*dst).G = c;
                    (*dst).B = c;
                    (*dst).A = (UInt8)0xff;
                    dst++;
                }
            }

            return bmp;
        }
        else if (this->format == FormatL_8_UInt_Normalize && dstFormat == FormatRGB_8_8_8_UInt_Normalize)
        {
            Bitmap* bmp = new Bitmap(width, height, FormatRGB_8_8_8_UInt_Normalize);
            UInt8* dst = bmp->GetPtr();

            for (int y = 0; y < height; y++)
            {
                UInt8* src = GetScanlinePtr(y);

                for (int x = 0; x < width; x++)
                {
                    UInt8 c = *src++;
                    *dst++ = c;
                    *dst++ = c;
                    *dst++ = c;
                }
            }

            return bmp;
        }
        else if (this->format == FormatRGB_8_8_8_UInt_Normalize && dstFormat == FormatRGBA_8_8_8_8_UInt_Normalize)
        {
            Bitmap* bmp = new Bitmap(width, height, FormatRGBA_8_8_8_8_UInt_Normalize);
            Vector4u8* dst = (Vector4u8*)bmp->GetPtr();

            for (int y = 0; y < height; y++)
            {
                UInt8* src = GetScanlinePtr(y);

                for (int x = 0; x < width; x++)
                {
                    (*dst).R = *src++;
                    (*dst).G = *src++;
                    (*dst).B = *src++;
                    (*dst).A = (UInt8)0xff;
                    dst++;
                }
            }

            return bmp;
        }
        else if (this->format == FormatRGBA_8_8_8_8_UInt_Normalize && dstFormat == FormatRGB_8_8_8_UInt_Normalize)
        {
            Bitmap* bmp = new Bitmap(width, height, FormatRGB_8_8_8_UInt_Normalize);
            UInt8* dst = bmp->GetPtr();

            for (int y = 0; y < height; y++)
            {
                UInt8* src = GetScanlinePtr(y);

                for (int x = 0; x < width; x++)
                {
                    *dst++ = *src++;
                    *dst++ = *src++;
                    *dst++ = *src++;
                    src++;
                }
            }

            return bmp;
        }

        XLI_THROW(String("Unable to convert ") + FormatInfo::ToString(this->format) + " to " + FormatInfo::ToString(dstFormat));
    }

    Bitmap* Bitmap::Rotate(RotateAngle angle)
    {
        Bitmap* target = new Bitmap(width, height, format);

        int bpp = GetBytesPerPixel();

        if (angle == RotateAngle90CW || angle == RotateAngle90CCW)
        {
            target->width = height;
            target->height = width;
            target->pitch = target->width * bpp;
        }

        int dstDeltaX;
        int dstDeltaY;
        int dstOffset;

        switch (angle)
        {
        case RotateAngle90CW:
            dstDeltaX = height * bpp;
            dstDeltaY = -(width * height + 1) * bpp;
            dstOffset = (height - 1) * bpp;
            break;

        case RotateAngle90CCW:
            dstDeltaX = -height * bpp;
            dstDeltaY = (width * height + 1) * bpp;
            dstOffset = (width - 1) * height * bpp;
            break;

        case RotateAngle180:
            dstDeltaX = -bpp;      
            dstDeltaY = 0;
            dstOffset = (width * height - 1) * bpp;
            break;

        default:
            XLI_THROW("Can't Rotate: Invalid RotateAngle");
        } 

        UInt8* src = data;
        UInt8* dst = target->data + dstOffset;
        int srcDeltaY = (pitch - width*bpp);

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                memcpy(dst, src, bpp);
                dst += dstDeltaX;
                src += bpp;
            }

            dst += dstDeltaY;
            src += srcDeltaY;
        }

        return target;
    }

    void Bitmap::Clear()
    {
        memset(data, 0, pitch * height);
    }

    void Bitmap::Blit(Bitmap* src, Recti srcRect, Vector2i dstPos)
    {
        if (srcRect.Left < 0) srcRect.Left = 0;
        if (srcRect.Top < 0) srcRect.Top = 0;
        if (srcRect.Right > src->GetWidth()) srcRect.Right = src->GetWidth();
        if (srcRect.Bottom > src->GetHeight()) srcRect.Bottom = src->GetHeight();

        if (dstPos.X < 0) dstPos.X = 0;
        if (dstPos.Y < 0) dstPos.Y = 0;
        if (dstPos.X + srcRect.Width() > GetWidth()) srcRect.Right = GetWidth() - dstPos.X + srcRect.Left;
        if (dstPos.Y + srcRect.Height() > GetHeight()) srcRect.Bottom = GetHeight() - dstPos.Y + srcRect.Top;

        Managed<Bitmap> temp = src->ConvertTo(format);

        int srcOffset = temp->pitch * srcRect.Top + GetBytesPerPixel() * srcRect.Left;
        int dstOffset = pitch * dstPos.Y + GetBytesPerPixel() * dstPos.X;
        int scanSize = srcRect.Width() * GetBytesPerPixel();

        UInt8* srcData = temp->data + srcOffset;
        UInt8* dstData = data + dstOffset;

        for (int i = 0; i < srcRect.Height(); i++)
        {
            memcpy(dstData, srcData, scanSize);
            srcData += temp->pitch;
            dstData += pitch;
        }
    }

    Bitmap* Bitmap::DownSample2x2()
    {
        int cc = GetComponentCount();
        int bpp = GetBytesPerPixel();

        if (cc != bpp)
        {
            XLI_THROW("Can't DownSample2x2: Unsupported pixel format");
        }

        int w = width >> 1; if (w < 1) w = 1;
        int h = height >> 1; if (h < 1) h = 1;

        Bitmap* target = new Bitmap(w, h, format);
        UInt8* dst = target->data;

        for (int y = 0; y < h; y++)
        {
            int dy = 1; if (y*2+1 >= height) dy = 0;
            UInt8* scan0 = GetScanlinePtr(y*2);
            UInt8* scan1 = GetScanlinePtr(y*2+dy);

            for (int x = 0; x < w; x++)
            {
                int dx = 1; if (x*2+1 >= width) dx = 0;
                    
                UInt8* src0 = scan0 + (x*2)*cc;
                UInt8* src1 = scan0 + (x*2+dx)*cc;
                UInt8* src2 = scan1 + (x*2)*cc;
                UInt8* src3 = scan1 + (x*2+dx)*cc;

                for (int c = 0; c < cc; c++)
                {    
                    *dst++ = (UInt8)(((unsigned int)*src0 + *src1 + *src2 + *src3) >> 2);
                    src0++;
                    src1++;
                    src2++;
                    src3++;
                }
            }
        }

        return target;
    }

    void Bitmap::GenerateMipMaps(Array<Bitmap*>& chain)
    {
        chain.Add(SharePtr(this));
        Bitmap* bmp = this;

        while (bmp->GetWidth() > 1 || bmp->GetHeight() > 1)
        {
            bmp = bmp->DownSample2x2();
            chain.Add(bmp);
        }
    }
}
