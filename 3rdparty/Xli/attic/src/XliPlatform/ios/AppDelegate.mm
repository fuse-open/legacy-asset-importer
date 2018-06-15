#include "AppDelegate.h"
#include <XliPlatform/Application.h>
#include <XliPlatform/Window.h>
#include <Xli/Console.h>

@implementation Xli_AppDelegate

- (void)Xli_OnUpdateFrame:(id)sender
{
    Xli::Application::SharedApp()->OnUpdateFrame();
}

- (id)init
{
    if ((self = [super init]))
    {
        NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
        [center addObserver:self selector:@selector(Xli_keyboardWillShowOrHide:)
            name:UIKeyboardWillShowNotification object:nil];
        [center addObserver:self selector:@selector(Xli_keyboardWillShowOrHide:)
            name:UIKeyboardWillHideNotification object:nil];
    }
    return self;
}

// Tells the delegate that the launch process has begun but that state restoration has not yet occurred.
- (BOOL)application:(UIApplication *)application willFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    Xli::PrintLine("------------------------------------------------------------ Test2");
    Xli::Application::SharedApp()->Start();
    return YES;
}

// Tells the delegate that the app is about to enter the foreground.
- (void)applicationWillEnterForeground:(UIApplication *)application
{
    Xli::Application::SharedApp()->EnterForeground();
}

// Tells the delegate that the app has become active.
- (void)applicationDidBecomeActive:(UIApplication *)application
{
    Xli::Application::SharedApp()->EnterInteractive();
}

// Tells the delegate that the app is about to become inactive.
- (void)applicationWillResignActive:(UIApplication *)application
{
    Xli::Application::SharedApp()->ExitInteractive();
}

// Tells the delegate that the app is now in the background.
- (void)applicationDidEnterBackground:(UIApplication *)application
{
    Xli::Application::SharedApp()->EnterBackground();
}

// Tells the delegate when the app is about to terminate.
- (void)applicationWillTerminate:(UIApplication *)application
{
    Xli::Application::SharedApp()->Terminate();
}

// Tells the delegate when the app receives a memory warning from the system.
- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
    Xli::Application::SharedApp()->OnLowMemory();
}

@end
