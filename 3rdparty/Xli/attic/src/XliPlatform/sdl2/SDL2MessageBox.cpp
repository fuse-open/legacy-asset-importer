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

#include <XliPlatform/MessageBox.h>
#include <XliPlatform/PlatformSpecific/SDL2.h>

namespace Xli
{
    DialogResult MessageBox::Show(Window* wnd, const String& message, const String& caption, DialogButtons buttons, int hints)
    {
        SDL_MessageBoxData data;
        SDL_MessageBoxButtonData buttondata[3];

        data.flags = 0;
        data.window = PlatformSpecific::SDL2::GetWindowHandle(wnd);
        data.title = caption.Ptr();
        data.message = message.Ptr();
        data.numbuttons = 0;
        data.buttons = buttondata;
        data.colorScheme = 0;

        switch (buttons)
        {
        case DialogButtonsOK:
            buttondata[0].flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
            buttondata[0].buttonid = 0;
            buttondata[0].text = "OK";
            data.numbuttons = 1;
            break;

        case DialogButtonsOKCancel: 
            buttondata[0].flags = 0;
            buttondata[0].buttonid = 0;
            buttondata[0].text = "OK";
            buttondata[1].flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
            buttondata[1].buttonid = 1;
            buttondata[1].text = "Cancel";
            data.numbuttons = 2;
            break;

        case DialogButtonsYesNo: 
            buttondata[0].flags = 0;
            buttondata[0].buttonid = 2;
            buttondata[0].text = "Yes";
            buttondata[1].flags = 0;
            buttondata[1].buttonid = 3;
            buttondata[1].text = "No";
            data.numbuttons = 2;
            break;

        case DialogButtonsYesNoCancel: 
            buttondata[0].flags = 0;
            buttondata[0].buttonid = 2;
            buttondata[0].text = "Yes";
            buttondata[1].flags = 0;
            buttondata[1].buttonid = 3;
            buttondata[1].text = "No";
            buttondata[2].flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
            buttondata[2].buttonid = 1;
            buttondata[2].text = "Cancel";
            data.numbuttons = 3;
            break;

        case DialogButtonsCancelTryContinue: 
            buttondata[0].flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
            buttondata[0].buttonid = 1;
            buttondata[0].text = "Cancel";
            buttondata[1].flags = 0;
            buttondata[1].buttonid = 4;
            buttondata[1].text = "Try Again";
            buttondata[2].flags = 0;
            buttondata[2].buttonid = 5;
            buttondata[2].text = "Continue";
            data.numbuttons = 3;
            break;

        default:
            XLI_THROW("Invalid button combination for MessageDialog");
        }

        if (hints & DialogHintsError) data.flags |= SDL_MESSAGEBOX_ERROR;
        else if (hints & DialogHintsInformation) data.flags |= SDL_MESSAGEBOX_INFORMATION;
        else if (hints & DialogHintsWarning) data.flags |= SDL_MESSAGEBOX_WARNING;
        
        if (hints & DialogHintsButton1Default) buttondata[0].flags |= SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
        else if (hints & DialogHintsButton2Default) buttondata[1].flags |= SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
        else if (hints & DialogHintsButton3Default) buttondata[2].flags |= SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;

        int result = -1;
        if (SDL_ShowMessageBox(&data, &result) == 0)
        {
            switch (result)
            {
            case 0: return DialogResultOK;
            case 1: return DialogResultCancel;
            case 2: return DialogResultYes;
            case 3: return DialogResultNo;
            case 4: return DialogResultTryAgain;
            case 5: return DialogResultContinue;
            }

            return DialogResultCancel;
        }

        XLI_THROW("MessageDialog failed");
    }
}
