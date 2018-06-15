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

#ifndef __XLI_FONT_FACE_H__
#define __XLI_FONT_FACE_H__

#include <Xli/Object.h>
#include <Xli/File.h>
#include <Xli/Vector2.h>
#include <Xli/String.h>
#include <XliImage/Bitmap.h>

namespace Xli
{
    /**
        \ingroup XliFont
    */
    enum FontRenderMode
    {
        FontRenderModeNone,
        FontRenderModeNormal,
        FontRenderModeMonochrome,
    };

    /**
        \ingroup XliFont
    */
    class FontFace: public Object
    {
    public:
        virtual String GetFamilyName() = 0;
        virtual String GetStyleName() = 0;

        virtual float GetAscender(float pixelSize) = 0;
        virtual float GetDescender(float pixelSize) = 0;
        virtual float GetLineHeight(float pixelSize) = 0;

        virtual bool ContainsGlyph(float pixelSize, Utf32 c) = 0;
        virtual Bitmap* RenderGlyph(float pixelSize, Utf32 c, FontRenderMode mode, Vector2* outAdvance, Vector2* outBearing) = 0;

        virtual bool TryGetKerning(float pixelSize, Utf32 left, Utf32 right, Vector2* outKerning) = 0;
    };

    /**
        \ingroup XliFont
    */
    class FreeType
    {
    public:
        static void Init();
        static void Done();

        static FontFace* LoadFontFace(Stream* fontFile);

        static FontFace* LoadFontFace(const String& filename)
        {
            File f(filename, FileModeRead);
            return LoadFontFace(&f);
        }
    };
}

#endif
