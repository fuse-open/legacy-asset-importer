#include "AppDelegate.h"
#include <Xli/Vector2.h>

//  TODO: Associate view directly with Xli::Window, so we don't assume
//  SharedApp()->RootWindow() here

typedef uintptr_t TouchEventId;
typedef Xli::Vector2 TouchPosition;

static inline void processTouchEvents(
    UIView *view, NSSet *touches,
    void(^emitTouchEvent)(TouchPosition, TouchEventId))
{
    CGFloat scale = view.contentScaleFactor;
    for (UITouch *touch in touches)
    {
        CGPoint point = [touch locationInView:view];
        TouchEventId touchId = reinterpret_cast<TouchEventId>(touch);

        emitTouchEvent(TouchPosition(point.x, point.y) * scale, touchId);
    }
}


@interface Xli_AppDelegate (TouchEvents)
@end


@implementation Xli_AppDelegate (TouchEvents)

// Tells the receiver when one or more fingers touch down in a view or window.
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesBegan:touches withEvent:event];

    Xli::Application *application = Xli::Application::SharedApp();
    Xli::Window *window = application->RootWindow();

    processTouchEvents(self.view, touches,
        ^void (TouchPosition position, TouchEventId touchId) {
            application->OnTouchDown(window, position, touchId); });
}

// Tells the receiver when one or more fingers associated with an event move within a view or window.
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesMoved:touches withEvent:event];

    Xli::Application *application = Xli::Application::SharedApp();
    Xli::Window *window = application->RootWindow();

    processTouchEvents(self.view, touches,
        ^void (TouchPosition position, TouchEventId touchId) {
            application->OnTouchMove(window, position, touchId); });
}

// Tells the receiver when one or more fingers are raised from a view or window.
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesEnded:touches withEvent:event];

    Xli::Application *application = Xli::Application::SharedApp();
    Xli::Window *window = application->RootWindow();

    processTouchEvents(self.view, touches,
        ^void (TouchPosition position, TouchEventId touchId) {
            application->OnTouchUp(window, position, touchId); });
}

// Sent to the receiver when a system event (such as a low-memory warning) cancels a touch event.
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesCancelled:touches withEvent:event];

    Xli::Application *application = Xli::Application::SharedApp();
    Xli::Window *window = application->RootWindow();

    processTouchEvents(self.view, touches,
        ^void (TouchPosition position, TouchEventId touchId) {
            application->OnTouchCancel(window, position, touchId); });
}

@end
