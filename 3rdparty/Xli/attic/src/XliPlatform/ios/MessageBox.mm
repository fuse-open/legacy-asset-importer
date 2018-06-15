#include <XliPlatform/Application.h>
#include <XliPlatform/MessageBox.h>
#include <UIKit/UIKit.h>


@interface Xli_MessageBox : UIAlertView <UIAlertViewDelegate>
{
    @package
    bool dismissed_;
    Xli::DialogButtons dialogButtons_;
    Xli::DialogResult dialogResult_;
}

- (id)initWithCaption:(Xli::String const &)caption
    message:(Xli::String const &)message
    dialogButtons:(Xli::DialogButtons)buttons;
@end


@implementation Xli_MessageBox

- (id)initWithCaption:(Xli::String const &)caption
    message:(Xli::String const &)message
    dialogButtons:(Xli::DialogButtons)buttons
{
    NSString *title = [NSString stringWithUTF8String:caption.Ptr()];
    NSString *msg = [NSString stringWithUTF8String:message.Ptr()];

    NSString *cancelButton = nil;
    NSString *button1 = nil;
    NSString *button2 = nil;

    switch (buttons)
    {
        case Xli::DialogButtonsOK:
            button1 = @"Ok";
            break;

        case Xli::DialogButtonsOKCancel:
            cancelButton = @"Cancel";
            button1 = @"Ok";
            break;

        case Xli::DialogButtonsYesNo:
            button1 = @"Yes";
            button2 = @"No";
            break;

        case Xli::DialogButtonsYesNoCancel:
            cancelButton = @"Cancel";
            button1 = @"Yes";
            button2 = @"No";
            break;

        case Xli::DialogButtonsCancelTryContinue:
            cancelButton = @"Cancel";
            button1 = @"Try Again";
            button2 = @"Continue";
            break;
    }

    self = [super initWithTitle:title message:msg delegate:self
        cancelButtonTitle:cancelButton otherButtonTitles:button1, button2, nil];

    if (self)
    {
        dismissed_ = false;
        dialogButtons_ = buttons;
        dialogResult_ = Xli::DialogResultCancel;
    }

    return self;
}

- (void)alertView:(Xli_MessageBox *)alertView
    didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    assert(alertView == self);

    NSInteger firstButton = self.firstOtherButtonIndex;
    switch (dialogButtons_)
    {
        case Xli::DialogButtonsOK:
        case Xli::DialogButtonsOKCancel:
            if (buttonIndex == firstButton)
                dialogResult_ = Xli::DialogResultOK;
            break;

        case Xli::DialogButtonsYesNo:
            if (buttonIndex == firstButton)
                dialogResult_ = Xli::DialogResultYes;
            else if (buttonIndex == firstButton + 1)
                dialogResult_ = Xli::DialogResultNo;
            break;

        case Xli::DialogButtonsYesNoCancel:
            if (buttonIndex == firstButton)
                dialogResult_ = Xli::DialogResultYes;
            else if (buttonIndex == firstButton + 1)
                dialogResult_ = Xli::DialogResultNo;
            break;

        case Xli::DialogButtonsCancelTryContinue:
            if (buttonIndex == firstButton)
                dialogResult_ = Xli::DialogResultTryAgain;
            else if (buttonIndex == firstButton + 1)
                dialogResult_ = Xli::DialogResultContinue;
            break;
    }

    dismissed_ = true;
}

@end


namespace Xli
{
    DialogResult MessageBox::Show(
        Window* wnd, const String& message, const String& caption,
        DialogButtons buttons, int hints)
    {
        struct Finalizer
        {
            Finalizer()
            {
                // Not caching SharedApp() as the event loop will run.
                Application::SharedApp()->ExitInteractive();
                pool_ = [[NSAutoreleasePool alloc] init];
            }

            ~Finalizer()
            {
                if (UIApplicationStateActive
                    == [UIApplication sharedApplication].applicationState)
                {
                    Application::SharedApp()->EnterInteractive();
                }

                [pool_ drain];
            }

            NSAutoreleasePool *pool_;
        };

        Finalizer finalizer;

        Xli_MessageBox *alert = [[Xli_MessageBox alloc]
            initWithCaption:caption message:message dialogButtons:buttons];

        [alert autorelease];
        [alert show];

        NSRunLoop *runLoop = [NSRunLoop currentRunLoop];
        NSDate *date = [NSDate distantFuture];

        do { } while (!alert->dismissed_
            && [runLoop runMode:NSDefaultRunLoopMode beforeDate:date]);

        return alert->dialogResult_;
    }
}
