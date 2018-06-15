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

#ifndef __XLI_MEDIA_IMAGE_FORMAT_H__
#define __XLI_MEDIA_IMAGE_FORMAT_H__

#include <Xli/IntTypes.h>
#include <Xli/Exception.h>

namespace Xli
{
    /**
        \ingroup XliImage
    */
    enum Format
    {
        FormatUnspecified = 0,

        FormatR_8_Int = 1,
        FormatRG_8_8_Int = 2,
        FormatRGB_8_8_8_Int = 3,
        FormatRGBA_8_8_8_8_Int = 4,

        FormatR_8_UInt = 5,
        FormatRG_8_8_UInt = 6,
        FormatRGB_8_8_8_UInt = 7,
        FormatRGBA_8_8_8_8_UInt = 8,

        FormatR_8_Int_Normalize = 9,
        FormatRG_8_8_Int_Normalize = 10,
        FormatRGB_8_8_8_Int_Normalize = 11,
        FormatRGBA_8_8_8_8_Int_Normalize = 12,

        FormatR_8_UInt_Normalize = 13,
        FormatRG_8_8_UInt_Normalize = 14,
        FormatRGB_8_8_8_UInt_Normalize = 15,
        FormatRGBA_8_8_8_8_UInt_Normalize = 16,

        FormatR_8_Typeless = 17,
        FormatRG_8_8_Typeless = 18,
        FormatRGB_8_8_8_Typeless = 19,
        FormatRGBA_8_8_8_8_Typeless = 20,

        FormatA_8_UInt_Normalize = 21,
        FormatL_8_UInt_Normalize = 22,
        FormatLA_8_8_UInt_Normalize = 23,

        FormatR_16_Int = 24,
        FormatRG_16_16_Int = 25,
        FormatRGB_16_16_16_Int = 26,
        FormatRGBA_16_16_16_16_Int = 27,

        FormatR_16_Int_Normalize = 28,
        FormatRG_16_16_Int_Normalize = 29,
        FormatRGB_16_16_16_Int_Normalize = 30,
        FormatRGBA_16_16_16_16_Int_Normalize = 31,

        FormatR_16_UInt = 32,
        FormatRG_16_16_UInt = 33,
        FormatRGB_16_16_16_UInt = 34,
        FormatRGBA_16_16_16_16_UInt = 35,

        FormatR_16_UInt_Normalize = 36,
        FormatRG_16_16_UInt_Normalize = 37,
        FormatRGB_16_16_16_UInt_Normalize = 38,
        FormatRGBA_16_16_16_16_UInt_Normalize = 39,

        FormatR_16_Float = 40,
        FormatRG_16_16_Float = 41,
        FormatRGB_16_16_16_Float = 42,
        FormatRGBA_16_16_16_16_Float = 43,

        FormatR_16_Typeless = 44,
        FormatRG_16_16_Typeless = 45,
        FormatRGB_16_16_16_Typeless = 46,
        FormatRGBA_16_16_16_16_Typeless = 47,

        FormatR_32_Int = 48,
        FormatRG_32_32_Int = 49,
        FormatRGB_32_32_32_Int = 50,
        FormatRGBA_32_32_32_32_Int = 51,

        FormatR_32_UInt = 52,
        FormatRG_32_32_UInt = 53,
        FormatRGB_32_32_32_UInt = 54,
        FormatRGBA_32_32_32_32_UInt = 55,
        
        FormatR_32_Float = 56,
        FormatRG_32_32_Float = 57,
        FormatRGB_32_32_32_Float = 58,
        FormatRGBA_32_32_32_32_Float = 59,

        FormatR_32_Typeless = 60,
        FormatRG_32_32_Typeless = 61,
        FormatRGB_32_32_32_Typeless = 62,
        FormatRGBA_32_32_32_32_Typeless = 63,

        // Compressed formats
        FormatCompressedRGB_BC1 = 200,
        FormatCompressedRGB_BC2 = 201,
        FormatCompressedRGB_BC3 = 202,
        FormatCompressedRGB_BC4 = 203,
        FormatCompressedRGB_BC5 = 204,
        FormatCompressedRGB_ETC1 = 205,
        FormatCompressedRGB_PVRTC_4BPP = 206,
        FormatCompressedRGB_PVRTC_2BPP = 207,
        FormatCompressedRGBA_PVRTC_4BPP = 208,
        FormatCompressedRGBA_PVRTC_2BPP = 209,

        // Depth/Stencil formats
        FormatD_32_Float = 500,
        FormatDSX_32_8_24_Float_UInt_Typeless = 501,
        FormatDS_24_8_UInt_Normalize_UInt = 502,

        FormatD_16_UNorm = 503,
        FormatR_16_UNorm = 504
    };

    /**
        \ingroup XliImage
    */
    class FormatInfo
    {
    public:
        static int ComponentCount(Format format);
        static String ToString(Format format);
        static int SizeOf(Format format);
    };
}

#endif
