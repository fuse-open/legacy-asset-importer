//
// Copyright (C) 2010-2014 Outracks Technologies
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial
// portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
// OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include <XliImage/Texture.h>
#include <XliImage/Bitmap.h>
#include <Xli/Shared.h>

namespace Xli
{
    Texture::Face::Face()
    {
    }

    Texture::Face::Face(Image* img)
    {
        MipLevels.Add(SharePtr(img));
    }

    Texture::Face& Texture::Face::operator = (const Face& copy)
    {
        MipLevels.Clear();
        MipLevels.AddRange(copy.MipLevels);
        return *this;
    }

    Texture::Texture(TextureType type)
    {
        Type = type;
    }

    int Texture::GetWidth() const
    {
        return Faces[0].MipLevels[0]->GetWidth();
    }

    int Texture::GetHeight() const
    {
        return Faces[0].MipLevels[0]->GetHeight();
    }

    int Texture::GetDepth() const
    {
        return Faces[0].MipLevels[0]->GetDepth();
    }

    int Texture::GetMipCount() const
    {
        return Faces[0].MipLevels.Length();
    }

    Format Texture::GetFormat() const
    {
        return Faces[0].MipLevels[0]->GetFormat();
    }

    Texture* Texture::Create(Image* img)
    {
        Texture* result = new Texture(TextureType2D);
        result->Faces.Add(Texture::Face(img));
        return result;
    }

    Texture* Texture::CreateCube(Image* posX, Image* negX, Image* posY, Image* negY, Image* posZ, Image* negZ)
    {
        Texture* result = new Texture(TextureTypeCube);
        result->Faces.Reserve(6);
        result->Faces.Add(Texture::Face(posX));
        result->Faces.Add(Texture::Face(negX));
        result->Faces.Add(Texture::Face(posY));
        result->Faces.Add(Texture::Face(negY));
        result->Faces.Add(Texture::Face(posZ));
        result->Faces.Add(Texture::Face(negZ));
        return result;
    }

    static Texture::Cube2DMode DetectCubeFrom2DMode(int w, int h)
    {
        if (w / 6 == h) return Texture::Cube2DMode6x1;
        if (2 * w / 3 == h) return Texture::Cube2DMode3x2;
        if (h / 6 == w) return Texture::Cube2DMode1x6;
        if (2 * h / 3 == w) return Texture::Cube2DMode2x3;
        XLI_THROW("Unable to create cube map: Invalid image dimension");
    }

    Texture* Texture::Convert2DToCube()
    {
        Managed<Bitmap> bmp = Faces[0].MipLevels[0]->ToBitmap();
        Cube2DMode mode = DetectCubeFrom2DMode(bmp->GetWidth(), bmp->GetHeight());

        int size;
        Recti coordsPosX, coordsNegX, coordsPosY, coordsNegY, coordsPosZ, coordsNegZ;

        switch (mode)
        {
        case Cube2DMode1x6:
            size = bmp->GetWidth();
            coordsPosX = Recti(0*size, 0*size, 1*size, 1*size);
            coordsNegX = Recti(0*size, 1*size, 1*size, 2*size);
            coordsPosY = Recti(0*size, 2*size, 1*size, 3*size);
            coordsNegY = Recti(0*size, 3*size, 1*size, 4*size);
            coordsPosZ = Recti(0*size, 4*size, 1*size, 5*size);
            coordsNegZ = Recti(0*size, 5*size, 1*size, 6*size);
            break;

        case Cube2DMode2x3:
            size = bmp->GetWidth() / 2;
            coordsPosX = Recti(0*size, 0*size, 1*size, 1*size);
            coordsNegX = Recti(1*size, 0*size, 2*size, 1*size);
            coordsPosY = Recti(0*size, 1*size, 1*size, 2*size);
            coordsNegY = Recti(1*size, 1*size, 2*size, 2*size);
            coordsPosZ = Recti(0*size, 2*size, 1*size, 3*size);
            coordsNegZ = Recti(1*size, 2*size, 2*size, 3*size);
            break;

        case Cube2DMode3x2:
            size = bmp->GetHeight() / 2;
            coordsPosX = Recti(0*size, 0*size, 1*size, 1*size);
            coordsNegX = Recti(1*size, 0*size, 2*size, 1*size);
            coordsPosY = Recti(2*size, 0*size, 3*size, 1*size);
            coordsNegY = Recti(0*size, 1*size, 1*size, 2*size);
            coordsPosZ = Recti(1*size, 1*size, 2*size, 2*size);
            coordsNegZ = Recti(2*size, 1*size, 3*size, 2*size);
            break;

        case Cube2DMode6x1:
            size = bmp->GetHeight();
            coordsPosX = Recti(0*size, 0*size, 1*size, 1*size);
            coordsNegX = Recti(1*size, 0*size, 2*size, 1*size);
            coordsPosY = Recti(2*size, 0*size, 3*size, 1*size);
            coordsNegY = Recti(3*size, 0*size, 4*size, 1*size);
            coordsPosZ = Recti(4*size, 0*size, 5*size, 1*size);
            coordsNegZ = Recti(5*size, 0*size, 6*size, 1*size);
            break;
        }

        Managed<Bitmap> posX = new Bitmap(size, size, bmp->GetFormat());
        Managed<Bitmap> negX = new Bitmap(size, size, bmp->GetFormat());
        Managed<Bitmap> posY = new Bitmap(size, size, bmp->GetFormat());
        Managed<Bitmap> negY = new Bitmap(size, size, bmp->GetFormat());
        Managed<Bitmap> posZ = new Bitmap(size, size, bmp->GetFormat());
        Managed<Bitmap> negZ = new Bitmap(size, size, bmp->GetFormat());

        posX->Blit(bmp, coordsPosX, Vector2i(0, 0));
        negX->Blit(bmp, coordsNegX, Vector2i(0, 0));
        posY->Blit(bmp, coordsPosY, Vector2i(0, 0));
        negY->Blit(bmp, coordsNegY, Vector2i(0, 0));
        posZ->Blit(bmp, coordsPosZ, Vector2i(0, 0));
        negZ->Blit(bmp, coordsNegZ, Vector2i(0, 0));

        return CreateCube(posX, negX, posY, negY, posZ, negZ);
    }

