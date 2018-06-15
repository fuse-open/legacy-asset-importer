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

#ifndef __XLI_GRAPHICS_TEXTURE_H__
#define __XLI_GRAPHICS_TEXTURE_H__

#include <Xli/Array.h>
#include <Xli/Managed.h>
#include <XliImage/Image.h>

namespace Xli
{
    /**
        \ingroup XliImage
    */
    enum TextureType
    {
        TextureType2D,
        TextureType3D,
        TextureTypeCube,
    };

    /**
        \ingroup XliImage
    */
    class Texture: public Object
    {
    public:
        enum FaceIndex
        {
            FaceIndexPositiveX = 0,
            FaceIndexNegativeX = 1,
            FaceIndexPositiveY = 2,
            FaceIndexNegativeY = 3,
            FaceIndexPositiveZ = 4,
            FaceIndexNegativeZ = 5,
        };

        struct Face
        {
            Array<Managed<Image> > MipLevels;
            
            Face();
            Face(Image* image);

            Face& operator = (const Face& copy);
        };

        TextureType Type;
        Array<Face> Faces;

        int GetWidth() const;
        int GetHeight() const;
        int GetDepth() const;
        int GetMipCount() const;
        Format GetFormat() const;

        Texture(TextureType type);

        static Texture* Create(Image* img);
        static Texture* CreateCube(Image* posX, Image* negX, Image* posY, Image* negY, Image* posZ, Image* negZ);

        enum Cube2DMode
        {
            Cube2DMode1x6,
            Cube2DMode2x3,
            Cube2DMode3x2,
            Cube2DMode6x1,
        };

        Texture* Convert2DToCube();
        Texture* ConvertCubeTo2D(Cube2DMode mode);
    };
}

#endif
