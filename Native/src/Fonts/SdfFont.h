#ifndef __CRUNCH_FONTS_SDF_FONT_H__
#define __CRUNCH_FONTS_SDF_FONT_H__

#include <Xli/HashMap.h>
#include <Xli/IntTypes.h>
#include <Xli/Managed.h>
#include <Xli/Vector2.h>
#include <XliImage/Bitmap.h>

namespace Fonts
{
    using namespace Xli;

    class SdfFont : public Object
    {
    public:
        struct GlyphInfo
        {
            float Advance;
            Vector2 Bearing, Size;
            Vector2i UpperLeft, LowerRight;
        };

        struct CharPair
        {
            Utf16 A, B;

            CharPair()
            {
            }

            CharPair(UInt16 a, UInt16 b) 
                : A(a), B(b)
            { 
            }

            bool operator == (const CharPair& other) const
            {
                return A == other.A && B == other.B;
            }
        };

        float Ascender, Descender, LineHeight;
        Managed<Xli::Bitmap> Bitmap;
        
        HashMap<Utf16, GlyphInfo> Glyphs;
        HashMap<CharPair, float> Kernings;

        String FamilyName;
        String StyleName;
        
        float PixelSize;
        float PixelSpread;

        void SaveMetricsAsPsf(const String& filename);
        void SaveBitmapAsPng(const String& filename);
    };
}

#endif