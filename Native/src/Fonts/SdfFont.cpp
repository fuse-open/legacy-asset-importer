#include <Fonts/SdfFont.h>
#include <Xli/File.h>
#include <Xli/BinaryWriter.h>
#include <XliImage/Png.h>

#define PSF_MAGIC 0x01465350

namespace Fonts
{
    void SdfFont::SaveMetricsAsPsf(const String& filename)
    {
        File f(filename, FileModeWrite);
        BinaryWriter w(&f);

        // magic number
        w.WriteUInt32(PSF_MAGIC);

        w.WriteFloat(Ascender);
        w.WriteFloat(Descender);
        w.WriteFloat(LineHeight);

        w.Write7BitEncodedInt32(Glyphs.Count());

        for (int it = Glyphs.Begin(); it != Glyphs.End(); it = Glyphs.Next(it))
        {
            const UInt16& key = Glyphs.GetKey(it);
            const GlyphInfo& value = Glyphs.GetValue(it);

            w.Write7BitEncodedInt32(key);
            w.WriteFloat(value.Advance);
            w.WriteFloat(value.Bearing.X);
            w.WriteFloat(value.Bearing.Y);
            w.WriteFloat(value.Size.X);
            w.WriteFloat(value.Size.Y);
            w.Write7BitEncodedInt32(value.UpperLeft.X);
            w.Write7BitEncodedInt32(value.UpperLeft.Y);
            w.Write7BitEncodedInt32(value.LowerRight.X);
            w.Write7BitEncodedInt32(value.LowerRight.Y);
        }

        w.Write7BitEncodedInt32(Kernings.Count());

        for (int it = Kernings.Begin(); it != Kernings.End(); it = Kernings.Next(it))
        {
            const CharPair& key = Kernings.GetKey(it);
            const float& value = Kernings.GetValue(it);

            w.Write7BitEncodedInt32(key.A);
            w.Write7BitEncodedInt32(key.B);
            w.WriteFloat(value);
        }

        w.WriteFloat(PixelSize);
        w.WriteFloat(PixelSpread);

        w.WriteString(FamilyName);
        w.WriteString(StyleName);
    }

    void SdfFont::SaveBitmapAsPng(const String& filename)
    {
        Png::Save(filename, Bitmap);
    }
}
