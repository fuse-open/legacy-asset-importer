#include <XliPlatform/Window.h>
#include "AppDelegate.h"

//  TODO: Associate view directly with Xli::Window, so we don't assume
//  SharedApp()->RootWindow() here

@interface Xli_AppDelegate (ViewEvents)
@end


@implementation Xli_AppDelegate (ViewEvents)

// Notifies the view controller that its view is about to be added to a view hierarchy.
- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    Xli::Application::SharedApp()->RootWindow()->Show();
}

// Notifies the view controller that its view was removed from a view hierarchy.
- (void)viewDidDisappear:(BOOL)animated
{
    Xli::Application::SharedApp()->RootWindow()->Hide();
    [super viewDidDisappear:animated];
}

- (void)viewWillLayoutSubviews
{
    Xli::Application *application = Xli::Application::SharedApp();
    Xli::Window *window = application->RootWindow();

    window->Hide();
    window->Show();

    application->OnSizeChanged(window);
}

@end
