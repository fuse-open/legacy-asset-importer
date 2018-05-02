#include <Fonts/Atlas.h>

namespace Fonts
{
    TextureAtlas::TextureAtlas()
    {

    }

    TextureAtlas::TextureAtlas(const BitmapAtlas* bc)
    {
        Tiles.Resize(bc->Tiles.Length());

        float w = (float)bc->Bitmap->GetWidth();
        float h = (float)bc->Bitmap->GetHeight();

        for (int i = 0; i < Tiles.Length(); i++)
        {
            Tiles[i].LowerLeft.X = (float)bc->Tiles[i].LowerLeft.X / w;
            Tiles[i].LowerLeft.Y = 1.0f - (float)bc->Tiles[i].LowerLeft.Y / h;
            Tiles[i].LowerRight.X = (float)bc->Tiles[i].LowerRight.X / w;
            Tiles[i].LowerRight.Y = 1.0f - (float)bc->Tiles[i].LowerRight.Y / h;
            Tiles[i].UpperRight.X = (float)bc->Tiles[i].UpperRight.X / w;
            Tiles[i].UpperRight.Y = 1.0f - (float)bc->Tiles[i].UpperRight.Y / h;
            Tiles[i].UpperLeft.X = (float)bc->Tiles[i].UpperLeft.X / w;
            Tiles[i].UpperLeft.Y = 1.0f - (float)bc->Tiles[i].UpperLeft.Y / h;
            Tiles[i].IsRotated = bc->Tiles[i].IsRotated;
        }
    }

    void TextureAtlas::MapTexCoords(int index, int count, const Vector2* input, Vector2* output) const
    {
        if (Tiles[index].IsRotated)
        {
            Vector2 bias = Tiles[index].UpperRight;
            Vector2 scale = Tiles[index].LowerLeft - Tiles[index].UpperRight;

            for (int i = 0; i < count; i++)
            {
                output[i] = bias + Vector2(input[i].Y, input[i].X) * scale;
            }
        }
        else
        {
            Vector2 bias = Tiles[index].LowerLeft;
            Vector2 scale = Tiles[index].UpperRight - Tiles[index].LowerLeft;

            for (int i = 0; i < count; i++)
            {
                output[i] = bias + input[i] * scale;
            }
        }
    }

    void TextureAtlas::MapTexCoords(int index, const Array<Vector2>& input, Array<Vector2>& output) const
    {
        int orgSize = output.Length();
        output.Resize(orgSize + input.Length());
        MapTexCoords(index, input.Length(), input.Ptr(), output.Ptr() + orgSize);
    }
}
