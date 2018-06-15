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
#include <Xli/CoreLib.h>
#include <XliPlatform/PlatformSpecific/Android.h>
#include <Xli/MutexQueue.h>
#include <Xli/Console.h>
#include <XliGL/GLContext.h>

#include "AInternal.h"
#include "AJniHelper.h"
#include "AShim.h"
#include "AKeyEvent.h"
#include "ALogStream.h"

static Xli::MutexQueue<Xli::PlatformSpecific::CTAction*> cross_thread_event_queue;
static bool seperateCoreThread = false;

namespace Xli
{
    namespace PlatformSpecific
    {
        void Android::SetLogTag(const char* tag)
        {
            setenv("XLI_APP_NAME", tag, 1);
        }

        JavaVM* Android::GetJavaVM()
        {
            return AJniHelper::GetVM();
        }

        jobject Android::GetActivity()
        {
            return AJniHelper::GetActivity();
        }

        extern "C"
        {
            void JNICALL XliJ_JavaThrowError (JNIEnv* env , jobject obj, jint errorCode, jstring errorMessage)
            {
                char const* cerrorMessage = env->GetStringUTFChars(errorMessage, NULL);
                String finalMessage = String("JavaThrown:") + String(cerrorMessage);
                if (!seperateCoreThread)
                {
                    String m = "XLiError (" + String(errorCode)+ ") - " + finalMessage;
                    XLI_THROW(m.Ptr());
                } else {
                    cross_thread_event_queue.Enqueue(new CTError(finalMessage, errorCode));
                }
                env->ReleaseStringUTFChars(errorMessage, cerrorMessage);
            }

            void JNICALL XliJ_UnoSurfaceReady (JNIEnv* env, jobject obj, jobject unoSurface)
            {
                if (!seperateCoreThread)
                {
                    Application* app = Application::SharedApp();
                    Window* window = app->RootWindow();
                    app->OnNativeHandleChanged(window);
                    window->Show();
                } else {
                    cross_thread_event_queue.Enqueue(new CTSurfaceReady());
                }
            }

            void JNICALL XliJ_SurfaceSizeChanged (JNIEnv* env, jobject obj, int width, int height)
            {
                if (!seperateCoreThread)
                {
                    Application* app = Xli::Application::SharedApp();
                    app->OnSizeChanged(app->RootWindow());
                } else {
                    cross_thread_event_queue.Enqueue(new CTSurfaceSizeChanged());
                }
            }

            void JNICALL XliJ_OnSurfaceTouch(JNIEnv* env, jobject obj, int pointerID, int x, int y, int type)
            {
                if (!seperateCoreThread)
                {
                    Application* app = Xli::Application::SharedApp();
                    Window* win = app->RootWindow();
                    switch (type)
                    {
                    case 0:
                        app->OnTouchMove(win, Vector2(x, y), pointerID);
                        break;
                    case 1:
                        app->OnTouchDown(win, Vector2(x, y), pointerID);
                        break;
                    case 2:
                        app->OnTouchUp(win, Vector2(x, y), pointerID);
                        break;
                    }
                } else {
                    cross_thread_event_queue.Enqueue(new CTTouchEvent(pointerID, x, y, type));
                }
            }

            void JNICALL XliJ_TimerCallback (JNIEnv* env, jobject obj, int timerID)
            {
                if (seperateCoreThread)
                {
                    Android::ProcessCrossThreadEvents();
                }
            }

            void JNICALL XliJ_OnKeyboardResized (JNIEnv* env, jobject obj)
            {
                if (!seperateCoreThread)
                {
                    Application* app = Xli::Application::SharedApp();
                    Window* win = app->RootWindow();
                    app->OnKeyboardResized(win);
                } else {
                    cross_thread_event_queue.Enqueue(new CTKeyboardResize());
                }
            }

            void JNICALL XliJ_OnKeyUp (JNIEnv *env , jobject obj, jint keyCode)
            {
                if (!seperateCoreThread)
                {
                    Application* app = Xli::Application::SharedApp();
                    Window* win = app->RootWindow();
                    app->OnKeyUp(win, AndroidToXliKeyEvent((AKeyEvent)keyCode));
                } else {
                    cross_thread_event_queue.Enqueue(new CTKeyAction((AKeyEvent)keyCode, false));
                }
            }
            void JNICALL XliJ_OnKeyDown (JNIEnv *env , jobject obj, jint keyCode)
            {
                if (!seperateCoreThread)
                {
                    Application* app = Xli::Application::SharedApp();
                    Window* win = app->RootWindow();
                    app->OnKeyDown(win, AndroidToXliKeyEvent((AKeyEvent)keyCode));
                } else {
                    cross_thread_event_queue.Enqueue(new CTKeyAction((AKeyEvent)keyCode, true));
                }
            }
            void JNICALL XliJ_OnTextInput (JNIEnv *env , jobject obj, jstring keyChars)
            {
                const char* jChars = env->GetStringUTFChars((jstring)keyChars, NULL);
                if (!seperateCoreThread)
                {
                    Application* app = Xli::Application::SharedApp();
                    Window* win = app->RootWindow();
                    app->OnTextInput(win, String(jChars));
                } else {
                    cross_thread_event_queue.Enqueue(new CTTextAction(String(jChars)));
                }
                env->ReleaseStringUTFChars((jstring)keyChars, jChars);
            }
            
            void JNICALL cppOnDestroy(JNIEnv *env , jobject obj)
            {
                Application::SharedApp()->Terminate();
            }
            void JNICALL cppOnPause(JNIEnv *env , jobject obj)
            {
                Application::SharedApp()->ExitInteractive();
            }
            void JNICALL cppOnResume(JNIEnv *env , jobject obj)
            {
                Application::SharedApp()->Start();
            }
            void JNICALL cppOnStart(JNIEnv *env , jobject obj)
            {
            }
            void JNICALL cppOnStop(JNIEnv *env , jobject obj)
            {
                Application::SharedApp()->EnterBackground();
            }
            void JNICALL cppOnSaveInstanceState(JNIEnv *env , jobject obj, jobject state)
            {
            }
            void JNICALL cppOnConfigChanged(JNIEnv *env , jobject obj)
            {
            }
            void JNICALL cppOnLowMemory(JNIEnv *env , jobject obj)
            {
                Application::SharedApp()->OnLowMemory();
            }
            void JNICALL cppOnWindowFocusChanged(JNIEnv *env , jobject obj, bool hasFocus)
            {
                if (hasFocus)
                {
                    Application::SharedApp()->EnterInteractive();
                } else {
                    Application::SharedApp()->ExitInteractive();
                }
            }
            void JNICALL cppOnVSync (JNIEnv* env, jobject obj, int milliseconds)
            {
                Application* app = Application::SharedApp();
                Window* window = app->RootWindow();
                // if (seperateCoreThread)
                // {
                //     Android::ProcessCrossThreadEvents();
                // }
                if (window->CurrentState() == Window::Visible)
                {
                    window->GetContext()->MakeCurrent(window);
                    app->OnUpdateFrame();
                }
            }        
        }

