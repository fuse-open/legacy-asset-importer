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

#ifndef __XLI_PLATFORM_MESSAGE_BOX_H__
#define __XLI_PLATFORM_MESSAGE_BOX_H__

#include <Xli/Exception.h>

namespace Xli
{
    /**
        \ingroup XliPlatform

        Defines button combinations for dialog boxes.
    */
    enum DialogButtons
    {
        /**
            The dialog contains only an OK button
        */
        DialogButtonsOK = 0,

        /**
            The dialog contains OK and a Cancel button
        */
        DialogButtonsOKCancel = 1,

        /**
            The dialog contains Yes and a No button
        */
        DialogButtonsYesNo = 2,

        /**
            The dialog contains Yes, No and a Cancel button
        */
        DialogButtonsYesNoCancel = 3,

        /**
            The dialog contains a Cancel, Try Again and a Continue button.
            This can also be displayed as Abort, Retry, Ignore on platforms where
            this is more appropriate.
        */
        DialogButtonsCancelTryContinue = 4
    };

    /**
        \ingroup XliPlatform

        Defines button combinations for dialog boxes.
    */
    enum DialogResult
    {
        /**
            The OK button was selected.
        */
        DialogResultOK = 0,

        /**
            The Cancel button was selected.
        */
        DialogResultCancel = 1,

        /**
            The Yes button was selected.
        */
        DialogResultYes = 2,

        /**
            The No button was selected.
        */
        DialogResultNo = 3,

        /**
            The Try Again (or Retry) button was selected.
        */
        DialogResultTryAgain = 4,

        /**
            The Continue (or Ignore) button was selected.
        */
        DialogResultContinue = 5
    };

    /**
        \ingroup XliPlatform

        Defines appearance hints for dialog boxes.
        The window manager chooses how to interpret these hints
    */
    enum DialogHints
    {
        /**
            The dialog displays an error message.
        */
        DialogHintsError = (1<<0),

        /**
            The dialog displays information.
        */
        DialogHintsInformation = (1<<1),

        /**
            The dialog displays a warning message.
        */
        DialogHintsWarning = (1<<2),

        /**
            The dialog displays a question.
        */
        DialogHintsQuestion = (1<<3),

        /**
            The first button is the default choice.
        */
        DialogHintsButton1Default = (1<<4),

        /**
            The second button is the default choice.
        */
        DialogHintsButton2Default = (1<<5),

        /**
            The third button is the default choice.
        */
        DialogHintsButton3Default = (1<<6)
    };

    class Window;

    /**
        \ingroup XliPlatform
    */
    class MessageBox
    {
    public:
        /**
            Displays a message box.
        */
        static DialogResult Show(Window* parent, const String& message, const String& caption, DialogButtons buttons = DialogButtonsOK, int hints = 0);
    };
}

#endif
