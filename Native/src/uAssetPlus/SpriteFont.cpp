#include <Fonts/BitmapFontBuilder.h>
#include <uAsset/Internal.h>
#include <uAssetPlus/SpriteFont.h>

int STDCALL uCreateSpriteFont(uHandle handle, uString filename, int fontSize, int pow2, uString charset)
{
    U_BEGIN_FUNC;
    BitmapFontBuilder b;
    b.PowerOfTwo = pow2 != 0;
    b.AllowRotation = false;
    b.Padding = 1;

    if (charset != 0)
        b.CharSet = charset;
        
    Managed<BitmapFont> font = b.Build(Unicode::Utf16To8(filename), fontSize);

    obj->SdfFontPtr = new SdfFont();

    obj->SdfFontPtr->Ascender = (float)font->Ascender;
    obj->SdfFontPtr->Descender = (float)font->Descender;
    obj->SdfFontPtr->LineHeight = (float)font->LineHeight;
    obj->SdfFontPtr->Bitmap = font->Bitmap;

    obj->SdfFontPtr->PixelSize = (float)font->PixelSize;
    obj->SdfFontPtr->PixelSpread = 0.0f;

    obj->SdfFontPtr->FamilyName = font->FamilyName;
    obj->SdfFontPtr->StyleName = font->StyleName;

    for (int it = font->Characters.Begin(); it != font->Characters.End(); it = font->Characters.Next(it))
    {
        const UInt32& key = font->Characters.GetKey(it);
        const BitmapFont::CharInfo& value = font->Characters.GetValue(it);

        SdfFont::GlyphInfo g;
        g.Advance = (float)value.Advance;
        g.Bearing.X = (float)value.Bearing.X;
        g.Bearing.Y = (float)value.Bearing.Y;
        g.Size.X = (float)value.Size.X;
        g.Size.Y = (float)value.Size.Y;
        g.UpperLeft.X = value.UpperLeft.X;
        g.UpperLeft.Y = value.UpperLeft.Y;
        g.LowerRight.X = value.LowerRight.X;
        g.LowerRight.Y = value.LowerRight.Y;

        obj->SdfFontPtr->Glyphs.Add(key, g);
    }

    for (int it = font->Kernings.Begin(); it != font->Kernings.End(); it = font->Kernings.Next(it))
    {
        const BitmapFont::CharPair& key = font->Kernings.GetKey(it);
        const int& value = font->Kernings.GetValue(it);

        obj->SdfFontPtr->Kernings.Add(SdfFont::CharPair(key.A, key.B), (float)value);
    }

    return 0;
    U_END_FUNC;
}

int STDCALL uSpriteFont_SaveTextureAsPng(uHandle handle, uString filename)
{
    U_BEGIN_FUNC;
    obj->SdfFontPtr->SaveBitmapAsPng(Unicode::Utf16To8(filename));
    return 0;
    U_END_FUNC;
}

int STDCALL uSpriteFont_SaveData(uHandle handle, uString filename)
{
    U_BEGIN_FUNC;
    obj->SdfFontPtr->SaveMetricsAsPsf(Unicode::Utf16To8(filename));
    return 0;
    U_END_FUNC;
}
