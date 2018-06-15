#include "AppDelegate.h"
#include <UIKit/UIKit.h>

static UIKeyboardType _Xli_keyboardForHint(Xli::TextInputHint hint)
{
    switch (hint)
    {
        case Xli::TextInputHintEmail:
            return UIKeyboardTypeEmailAddress;

        case Xli::TextInputHintURL:
            return UIKeyboardTypeURL;

        case Xli::TextInputHintPhone:
            return UIKeyboardTypePhonePad;

        case Xli::TextInputHintNumber:
            return UIKeyboardTypeDecimalPad;

        case Xli::TextInputHintDefault:
        default:
            break;
    }

    return UIKeyboardTypeDefault;

    //  Available keyboard types:
    //
    //      return UIKeyboardTypeDefault;
    //      return UIKeyboardTypeASCIICapable;
    //      return UIKeyboardTypeNumbersAndPunctuation;
    //      return UIKeyboardTypeURL;
    //      return UIKeyboardTypeNumberPad;
    //      return UIKeyboardTypePhonePad;
    //      return UIKeyboardTypeNamePhonePad;
    //      return UIKeyboardTypeEmailAddress;
    //      return UIKeyboardTypeDecimalPad;
    //      return UIKeyboardTypeTwitter;
    //      return UIKeyboardTypeWebSearch;
}

static UITextField *_Xli_initializeKeyboard(
    UIViewController<UITextFieldDelegate> *delegate)
{
    UITextField *textField = [[UITextField alloc] initWithFrame:CGRectZero];

    textField.autocapitalizationType = UITextAutocapitalizationTypeNone;
    textField.autocorrectionType = UITextAutocorrectionTypeNo;
    textField.spellCheckingType = UITextSpellCheckingTypeNo;
    textField.enablesReturnKeyAutomatically = NO;
    textField.keyboardAppearance = UIKeyboardAppearanceDefault;
    textField.returnKeyType = UIReturnKeyDefault;
    textField.secureTextEntry = NO;

    textField.hidden = YES;

    // Something to delete.
    textField.text = @" ";

    textField.delegate = delegate;

    return [textField autorelease];
}

@implementation Xli_AppDelegate (KeyboardInput)

- (void)Xli_keyboardWillShowOrHide:(NSNotification *)notification
{
    CGFloat scale = self.view.contentScaleFactor;
    CGRect frame = [[notification.userInfo
        objectForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue];

    self->_Xli_keyboardBounds = Xli::Recti(
        Xli::Vector2i(frame.origin.x, frame.origin.y) * scale,
        Xli::Vector2i(frame.size.width, frame.size.height) * scale);

    Xli::Application *application = Xli::Application::SharedApp();
    Xli::Window *window = application->RootWindow();

    application->OnKeyboardResized(window);
}

- (BOOL)Xli_isKeyboardVisible
{
    UITextField *textField = self.Xli_textField;
    return textField && textField.superview;
}

- (void)Xli_showKeyboardWithHint:(Xli::TextInputHint)hint
{
    UITextField *textField = self.Xli_textField;
    if (!textField)
        self.Xli_textField = textField = _Xli_initializeKeyboard(self);

    textField.keyboardType = _Xli_keyboardForHint(hint);

    [self.view addSubview:textField];
    [textField becomeFirstResponder];
}

- (void)Xli_hideKeyboard
{
    [self.Xli_textField resignFirstResponder];
}

- (void)textFieldDidEndEditing:(UITextField *)textField
{
    assert(textField == self.Xli_textField);
    [textField removeFromSuperview];
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    assert(textField == self.Xli_textField);

    Xli::Application *application = Xli::Application::SharedApp();
    Xli::Window *window = application->RootWindow();

    if (range.length)
    {
        application->OnKeyDown(window, Xli::KeyBackspace);
        application->OnKeyUp(window, Xli::KeyBackspace);
    }
    else if ([string length])
        application->OnTextInput(window, Xli::String([string UTF8String]));

    // Reject all edits
    return NO;
}

- (BOOL)textFieldShouldClear:(UITextField *)textField
{
    assert(textField == textField_);

    Xli::Application *application = Xli::Application::SharedApp();
    Xli::Window *window = application->RootWindow();

    application->OnClearTextInput(window);

    // Reject all edits
    return NO;
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    assert(textField == textField_);

    Xli::Application *application = Xli::Application::SharedApp();
    Xli::Window *window = application->RootWindow();

    application->OnKeyDown(window, Xli::KeyEnter);
    application->OnKeyUp(window, Xli::KeyEnter);

    return YES;
}

@end
