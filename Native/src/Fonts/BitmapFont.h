#ifndef __XLI_GRAPHICS_BITMAP_FONT_H__
#define __XLI_GRAPHICS_BITMAP_FONT_H__

#include <XliImage/Bitmap.h>
#include <Xli/Managed.h>
#include <Xli/Vector2.h>
#include <Xli/HashMap.h>
#include <Xli/File.h>

namespace Fonts
{
    using namespace Xli;

    class BitmapFont: public Object
    {
    public:
        struct CharInfo
        {
            int Advance;
            Vector2i Bearing, Size;
            Vector2i UpperLeft, UpperRight, LowerRight, LowerLeft;
        };

        struct CharPair
        {
            Utf16 A, B;

            CharPair()
            {
            }

            CharPair(Utf16 a, Utf16 b)
                : A(a), B(b)
            {
            }

            bool operator == (const CharPair& other) const
            {
                return A == other.A && B == other.B;
            }
        };

        int Ascender, Descender, LineHeight;
        Managed<Xli::Bitmap> Bitmap;

        HashMap<Utf16, CharInfo> Characters;
        HashMap<CharPair, int> Kernings;

        String FamilyName;
        String StyleName;
        int PixelSize;
    };
}

#endif
