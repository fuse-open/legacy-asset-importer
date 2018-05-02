#ifndef __XLI_BITMAP_FONT_BUILDER_H__
#define __XLI_BITMAP_FONT_BUILDER_H__

#include <XliFont/FontFace.h>
#include <Xli/Utf16String.h>
#include "BitmapFont.h"

namespace Fonts
{
    using namespace Xli;

    class BitmapFontBuilder
    {
    public:
        int Padding;
        bool AllowRotation;
        bool PowerOfTwo;
        Format PixelFormat;
        Utf16String CharSet;

        BitmapFontBuilder();

        BitmapFont* Build(FontFace* ff, int size);
        BitmapFont* Build(const Xli::String& fileName, int size);
    };
}

#endif
