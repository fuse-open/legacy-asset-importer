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

#include <UIKit/UIKit.h>
#include <Xli/Array.h>
#include <XliPlatform/Display.h>

namespace Xli
{
    int Display::GetCount()
    {
        return 1;
    }

    Vector2i Display::GetScreenSize() { return Vector2i(0, 0); }
    Recti Display::GetRect(int index)
    {
        assert(index == 0);

        CGRect screenBounds = [[UIScreen mainScreen] bounds];
        return Recti(
            screenBounds.origin.x, screenBounds.origin.y,
            screenBounds.size.width, screenBounds.size.height);
    }

    bool Display::GetCurrentSettings(int index, DisplaySettings& settings)
    {
        assert(index == 0);

        UIScreen *screen = [UIScreen mainScreen];
        CGFloat scale = screen.scale;
        CGSize screenSize = screen.bounds.size;

        settings.Resolution.X = screenSize.width * scale;
        settings.Resolution.Y = screenSize.height * scale;
        settings.RefreshRate = 60;
        settings.BitsPerPixel = 32;
        settings.DriverData = 0;

        return true;
    }

    void Display::GetSupportedSettings(int index, Array<DisplaySettings>& settings)
    {
        assert(index == 0);

        int j = settings.Add();
        GetCurrentSettings(index, settings[j]);
    }

    bool Display::ChangeSettings(int index, const DisplaySettings& settings)
    {
        return false;
    }

    float Display::GetDensity(int displayIndex)
    {
        assert(index == 0);
        return [[UIScreen mainScreen] scale];
    }

    Vector2 Display::GetDpi(int displayIndex)
    {
        assert(index == 0);

        CGFloat dpi = 163.0 * [[UIScreen mainScreen] scale];
        return Vector2(dpi, dpi);
    }

    bool Display::IsStatusBarVisible()
    {
        return ![UIApplication sharedApplication].statusBarHidden;
    }

    Vector2i Display::GetStatusBarPosition()
    {
        CGFloat scale = [[UIScreen mainScreen] scale];
        CGPoint position
            = [UIApplication sharedApplication].statusBarFrame.origin;
        return Vector2i(position.x * scale, position.y * scale);
    }

    Vector2i Display::GetStatusBarSize()
    {
        CGFloat scale = [[UIScreen mainScreen] scale];
        CGSize size = [UIApplication sharedApplication].statusBarFrame.size;
        return Vector2i(size.width * scale, size.height * scale);
    }
}
