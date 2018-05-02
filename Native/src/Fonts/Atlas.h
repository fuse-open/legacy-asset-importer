#ifndef __XLI_GRAPHICS_ATLAS_H__
#define __XLI_GRAPHICS_ATLAS_H__

#include <XliImage/Bitmap.h>
#include <Xli/Managed.h>
#include <Xli/Vector2.h>
#include <Xli/Array.h>

namespace Fonts
{
    using namespace Xli;

    class BitmapAtlas: public Object
    {
    public:
        struct Tile
        {
            Vector2i LowerLeft;
            Vector2i LowerRight;
            Vector2i UpperRight;
            Vector2i UpperLeft;
            bool IsRotated;
        };

        Managed<Xli::Bitmap> Bitmap;
        Array<Tile> Tiles;
    };

    class TextureAtlas: public Object
    {
    public:
        struct Tile
        {
            Vector2 LowerLeft;
            Vector2 LowerRight;
            Vector2 UpperRight;
            Vector2 UpperLeft;
            bool IsRotated;
        };

        Array<Tile> Tiles;

        TextureAtlas();
        TextureAtlas(const BitmapAtlas* bc);

        void MapTexCoords(int index, int count, const Vector2* input, Vector2* output) const;
        void MapTexCoords(int index, const Array<Vector2>& input, Array<Vector2>& output) const;
    };
}


#endif
