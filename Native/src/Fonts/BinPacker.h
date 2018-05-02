#ifndef __CRUNCH_FONTS_BIN_PACKER_H__
#define __CRUNCH_FONTS_BIN_PACKER_H__

#include <Xli/Array.h>
#include <Xli/Vector2.h>
#include <Xli/Managed.h>

namespace Fonts
{
    using namespace Xli;

    class BinPacker
    {
    public:
        struct PackedRect
        {
            int Index;
            Vector2i Position;
            bool Rotated;

            PackedRect()
                : Index(-1)
            {
            }

            PackedRect(int index, Vector2i pos, bool rot)
                : Index(index), Position(pos), Rotated(rot)
            {
            }
        };

        void Pack(const Array<Vector2i>& rects, Array<Managed<ArrayRef<PackedRect> > >& result, int packSize, bool allowRotation = true);

    private:
        struct Rect
        {
            int  x, y, w, h, ID, children[2];
            bool rotated, packed;

            Rect()
                : x(0), y(0), w(0), h(0), ID(-1), rotated(false), packed(false)
            {
                children[0] = -1;
                children[1] = -1;
            }

            Rect(int size)
                : x(0), y(0), w(size), h(size), ID(-1), rotated(false), packed(false)
            {
                children[0] = -1;
                children[1] = -1;
            }

            Rect(int x, int y, int w, int h, int ID = -1)
                : x(x), y(y), w(w), h(h), ID(ID), rotated(false), packed(false)
            {
                children[0] = -1;
                children[1] = -1;
            }
        
            int GetArea() const
            {
                return w * h;
            }
        
            void Rotate()
            {
                Swap(w, h);
                rotated = !rotated;
            }
        
            bool operator < (const Rect& rect) const 
            {
                return GetArea() > rect.GetArea();
            }
        };

        void Clear();
        void Fill(int pack, bool allowRotation);
        void Split(int pack, int rect);
        bool Fits(Rect& rect1, const Rect& rect2, bool allowRotation);
        void AddPackToArray(int pack, Array<PackedRect>& array) const;
    
        bool RectIsValid(int i) const;
        bool PackIsValid(int i) const;
    
        int m_packSize;
        int m_numPacked;
        Array<Rect> m_rects;
        Array<Rect> m_packs;
        Array<int> m_roots;
    };
}

#endif
