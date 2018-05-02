#include "BitmapFontBuilder.h"
#include "AtlasBuilder.h"
#include <Xli/Console.h>
#include <XliImage/Png.h>
#include <Xli/Unicode.h>
#include <cctype>

namespace Fonts
{
    BitmapFontBuilder::BitmapFontBuilder()
    {
        Padding = 4;
        AllowRotation = true;
        PowerOfTwo = false;
        PixelFormat = FormatL_8_UInt_Normalize;
        CharSet = Unicode::Utf8To16("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
    }

    BitmapFont* BitmapFontBuilder::Build(FontFace* ff, int size)
    {
        const float fSize = (float)size;

        Utf16 ws[] = { ' ', '\0' };
        Utf16String wsCharSet = CharSet + Utf16String(ws, 2);

        AtlasBuilder cb;
        cb.PixelFormat = PixelFormat;
        cb.Padding = Padding;
        cb.AllowRotation = AllowRotation;
        cb.PowerOfTwo = PowerOfTwo;

        for (int i = 0; i < wsCharSet.Length(); i++)
        {
            Utf16 c = wsCharSet[i];
            Managed<Bitmap> bmp = ff->RenderGlyph(fSize, c, FontRenderModeNormal, 0, 0);
            cb.AddTile(bmp);
        }

        Managed<BitmapAtlas> atlas = cb.BuildBitmapAtlas();
        Managed<BitmapFont> bf = new BitmapFont();
        int atlasIndex = 0;

        for (int i = 0; i < wsCharSet.Length(); i++)
        {
            Utf16 c = wsCharSet[i];

            if (bf->Characters.ContainsKey(c))
            {
                Error->WriteLine("WARNING: Font already contains character #" + (String)(int)c);
                continue;
            }

            BitmapFont::CharInfo cd;

            cd.UpperRight = atlas->Tiles[atlasIndex].UpperRight;
            cd.UpperLeft = atlas->Tiles[atlasIndex].UpperLeft;
            cd.LowerLeft = atlas->Tiles[atlasIndex].LowerLeft;
            cd.LowerRight = atlas->Tiles[atlasIndex].LowerRight;
            cd.Size.X = cd.UpperRight.X - cd.UpperLeft.X;
            cd.Size.Y = cd.LowerRight.Y - cd.UpperRight.Y;

            if (atlas->Tiles[atlasIndex].IsRotated)
            {
                cd.Size.X = cd.UpperRight.Y - cd.UpperLeft.Y;
                cd.Size.Y = cd.UpperRight.X - cd.LowerRight.X;
            }

            atlasIndex++;

            for (int j = 0; j < wsCharSet.Length(); j++)
            {
                Vector2 kerning;
                if (j != i && ff->TryGetKerning(fSize, wsCharSet[j], c, &kerning))
                {
                    int k = (int)kerning.X;
                    BitmapFont::CharPair cp(wsCharSet[j], c);

                    if (k != 0 && !bf->Kernings.ContainsKey(cp))
                        bf->Kernings.Add(cp, k);
                }
            }

            Vector2 advance, bearing;
            Managed<Bitmap> bmp = ff->RenderGlyph(fSize, c, FontRenderModeNormal, &advance, &bearing);
            
            cd.Bearing = (Vector2i)bearing;
            cd.Advance = (int)advance.X;

            bf->Characters.Add(c, cd);
        }

        bf->Bitmap = atlas->Bitmap;
        bf->Ascender = (int)ff->GetAscender(fSize);
        bf->Descender = (int)ff->GetDescender(fSize);
        bf->LineHeight = (int)ff->GetLineHeight(fSize);
        bf->FamilyName = ff->GetFamilyName();
        bf->StyleName = ff->GetStyleName();
        bf->PixelSize = size;

        bf->AddRef();
        return bf;
    }

    BitmapFont* BitmapFontBuilder::Build(const Xli::String& fileName, int size)
    {
        Managed<FontFace> ff = FreeType::LoadFontFace(fileName);
        return Build(ff, size);
    }
}
