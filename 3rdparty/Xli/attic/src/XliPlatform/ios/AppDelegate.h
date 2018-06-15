#pragma once

#include <UIKit/UIKit.h>

#include <Xli/Rectangle.h>
#include <XliPlatform/Application.h>

@interface Xli_AppDelegate : UIViewController<UIApplicationDelegate>
{
    @public Xli::Recti _Xli_keyboardBounds;
}

@property (nonatomic, retain) UITextField *Xli_textField;

- (void)Xli_OnUpdateFrame:(id)sender;

@end

@interface Xli_AppDelegate (KeyboardInput) <UITextFieldDelegate>
- (void)Xli_keyboardWillShowOrHide:(NSNotification *)notification;

- (BOOL)Xli_isKeyboardVisible;

- (void)Xli_showKeyboardWithHint:(Xli::TextInputHint)hint;
- (void)Xli_hideKeyboard;
@end