        void Android::ProcessCrossThreadEvents()
        {
            while ((cross_thread_event_queue.Count() > 0))
            {
                CTAction* action = cross_thread_event_queue.Dequeue();
                action->Execute();
                delete action;
            }
        }

        static void AttachNativeCallbacks(JNIEnv* l_env, jclass shim_class)
        {
            LOGD("Registering native functions");
            static JNINativeMethod native_funcs[] = {
                {(char* const)"XliJ_JavaThrowError", (char* const)"(ILjava/lang/String;)V", (void *)&XliJ_JavaThrowError},
                {(char* const)"XliJ_UnoSurfaceReady", (char* const)"(Landroid/view/Surface;)V", (void *)&XliJ_UnoSurfaceReady},
                {(char* const)"XliJ_SurfaceSizeChanged", (char* const)"(II)V", (void *)&XliJ_SurfaceSizeChanged},
                {(char* const)"XliJ_OnSurfaceTouch", (char* const)"(IIII)V", (void *)&XliJ_OnSurfaceTouch},
                {(char* const)"XliJ_TimerCallback", (char* const)"(I)V", (void *)&XliJ_TimerCallback},
                {(char* const)"XliJ_OnKeyboardResized", (char* const)"()V", (void *)&XliJ_OnKeyboardResized},
                {(char* const)"XliJ_OnKeyUp", (char* const)"(I)V", (void *)&XliJ_OnKeyUp},
                {(char* const)"XliJ_OnKeyDown", (char* const)"(I)V", (void *)&XliJ_OnKeyDown},
                {(char* const)"XliJ_OnTextInput", (char* const)"(Ljava/lang/String;)V", (void *)&XliJ_OnTextInput},

                {(char* const)"cppOnDestroy", (char* const)"()V", (void *)&cppOnDestroy},
                {(char* const)"cppOnPause", (char* const)"()V", (void *)&cppOnPause},
                {(char* const)"cppOnResume", (char* const)"()V", (void *)&cppOnResume},
                {(char* const)"cppOnStart", (char* const)"()V", (void *)&cppOnStart},
                {(char* const)"cppOnStop", (char* const)"()V", (void *)&cppOnStop},
                {(char* const)"cppOnSaveInstanceState", (char* const)"(Landroid/os/Bundle;)V", (void *)&cppOnSaveInstanceState},
                {(char* const)"cppOnConfigChanged", (char* const)"()V", (void *)&cppOnConfigChanged},
                {(char* const)"cppOnLowMemory", (char* const)"()V", (void *)&cppOnLowMemory},
                {(char* const)"cppOnWindowFocusChanged", (char* const)"(Z)V", (void *)&cppOnWindowFocusChanged},
                {(char* const)"cppOnVSync", (char* const)"(I)V", (void *)&cppOnVSync},
            };
            // the last argument is the number of native functions
            jint attached = l_env->RegisterNatives(shim_class, native_funcs, 19);
            if (attached < 0) {
                LOGE("COULD NOT REGISTER NATIVE FUNCTIONS");
                XLI_THROW("COULD NOT REGISTER NATIVE FUNCTIONS");
            } else {
                LOGD("Native functions registered");
            }
        }

        void Android::OnJNILoad(JavaVM* vm, JNIEnv* env, jclass shim_class)
        {
            Xli::PlatformSpecific::AJniHelper::Init(vm, env, shim_class);
            AttachNativeCallbacks(env, shim_class);
        }

        void Android::Init(bool sepCoreThread)
        {
            seperateCoreThread = sepCoreThread;
            Out->SetStream(ManagePtr(new ALogStream(ANDROID_LOG_INFO)));
            Error->SetStream(ManagePtr(new ALogStream(ANDROID_LOG_WARN)));
        }
    }
}
