#ifndef __XLI_SPRITE_FONT_H__
#define __XLI_SPRITE_FONT_H__

#include <Fonts/BitmapFont.h>

namespace Fonts
{
    using namespace Xli;

    enum TextAlign
    {
        TextAlignLeft = 1<<0,
        TextAlignCenter = 1<<1,
        TextAlignRight = 1<<2,
        TextAlignTop = 1<<3,
        TextAlignMiddle = 1<<4,
        TextAlignBottom = 1<<5,

        TextAlignTopLeft = TextAlignTop | TextAlignLeft,
        TextAlignTopCenter = TextAlignTop | TextAlignCenter,
        TextAlignTopRight = TextAlignTop | TextAlignRight,
        TextAlignMiddleLeft = TextAlignMiddle | TextAlignLeft,
        TextAlignMiddleCenter = TextAlignMiddle | TextAlignCenter,
        TextAlignMiddleRight = TextAlignMiddle | TextAlignRight,
        TextAlignBottomLeft = TextAlignBottom | TextAlignLeft,
        TextAlignBottomCenter = TextAlignBottom | TextAlignCenter,
        TextAlignBottomRight = TextAlignBottom | TextAlignRight,
    };

    class SpriteFont: public Object
    {
    protected:
        SpriteFont(BitmapFont* bmpFont);

    public:
        struct Vertex
        {
            Vector2i16 Position;
            Vector2u16 TexCoord;
        };

        struct CharInfo
        {
            Vector2u16 UpperLeft, UpperRight, LowerRight, LowerLeft;
            Vector2i Size, Bearing;
            int Advance;
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

        String FamilyName;
        String StyleName;

        int Ascender, Descender, LineHeight;
        HashMap<Utf16, CharInfo> Characters;
        HashMap<CharPair, int> Kernings;

        virtual ~SpriteFont() {}

        int GetStringWidth(const String& str) const;
        int GetLineHeight() const;

        void WriteCharacter(const CharInfo& cd, const Vector2i& cursor, Array<Vertex>& output) const;
        Vector2i WriteString(const String& str, Vector2i cursor, const Recti& cullRect, Array<Vertex>& output) const;

        void WriteString(const String& str, const Recti& rect, int textAlign, bool wordWrap, Array<Vertex>& output) const;
        void WriteString(const String& str, const Recti& rect, int textAlign, Array<Vertex>& output) const { WriteString(str, rect, textAlign, false, output); }

        void WordWrap(const String& str, Vector2i windowSize, Array<String>& result, int startPosition = 0, int* endPosition = 0, int startIndex = 0, int* endIndex = 0) const;
    };
}

#endif
