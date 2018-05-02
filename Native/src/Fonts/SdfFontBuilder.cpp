#include <Fonts/BinPacker.h>
#include <Fonts/SdfFontBuilder.h>
#include <Common/Math.h>
#include <Xli/Unicode.h>
#include <Xli/Console.h>
#include <cstring>

namespace Fonts
{
    SdfFontBuilder::SdfFontBuilder()
    {
        TextureSize = 256;
        CharSet = Unicode::Utf8To16("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
    }

    struct SdfGlyph
    {
        int ID;
        int width, height;
        int x, y;
        float xoff, yoff;
        float xadv;
    };

    const int ScaleFactor = 16;

    static bool GeneratePackList(FontFace* face, float font_size, int pack_tex_size, const Utf16String& render_list, Array<SdfGlyph>& packed_glyphs)
    {
        packed_glyphs.Clear();

        Array<Vector2i> rectangle_info;
        Array<Managed<ArrayRef<BinPacker::PackedRect> > > packed_glyph_info;

        for (int i = 0; i < render_list.Length(); i++)
        {
            Vector2 advance, bearing;
            Managed<Bitmap> bmp = face->RenderGlyph(font_size, render_list[i], FontRenderModeMonochrome, &advance, &bearing);

            int w = bmp->GetWidth();
            int h = bmp->GetHeight();
            int sw = w + ScaleFactor * 4;
            int sh = h + ScaleFactor * 4;
            int sdfw = sw / ScaleFactor;
            int sdfh = sh / ScaleFactor;

            SdfGlyph add_me;
            add_me.ID = render_list[i];
            add_me.width = sdfw;
            add_me.height = sdfh;
            add_me.xoff = bearing.X / ScaleFactor - 1.5f;
            add_me.yoff = bearing.Y / ScaleFactor + 1.5f;
            add_me.xadv = advance.X / ScaleFactor;
            add_me.x = -1;
            add_me.y = -1;

            packed_glyphs.Add(add_me);
            rectangle_info.Add(Vector2i(sdfw, sdfh));
        }
        
        BinPacker().Pack(rectangle_info, packed_glyph_info, pack_tex_size, false);
        
        if (packed_glyph_info.Length() == 1)
        {
            int l = packed_glyph_info[0]->Data.Length();

            for (int i = 0; i < l; i++)
            {
                int idx = packed_glyph_info[0]->Data[i].Index;
                packed_glyphs[idx].x = packed_glyph_info[0]->Data[i].Position.X;
                packed_glyphs[idx].y = packed_glyph_info[0]->Data[i].Position.Y;
            }

            return true;
        }

        return false;
    }

    static UInt8 GetSdfRadial(UInt8* fontmap, int w, int h, int x, int y, int max_radius)
    {
        //    hideous brute force method
        float d2 = max_radius * max_radius + 1.0f;
        unsigned char v = fontmap[x + y * w];

        for (int radius = 1; radius <= max_radius && radius * radius < d2; radius++)
        {
            int line, lo, hi;
            
            //    north
            line = y - radius;
            if (line >= 0 && line < h)
            {
                lo = x - radius;
                hi = x + radius;
                if (lo < 0) lo = 0;
                if (hi >= w) hi = w - 1;
                int idx = line * w + lo;
                for (int i = lo; i <= hi; ++i)
                {
                    if (fontmap[idx] != v)
                    {
                        float nx = (float)(i - x);
                        float ny = (float)(line - y);
                        float nd2 = nx*nx+ny*ny;
                        if (nd2 < d2)
                            d2 = nd2;
                    }
                    ++idx;
                }
            }

            //    south
            line = y + radius;
            if (line >= 0 && line < h)
            {
                lo = x - radius;
                hi = x + radius;
                if (lo < 0) lo = 0;
                if (hi >= w) hi = w - 1;
                int idx = line * w + lo;
                for (int i = lo; i <= hi; ++i)
                {
                    if (fontmap[idx] != v)
                    {
                        float nx = (float)(i - x);
                        float ny = (float)(line - y);
                        float nd2 = nx*nx+ny*ny;
                        if (nd2 < d2)
                            d2 = nd2;
                    }
                    ++idx;
                }
            }

            //    west
            line = x - radius;
            if (line >= 0 && line < w)
            {
                lo = y - radius + 1;
                hi = y + radius - 1;
                if (lo < 0) lo = 0;
                if (hi >= h) hi = h - 1;
                int idx = lo * w + line;
                for (int i = lo; i <= hi; ++i)
                {
                    if (fontmap[idx] != v)
                    {
                        float nx = (float)(line - x);
                        float ny = (float)(i - y);
                        float nd2 = nx*nx+ny*ny;
                        if (nd2 < d2)
                            d2 = nd2;
                    }
                    idx += w;
                }
            }

            //    east
            line = x + radius;
            if(line >= 0 && line < w)
            {
                lo = y - radius + 1;
                hi = y + radius - 1;
                if( lo < 0 ) lo = 0;
                if( hi >= h ) hi = h - 1;
                int idx = lo * w + line;
                for (int i = lo; i <= hi; ++i)
                {
                    if (fontmap[idx] != v)
                    {
                        float nx = (float)(line - x);
                        float ny = (float)(i - y);
                        float nd2 = nx*nx+ny*ny;
                        if (nd2 < d2)
                            d2 = nd2;
                    }
                    idx += w;
                }
            }
        }

        d2 = sqrtf( d2 );

        if (v == 0)
            d2 = -d2;

        d2 *= 127.5f / max_radius;
        d2 += 127.5f;
        
        if (d2 < 0.0f) d2 = 0.0f;
        if (d2 > 255.0f) d2 = 255.0f;

        return (UInt8)(d2 + 0.5f);
    }

    SdfFont* SdfFontBuilder::Build(FontFace* face)
    {
        Utf16 ws[] = { ' ', '\0' };
        Utf16String wsCharSet = CharSet + Utf16String(ws, 2);

        int sz = 4;
        bool keep_going = true;
        Array<SdfGlyph> all_glyphs;

        while (keep_going)
        {
            sz *= 2;
            keep_going = GeneratePackList(face, (float)sz * (float)ScaleFactor, TextureSize, wsCharSet, all_glyphs);
        }

        int sz_step = sz / 4;

        while (sz_step)
        {
            if (keep_going)
                sz += sz_step;
            else
                sz -= sz_step;

            sz_step /= 2;
            keep_going = GeneratePackList(face, (float)(sz * ScaleFactor), TextureSize, wsCharSet, all_glyphs);
        }

        while (!keep_going && sz > 1)
        {
            --sz;
            keep_going = GeneratePackList(face, (float)(sz * ScaleFactor), TextureSize, wsCharSet, all_glyphs);
        }

        if (!keep_going)
        {
            XLI_THROW("The font does not fit into the texture");
        }

        SdfFont* result = new SdfFont();
        
        result->Bitmap = new Bitmap(TextureSize, TextureSize, FormatL_8_UInt_Normalize);
        result->Bitmap->Clear();

        UInt8* dst = result->Bitmap->GetPtr();

        for (int i = 0; i < wsCharSet.Length(); i++)
        {
            Utf16 c = wsCharSet[i];
            Managed<Bitmap> glyph = face->RenderGlyph((float)(sz * ScaleFactor), c, FontRenderModeMonochrome, 0, 0);

            int w = glyph->GetWidth();
            int h = glyph->GetHeight();
            int p = glyph->GetPitch();

            int sw = w + ScaleFactor * 4;
            int sh = h + ScaleFactor * 4;
            UInt8* smooth_buf = new UInt8[sw*sh];
            memset(smooth_buf, 0, sw * sh);

            for (int y = 0; y < h; y++)
                for (int x = 0; x < w; x++)
                    smooth_buf[ScaleFactor * 2 + x + (y + ScaleFactor * 2) * sw] = glyph->GetPtr()[y * p + x];

            int sdfw = all_glyphs[i].width;
            int sdfx = all_glyphs[i].x;
            int sdfh = all_glyphs[i].height;
            int sdfy = all_glyphs[i].y;

            for (int y = 0; y < sdfh; y++)
                for (int x = 0; x < sdfw; x++)
                    dst[x + sdfx + (y + sdfy) * TextureSize] = 
                        GetSdfRadial(smooth_buf, sw, sh, x * ScaleFactor + (ScaleFactor / 2), y * ScaleFactor + (ScaleFactor / 2), 2 * ScaleFactor);

            delete [] smooth_buf;

            if (result->Glyphs.ContainsKey(c))
            {
                Error->WriteLine("WARNING: Font already contains character #" + (String)(int)c);
                continue;
            }

            for (int j = 0; j < wsCharSet.Length(); j++)
            {
                Vector2 kerning;
                if (j != i && face->TryGetKerning((float)sz, c, wsCharSet[j], &kerning))
                {
                    SdfFont::CharPair key(c, wsCharSet[j]);
                    if (kerning.X != 0 && !result->Kernings.ContainsKey(key))
                        result->Kernings.Add(key, kerning.X / (float)(sz * ScaleFactor));
                }
            }

            SdfFont::GlyphInfo g;
            g.UpperLeft.X = sdfx;
            g.UpperLeft.Y = sdfy;
            g.LowerRight.X = sdfx + sdfw;
            g.LowerRight.Y = sdfy + sdfh;
            g.Advance = all_glyphs[i].xadv;
            g.Bearing.X = all_glyphs[i].xoff;
            g.Bearing.Y = all_glyphs[i].yoff;
            g.Size.X = (float)sdfw;
            g.Size.Y = (float)sdfh;

            result->Glyphs.Add(wsCharSet[i], g);
        }

        result->PixelSize = (float)sz;
        result->PixelSpread = 1.5f;

        result->FamilyName = face->GetFamilyName();
        result->StyleName = face->GetStyleName();

        result->Ascender = face->GetAscender((float)(sz * ScaleFactor)) / (float)ScaleFactor;
        result->Descender = face->GetDescender((float)(sz * ScaleFactor)) / (float)ScaleFactor;
        result->LineHeight = face->GetLineHeight((float)(sz * ScaleFactor)) / (float)ScaleFactor;

        return result;
    }

    SdfFont* SdfFontBuilder::Build(const String& filename)
    {
        Managed<FontFace> face = FreeType::LoadFontFace(filename);
        return Build(face);
    }
}
