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

#include <Xli/Array.h>
#include <Xli/CoreLib.h>
#include <cstdlib>

# include <XliPlatform/Application.h>
#if defined(XLI_PLATFORM_ANDROID)
# include <XliPlatform/PlatformSpecific/Android.h>
#elif defined(XLI_PLATFORM_IOS)
# include <XliPlatform/PlatformSpecific/iOS.h>
#endif

namespace Xli
{
    extern Application* InitializeSharedApp();
}

extern "C" int main(int argc, char** argv)
{
    int result = EXIT_FAILURE;

    try
    {
        Xli::CoreLib::Init();

#if defined(XLI_PLATFORM_ANDROID)
        Xli::PlatformSpecific::Android::Init(((int)argv)==1);
#elif defined(XLI_PLATFORM_IOS)
        Xli::PlatformSpecific::iOS::Init();
#endif
        Xli::Application::application_ = Xli::InitializeSharedApp();
        result = Xli::Application::SharedApp()->Run(argc, argv);
    }
    catch (const Xli::Exception& e)
    {
        Xli::CoreLib::OnUnhandledException(e, "main");
    }
    catch (...)
    {
        Xli::Exception e("An unsupported C++ exception was thrown");
        Xli::CoreLib::OnUnhandledException(e, "main");
    }

    return result;
}

#if defined(XLI_PLATFORM_WIN32)

#include <XliPlatform/PlatformSpecific/Win32.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    return main(__argc, __argv);
}

#endif

#if defined(XLI_PLATFORM_ANDROID)
#include <android/log.h>
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "XliApp", __VA_ARGS__))

extern "C" {
    void JNICALL cppOnCreate(JNIEnv *env , jobject obj, jboolean seperateCoreThread)
    {
        LOGD ("----------");
        main(0,(char**)(seperateCoreThread ? 1 : 0));
        LOGD ("----------");
    }
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    LOGD ("----------");
    LOGD ("Jni_OnLoad");

    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        LOGD ("&&&&&&& GetEnv failed &&&&&&");
        return -1;
    }

    jclass shimClass = env->FindClass("com/Shim/XliJ");

    // attach oncreate
    static JNINativeMethod native_funcs[] = {
        {(char* const)"cppOnCreate", (char* const)"(Z)V", (void *)&cppOnCreate},
    };
    jint attached = env->RegisterNatives(shimClass, native_funcs, 1);
    if (attached < 0) {
        LOGD("COULD NOT REGISTER OnCreate CALLBACK");
        XLI_THROW("COULD NOT REGISTER OnCreate CALLBACK");
    } else {
        LOGD("Native functions registered");
    }

    Xli::PlatformSpecific::Android::OnJNILoad(vm, env, shimClass);
    LOGD ("----------");
    return JNI_VERSION_1_6;
}


#endif
