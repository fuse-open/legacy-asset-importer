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

#include <Xli/Random.h>

namespace Xli
{
    Random::Random(UInt32 seed)
    {
        SetSeed(seed);
    }

    void Random::SetSeed(UInt32 seed)
    {
        if (!seed) seed = 1;
        z = seed;
        w = seed + 1;
    }

    UInt32 Random::NextUInt32()
    {
        z = 36969 * (z & UInt16Max) + (z >> 16);
        w = 18000 * (w & UInt16Max) + (w >> 16);
        return (z << 16) + w;
    }

    int Random::NextInt(int high)
    {
        return (int)(NextFloat() * (float)high);
    }

    int Random::NextInt(int low, int high)
    {
        return (int)(NextFloat() * (float)(high - low) + (float)low);
    }

    float Random::NextFloat()
    {
        return ((float)NextUInt32() / (float)UInt32Max);
    }

    float Random::NextFloat(float min, float max)
    {
        return NextFloat() * (max - min) + min;
    }
    
    Vector2 Random::NextVector2(float min, float max)
    {
        return Vector2(NextFloat(min, max), NextFloat(min, max));
    }

    Vector3 Random::NextVector3(float min, float max)
    {
        return Vector3(NextFloat(min, max), NextFloat(min, max), NextFloat(min, max));
    }

    Vector4 Random::NextVector4(float min, float max)
    {
        return Vector4(NextFloat(min, max), NextFloat(min, max), NextFloat(min, max), NextFloat(min, max));
    }


    Random Rand;
}
