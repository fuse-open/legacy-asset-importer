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
#include <XliPlatform/Window.h>
#include <XliPlatform/PlatformSpecific/Win32.h>
#include <Xli/Unicode.h>

namespace Xli
{
    DialogResult MessageBox::Show(Window* wnd, const String& message, const String& caption, DialogButtons buttons, int hints)
    {
        UINT type = 0;

        switch (buttons)
        {
        case DialogButtonsOK: type |= MB_OK; break;
        case DialogButtonsOKCancel: type |= MB_OKCANCEL; break;
        case DialogButtonsYesNo: type |= MB_YESNO; break;
        case DialogButtonsYesNoCancel: type |= MB_YESNOCANCEL; break;
        case DialogButtonsCancelTryContinue: type |= MB_CANCELTRYCONTINUE; break;
        default:
            XLI_THROW("Invalid button combination for MessageDialog");
        }

        if (hints & DialogHintsError) type |= MB_ICONERROR;
        else if (hints & DialogHintsInformation) type |= MB_ICONINFORMATION;
        else if (hints & DialogHintsQuestion) type |= MB_ICONQUESTION;
        else if (hints & DialogHintsWarning) type |= MB_ICONWARNING;
        
        if (hints & DialogHintsButton1Default) type |= MB_DEFBUTTON1;
        else if (hints & DialogHintsButton2Default) type |= MB_DEFBUTTON2;
        else if (hints & DialogHintsButton3Default) type |= MB_DEFBUTTON3;

        Utf16String messageW = Unicode::Utf8To16(message);
        Utf16String captionW = Unicode::Utf8To16(caption);
        int result = MessageBoxW(PlatformSpecific::Win32::GetWindowHandle(wnd), messageW.Ptr(), captionW.Ptr(), type);

        switch (result)
        {
        case IDOK: return DialogResultOK;
        case IDCANCEL: return DialogResultCancel;
        case IDCONTINUE: return DialogResultContinue;
        case IDIGNORE: return DialogResultContinue;
        case IDNO: return DialogResultNo;
        case IDTRYAGAIN: return DialogResultTryAgain;
        case IDYES: return DialogResultYes;
        default:
            XLI_THROW("MessageDialog failed: " + PlatformSpecific::Win32::GetLastErrorString());
        }
    }
}
