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
#include <Xli/Console.h>

namespace Xli
{
    DialogResult MessageBox::Show(Window* parent, const String& message, const String& caption, DialogButtons buttons, int hints)
    {
        PrintLine("");
        PrintLine("---------------------------");
        PrintLine(caption);
        PrintLine("---------------------------");
        PrintLine(message);
        PrintLine("---------------------------");

        DialogResult r;

        while (true)
        {
            switch (buttons)
            {
            case DialogButtonsOK: Print("OK"); break;
            case DialogButtonsOKCancel: Print("OK/Cancel"); break;
            case DialogButtonsYesNo: Print("Yes/No"); break;
            case DialogButtonsYesNoCancel: Print("Yes/No/Cancel"); break;
            case DialogButtonsCancelTryContinue: Print("Cancel/Retry/Continue"); break;
            }

            Print(": ");
            String in = In->ReadLine().Trim().ToLower();

            if (buttons == DialogButtonsOK) { r = DialogResultOK; break; }
            else if ((buttons == DialogButtonsOKCancel) && (in == "ok")) { r = DialogResultOK; break; }
            else if ((buttons == DialogButtonsYesNo || buttons == DialogButtonsYesNoCancel) && (in == "yes" || in == "y")) { r = DialogResultYes; break; }
            else if ((buttons == DialogButtonsYesNo || buttons == DialogButtonsYesNoCancel) && (in == "no" || in == "n")) { r = DialogResultNo; break; }
            else if ((buttons == DialogButtonsOKCancel || buttons == DialogButtonsYesNoCancel || buttons == DialogButtonsCancelTryContinue) && (in == "cancel")) { r = DialogResultCancel; break; }
            else if ((buttons == DialogButtonsCancelTryContinue) && (in == "retry")) { r = DialogResultTryAgain; break; }
            else if ((buttons == DialogButtonsCancelTryContinue) && (in == "continue")) { r = DialogResultContinue; break; }
        }

        PrintLine("---------------------------");

        return r;
    }
}
