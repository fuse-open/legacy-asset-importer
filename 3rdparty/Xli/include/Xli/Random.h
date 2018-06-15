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

#ifndef __XLI_RANDOM_H__
#define __XLI_RANDOM_H__

#include <Xli/Vector2.h>
#include <Xli/Vector3.h>
#include <Xli/Vector4.h>

namespace Xli
{
    /**
        \ingroup XliUtils

        A simple 32-bit pseudo-random number generator
    */
    class Random
    {
        UInt32 z, w;

    public:
        Random(UInt32 seed = 1);
        
        void SetSeed(UInt32 seed);

        /**
            Returns a UInt32 in range [0..UInt32Max]
        */
        UInt32 NextUInt32();

        /**
            Returns an int in range [0..high-1]
        */
        int NextInt(int high);

        /**
            Returns an int in range [low..high-1]
        */
        int NextInt(int low, int high);

        /**
            Returns a float in range [0..1]
        */
        float NextFloat();

        /**
            Returns a float in range [min..max]
        */
        float NextFloat(float min, float max);

        /**
            Returns a Vector2 in range [min..max]
        */
        Vector2 NextVector2(float min, float max);

        /**
            Returns a random Vector3 in range [min..max]
        */
        Vector3 NextVector3(float min, float max);

        /**
            Returns a random Vector4 in range [min..max]
        */
        Vector4 NextVector4(float min, float max);
    };

    /**
        \addtogroup XliUtils
        @{
    */

    /**
        Global Random object
    */
    extern Random Rand;

    /** @} */
}

#endif

