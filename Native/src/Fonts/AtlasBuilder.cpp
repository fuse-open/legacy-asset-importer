#include <Fonts/AtlasBuilder.h>
#include <Common/Math.h>

namespace Fonts
{
    AtlasBuilder::AtlasBuilder()
    {
        Padding = 0;
        AllowRotation = true;
        PowerOfTwo = false;
        PixelFormat = FormatRGBA_8_8_8_8_UInt_Normalize;
    }

    class TileNode
    {
    protected:
        const AtlasBuilder* builder;

    public:
        TileNode(const AtlasBuilder* builder): builder(builder) {}
        virtual ~TileNode() {}

        virtual void Blit(BitmapAtlas* Atlas, Vector2i mount) = 0;
        virtual void Rotate() = 0;

        virtual Vector2i GetSize() = 0;

        bool operator < (TileNode& t)
        {
            if (GetSize().X == t.GetSize().X) return GetSize().Y < t.GetSize().Y;
            return GetSize().X < t.GetSize().X;
        }
    };

    class SingleTileNode: public TileNode
    {
        Vector2i size;
        bool rotated;
        int tile;

        AtlasBuilder::Tile& getTile()
        {
            return builder->Tiles[tile];
        }
        Bitmap* getSource()
        {
            return getTile().Source.Get();
        }

        AtlasBuilder* builder;

    public:
        SingleTileNode(AtlasBuilder* builder, int tile): TileNode(builder)
        {
            this->tile = tile;
            this->builder = builder;

            size.X = getSource()->GetWidth() + 2*builder->Padding;
            size.Y = getSource()->GetHeight() + 2*builder->Padding;
            rotated = false;

            if (builder->AllowRotation)
            {
                if (getSource()->GetHeight() > getSource()->GetWidth()) Rotate();
            }
            else
            {
                int max = Max(size.X, size.Y);
                size.X = max;
                size.Y = max;
            }
        }

        virtual void Blit(BitmapAtlas* Atlas, Vector2i mount)
        {
            Bitmap* source = getSource();
            Bitmap* target = Atlas->Bitmap;

            Vector2i upperLeft, upperRight, lowerLeft, lowerRight;

            if (rotated && builder->AllowRotation)
            {
                source = source->Rotate(RotateAngle90CW);
                    
                lowerLeft.X = mount.X + builder->Padding;
                lowerLeft.Y = mount.Y + builder->Padding;
                lowerRight.X = mount.X + builder->Padding;
                lowerRight.Y = mount.Y + builder->Padding + source->GetHeight();
                upperRight.X = mount.X + builder->Padding + source->GetWidth();
                upperRight.Y = mount.Y + builder->Padding + source->GetHeight();
                upperLeft.X = mount.X + builder->Padding + source->GetWidth();
                upperLeft.Y = mount.Y + builder->Padding;
            }
            else
            {
                source->AddRef();

                lowerLeft.X = mount.X + builder->Padding;
                lowerLeft.Y = mount.Y + builder->Padding + source->GetHeight();
                lowerRight.X = mount.X + builder->Padding + source->GetWidth();
                lowerRight.Y = mount.Y + builder->Padding + source->GetHeight();
                upperRight.X = mount.X + builder->Padding + source->GetWidth();
                upperRight.Y = mount.Y + builder->Padding;
                upperLeft.X = mount.X + builder->Padding;
                upperLeft.Y = mount.Y + builder->Padding;
            }


            Atlas->Tiles[tile].UpperLeft = upperLeft;
            Atlas->Tiles[tile].UpperRight = upperRight;
            Atlas->Tiles[tile].LowerLeft = lowerLeft;
            Atlas->Tiles[tile].LowerRight = lowerRight;
            Atlas->Tiles[tile].IsRotated = rotated && builder->AllowRotation;

            target->Blit(source, source->GetRect(), Vector2i(mount.X + builder->Padding, mount.Y + builder->Padding));

            // Padding
            for (int i = 0; i < builder->Padding; i++)
            {
                target->Blit(target, Recti(Vector2i(mount.X+builder->Padding, mount.Y+builder->Padding), Vector2i(1, source->GetHeight())), Vector2i(mount.X+i, mount.Y+builder->Padding));
                target->Blit(target, Recti(Vector2i(mount.X+builder->Padding+source->GetWidth()-1, mount.Y+builder->Padding), Vector2i(1, source->GetHeight())), Vector2i(mount.X+builder->Padding+source->GetWidth()+i, mount.Y+builder->Padding));
            }
            for (int i = 0; i < builder->Padding; i++)
            {
                target->Blit(target, Recti(Vector2i(mount.X, mount.Y+builder->Padding), Vector2i(source->GetWidth()+2*builder->Padding, 1)), Vector2i(mount.X, mount.Y+i));
                target->Blit(target, Recti(Vector2i(mount.X, mount.Y+builder->Padding+source->GetHeight()-1), Vector2i(source->GetWidth()+2*builder->Padding, 1)), Vector2i(mount.X, mount.Y+builder->Padding+source->GetHeight()+i));
            }

            // Release rotated copy
            source->Release();
        }

