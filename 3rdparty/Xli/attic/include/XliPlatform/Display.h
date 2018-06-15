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

#ifndef __XLI_PLATFORM_DISPLAY_H__
#define __XLI_PLATFORM_DISPLAY_H__

#include <Xli/Rectangle.h>

namespace Xli
{
    /**
        \ingroup XliPlatform
    */
    struct DisplaySettings
    {
        Vector2i Resolution;
        int RefreshRate;
        int BitsPerPixel;
        void* DriverData;

        DisplaySettings(): DriverData(0) {}
    };

    /**
        \ingroup XliPlatform
    */
    class Display
    {
    public:
        /**
            Returns the number of screens connected to the system.
        */
        static int GetCount();

        /**
            Returns the coordinate system of the specified screen.
        */
        static Recti GetRect(int index);

        static void GetSupportedSettings(int index, Array<DisplaySettings>& settings);        
        static bool GetCurrentSettings(int index, DisplaySettings& settings);
        static bool ChangeSettings(int index, const DisplaySettings& settings);
        
        static Vector2i GetStatusBarPosition();
        static Vector2i GetStatusBarSize();
        static bool IsStatusBarVisible();
        
        static float GetDensity(int displayIndex);
        static Vector2 GetDpi(int displayIndex);

        /**
           Returns the size of the primary monitor
        */
        static Vector2i GetScreenSize();
    };
}


#endif
