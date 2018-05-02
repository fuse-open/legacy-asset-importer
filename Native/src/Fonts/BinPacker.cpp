#include <Fonts/BinPacker.h>

namespace Fonts
{
    void BinPacker::Pack(const Array<Vector2i>& rects, Array<Managed<ArrayRef<PackedRect> > >& result, int packSize, bool allowRotation)
    {
        Clear();

        m_packSize = packSize;

        // Add rects to member array, and check to make sure none is too big
        for (int i = 0; i < rects.Length(); i++) 
        {
            if (rects[i].X > m_packSize || rects[i].Y > m_packSize)
                XLI_THROW("All rect dimensions must be <= the pack size");
        
            m_rects.Add(Rect(0, 0, rects[i].X, rects[i].Y, i));
        }

        // Sort from greatest to least area
        m_rects.Sort();

        // Pack
        while (m_numPacked < (int)m_rects.Length())
        {
            int i = m_packs.Length();
            m_packs.Add(Rect(m_packSize));
            m_roots.Add(i);
            Fill(i, allowRotation);
        }

        // Write out
        result.Resize(m_roots.Length());
        for (int i = 0; i < m_roots.Length(); ++i) 
        {
            result[i] = new ArrayRef<PackedRect>();
            AddPackToArray(m_roots[i], result[i]->Data);
        }

        // Check and make sure all rects were packed
        for (int i = 0; i < m_rects.Length(); ++i)
            if (!m_rects[i].packed)
                XLI_THROW("Not all rects were packed");
    }

    void BinPacker::Clear()
    {
        m_packSize = 0;
        m_numPacked = 0;
        m_rects.Clear();
        m_packs.Clear();
        m_roots.Clear();
    }

    void BinPacker::Fill(int pack, bool allowRotation)
    {
        if (!PackIsValid(pack)) 
            XLI_THROW("Invalid pack");

        int i = pack;

        // For each rect
        for (int j = 0; j < m_rects.Length(); ++j)
        {
            // If it's not already packed
            if (!m_rects[j].packed)
            {
                // If it fits in the current working area
                if (Fits(m_rects[j], m_packs[i], allowRotation))
                {
                    // Store in lower-left of working area, split, and recurse
                    ++m_numPacked;
                    Split(i, j);
                    Fill(m_packs[i].children[0], allowRotation);
                    Fill(m_packs[i].children[1], allowRotation);
                    return;
                }
            }
        }
    }

    void BinPacker::Split(int pack, int rect)
    {
        if (!PackIsValid(pack)) 
            XLI_THROW("Invalid pack");

        if (!RectIsValid(rect)) 
            XLI_THROW("Invalid rect");

        int i = pack;
        int j = rect;

        // Split the working area either horizontally or vertically with respect
        // to the rect we're storing, such that we get the largest possible child
        // area.

        Rect left = m_packs[i];
        Rect right = m_packs[i];
        Rect bottom = m_packs[i];
        Rect top = m_packs[i];

        left.y += m_rects[j].h;
        left.w = m_rects[j].w;
        left.h -= m_rects[j].h;
        right.x += m_rects[j].w;
        right.w -= m_rects[j].w;

        bottom.x += m_rects[j].w;
        bottom.h = m_rects[j].h;
        bottom.w -= m_rects[j].w;
        top.y += m_rects[j].h;
        top.h -= m_rects[j].h;

        int maxLeftRightArea = left.GetArea();
        if (right.GetArea() > maxLeftRightArea)
            maxLeftRightArea = right.GetArea();

        int maxBottomTopArea = bottom.GetArea();
        if (top.GetArea() > maxBottomTopArea)
            maxBottomTopArea = top.GetArea();

        if (maxLeftRightArea > maxBottomTopArea)
        {
            if (left.GetArea() > right.GetArea())
            {
                m_packs.Add(left);
                m_packs.Add(right);
            } 
            else 
            {
                m_packs.Add(right);
                m_packs.Add(left);
            }
        } 
        else 
        {
            if (bottom.GetArea() > top.GetArea())
            {
                m_packs.Add(bottom);
                m_packs.Add(top);
            } 
            else 
            {
                m_packs.Add(top);
                m_packs.Add(bottom);
            }
        }

        // This pack area now represents the rect we've just stored, so save the
        // relevant info to it, and assign children.
        m_packs[i].w = m_rects[j].w;
        m_packs[i].h = m_rects[j].h;
        m_packs[i].ID = m_rects[j].ID;
        m_packs[i].rotated = m_rects[j].rotated;
        m_packs[i].children[0] = m_packs.Length() - 2;
        m_packs[i].children[1] = m_packs.Length() - 1;

        // Done with the rect
        m_rects[j].packed = true;
    }

    bool BinPacker::Fits(Rect& rect1, const Rect& rect2, bool allowRotation)
    {
        // Check to see if rect1 fits in rect2, and rotate rect1 if that will
        // enable it to fit.

        if (rect1.w <= rect2.w && rect1.h <= rect2.h) 
        {
            return true;
        }
        else if (allowRotation && rect1.h <= rect2.w && rect1.w <= rect2.h) 
        {
            rect1.Rotate();
            return true;
        } 
        else 
        {
            return false;
        }
    }

    void BinPacker::AddPackToArray(int pack, Array<PackedRect>& result) const
    {
        if (!PackIsValid(pack)) 
            XLI_THROW("Invalid pack");

        int i = pack;
        if (m_packs[i].ID != -1)
        {
            result.Add(PackedRect(m_packs[i].ID, Vector2i(m_packs[i].x, m_packs[i].y), m_packs[i].rotated != 0));

            if (m_packs[i].children[0] != -1)
                AddPackToArray(m_packs[i].children[0], result);

            if (m_packs[i].children[1] != -1)
                AddPackToArray(m_packs[i].children[1], result);
        }
    }

    bool BinPacker::RectIsValid(int i) const
    {
        return i >= 0 && i < (int)m_rects.Length();
    }

    bool BinPacker::PackIsValid(int i) const
    {
        return i >= 0 && i < (int)m_packs.Length();
    }
}
