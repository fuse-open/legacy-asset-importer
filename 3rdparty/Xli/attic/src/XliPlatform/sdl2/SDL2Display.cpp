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

#include <XliPlatform/Display.h>
#include <Xli/Array.h>
#include <SDL.h>

namespace Xli
{
    int Display::GetCount()
    {
        return SDL_GetNumVideoDisplays();
    }

    Recti Display::GetRect(int index)
    {
        SDL_Rect rect;
        SDL_GetDisplayBounds(index, &rect);
        return Recti(Vector2i(rect.x, rect.y), Vector2i(rect.w, rect.h));
    }

    bool Display::GetCurrentSettings(int index, DisplaySettings& settings)
    {
        SDL_DisplayMode mode;
        if (SDL_GetCurrentDisplayMode(index, &mode) == 0)
        {
            settings.Resolution.X = mode.w;
            settings.Resolution.Y = mode.h;
            settings.BitsPerPixel = mode.format;
            settings.RefreshRate = mode.refresh_rate;
            settings.DriverData = mode.driverdata;
            return true;
        }

        return false;
    }

    void Display::GetSupportedSettings(int index, Array<DisplaySettings>& settings)
    {
        SDL_DisplayMode mode;
        for (int i = 0, l = SDL_GetNumDisplayModes(index); i < l; i++)
        {
            int j = settings.Add();
            settings[j].Resolution.X = mode.w;
            settings[j].Resolution.Y = mode.h;
            settings[j].BitsPerPixel = mode.format;
            settings[j].RefreshRate = mode.refresh_rate;
            settings[j].DriverData = mode.driverdata;
        }
    }

    bool Display::ChangeSettings(int index, const DisplaySettings& settings)
    {
        SDL_DisplayMode mode;
        mode.w = settings.Resolution.X;
        mode.h = settings.Resolution.Y;
        mode.format = settings.BitsPerPixel;
        mode.refresh_rate = settings.RefreshRate;
        mode.driverdata = settings.DriverData;
        // TODO
        return false;
    }

#if !defined(XLI_PLATFORM_OSX)
    float Display::GetDensity(int displayIndex)
    {
        // TODO
        return 1.0f;
    }

    Vector2 Display::GetDpi(int displayIndex)
    {
        // TODO
        return Vector2(72, 72);
    }
#endif
}