    Texture* Texture::ConvertCubeTo2D(Cube2DMode mode)
    {
        Managed<Bitmap> posX = Faces[0].MipLevels[0]->ToBitmap();
        Managed<Bitmap> negX = Faces[1].MipLevels[0]->ToBitmap();
        Managed<Bitmap> posY = Faces[2].MipLevels[0]->ToBitmap();
        Managed<Bitmap> negY = Faces[3].MipLevels[0]->ToBitmap();
        Managed<Bitmap> posZ = Faces[4].MipLevels[0]->ToBitmap();
        Managed<Bitmap> negZ = Faces[5].MipLevels[0]->ToBitmap();

        int size = posX->GetWidth();
        
        int width, height;
        Recti coordsPosX, coordsNegX, coordsPosY, coordsNegY, coordsPosZ, coordsNegZ;

        switch (mode)
        {
        case Cube2DMode1x6:
            width = 1*size;
            height = 6*size;
            coordsPosX = Recti(0*size, 0*size, 1*size, 1*size);
            coordsNegX = Recti(0*size, 1*size, 1*size, 2*size);
            coordsPosY = Recti(0*size, 2*size, 1*size, 3*size);
            coordsNegY = Recti(0*size, 3*size, 1*size, 4*size);
            coordsPosZ = Recti(0*size, 4*size, 1*size, 5*size);
            coordsNegZ = Recti(0*size, 5*size, 1*size, 6*size);
            break;

        case Cube2DMode2x3:
            width = 2*size;
            height = 3*size;
            coordsPosX = Recti(0*size, 0*size, 1*size, 1*size);
            coordsNegX = Recti(1*size, 0*size, 2*size, 1*size);
            coordsPosY = Recti(0*size, 1*size, 1*size, 2*size);
            coordsNegY = Recti(1*size, 1*size, 2*size, 2*size);
            coordsPosZ = Recti(0*size, 2*size, 1*size, 3*size);
            coordsNegZ = Recti(1*size, 2*size, 2*size, 3*size);
            break;

        case Cube2DMode3x2:
            width = 3*size;
            height = 2*size;
            coordsPosX = Recti(0*size, 0*size, 1*size, 1*size);
            coordsNegX = Recti(1*size, 0*size, 2*size, 1*size);
            coordsPosY = Recti(2*size, 0*size, 3*size, 1*size);
            coordsNegY = Recti(0*size, 1*size, 1*size, 2*size);
            coordsPosZ = Recti(1*size, 1*size, 2*size, 2*size);
            coordsNegZ = Recti(2*size, 1*size, 3*size, 2*size);
            break;

        case Cube2DMode6x1:
            width = 6*size;
            height = 1*size;
            coordsPosX = Recti(0*size, 0*size, 1*size, 1*size);
            coordsNegX = Recti(1*size, 0*size, 2*size, 1*size);
            coordsPosY = Recti(2*size, 0*size, 3*size, 1*size);
            coordsNegY = Recti(3*size, 0*size, 4*size, 1*size);
            coordsPosZ = Recti(4*size, 0*size, 5*size, 1*size);
            coordsNegZ = Recti(5*size, 0*size, 6*size, 1*size);
            break;
        }
        
        Managed<Bitmap> bmp = new Bitmap(width, height, posX->GetFormat());

        bmp->Blit(posX, posX->GetRect(), coordsPosX.TopLeft());
        bmp->Blit(negX, negX->GetRect(), coordsNegX.TopLeft());
        bmp->Blit(posY, posY->GetRect(), coordsPosY.TopLeft());
        bmp->Blit(negY, negY->GetRect(), coordsNegY.TopLeft());
        bmp->Blit(posZ, posZ->GetRect(), coordsPosZ.TopLeft());
        bmp->Blit(negZ, negZ->GetRect(), coordsNegZ.TopLeft());

        return Create(bmp);
    }
}