        virtual void Rotate()
        {
            rotated = !rotated;
            Swap(size.X, size.Y);
        }

        virtual Vector2i GetSize()
        {
            return size;
        }
    };

    class DoubleTileNode: public TileNode
    {
        TileNode* a;
        TileNode* b;
        bool rotated;
        Vector2i size;

    public:
        DoubleTileNode(const AtlasBuilder* builder, TileNode* a, TileNode* b): TileNode(builder)
        {
            this->a = a;
            this->b = b;

            rotated = false;
            size.X = Max(a->GetSize().X, b->GetSize().X);
            size.Y = a->GetSize().Y + b->GetSize().Y;

            if (size.Y > size.X) Rotate();
        }

        virtual ~DoubleTileNode()
        {
            delete a;
            delete b;
        }

        virtual void Blit(BitmapAtlas* Atlas, Vector2i aMount)
        {
            Vector2i bMount = aMount;

            if (rotated) bMount.X += a->GetSize().X;
            else bMount.Y += a->GetSize().Y;

            a->Blit(Atlas, aMount);
            b->Blit(Atlas, bMount);
        }

        virtual void Rotate()
        {
            rotated = !rotated;
            Swap(size.X, size.Y);
            a->Rotate();
            b->Rotate();
        }

        virtual Vector2i GetSize()
        {
            return size;
        }
    };

    static int addNode(Array<TileNode*>& nodes, TileNode* node)
    {
        for (int i = 0; i < nodes.Length(); i++)
        {
            if (*nodes[i] < *node)
            {
                nodes.Add();
                for (int j = nodes.Length() - 1; j > i; j--)
                {
                    nodes[j] = nodes[j - 1];
                }
                nodes[i] = node;
                return i;
            }
        }

        return nodes.Add(node);
    }

    static TileNode* popNode(Array<TileNode*>& nodes)
    {
        return nodes.RemoveLast();
    }

    BitmapAtlas* AtlasBuilder::BuildBitmapAtlas()
    {
        BitmapAtlas* atlas = new BitmapAtlas();

        if (!Tiles.Length())
        {
            atlas->Bitmap = new Bitmap(1, 1, PixelFormat);
            atlas->Bitmap->Clear();
            return atlas;
        }

        // Create sorted node-array
        Array<TileNode*> nodes;
        nodes.Reserve(Tiles.Length());
        for (int i = 0; i < Tiles.Length(); i++)
        {
            addNode(nodes, new SingleTileNode(this, i));
        }

        while (nodes.Length() > 1)
        {
            // Pop the two smallest nodes and combine them
            TileNode* a = popNode(nodes);
            TileNode* b = popNode(nodes);
            addNode(nodes, new DoubleTileNode(this, a, b));
        }

        // Pop last node
        TileNode* t = popNode(nodes);   

        // Create Atlas
        atlas->Tiles.Resize(Tiles.Length());
        
        int w = PowerOfTwo? UpperPow2(t->GetSize().X): t->GetSize().X;
        int h = PowerOfTwo? UpperPow2(t->GetSize().Y): t->GetSize().Y;

        if (PowerOfTwo)
        {
            if (w > h) h = w;
            else if (h > w) w = h;
        }
        
        atlas->Bitmap = new Bitmap(w, h, PixelFormat);
        atlas->Bitmap->Clear();

        t->Blit(atlas, Vector2i(0, 0)); 
        delete t;

        return atlas;
    }
}
