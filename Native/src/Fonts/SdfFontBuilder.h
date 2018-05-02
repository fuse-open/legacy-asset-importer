#ifndef __CRUNCH_FONTS_SDF_FONT_BUILDER_H__
#define __CRUNCH_FONTS_SDF_FONT_BUILDER_H__

#include <Fonts/SdfFont.h>
#include <Xli/Utf16String.h>
#include <XliFont/FontFace.h>

namespace Fonts
{
    class SdfFontBuilder
    {
    public:
        Utf16String CharSet;
        int TextureSize;

        SdfFontBuilder();

        SdfFont* Build(FontFace* ff);
        SdfFont* Build(const String& filename);
    };
}

#endif
