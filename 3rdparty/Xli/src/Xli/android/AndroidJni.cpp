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

#define DEBUG_JNI

#include "AndroidJni.h"
#include <Xli/Exception.h>

#include <pthread.h>
#include <stdarg.h>

#include <android/log.h>
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "XLI", __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "XLI", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "XLI", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "XLI", __VA_ARGS__))
#define LOGF(...) ((void)__android_log_print(ANDROID_LOG_FATAL, "XLI", __VA_ARGS__))

static JavaVM* StaticJavaVM;
static pthread_key_t StaticThreadKey;

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    StaticJavaVM = vm;
    return JNI_VERSION_1_6;
}

namespace Xli
{
    namespace PlatformSpecific
    {
        static void JniDestroyThread(void* value)
        {
            LOGD("JNI: Detaching current thread");

            JNIEnv* env = (JNIEnv*)value;
            StaticJavaVM->DetachCurrentThread();
            pthread_setspecific(StaticThreadKey, NULL);
        }

        AndroidJni::AndroidJni()
        {
            if (!StaticJavaVM)
                XLI_THROW("JNI ERROR: Java VM ptr not set");

            if (StaticJavaVM->GetEnv(reinterpret_cast<void**>(&_env), JNI_VERSION_1_6) != JNI_OK &&
                StaticJavaVM->AttachCurrentThread(&_env, NULL) != JNI_OK)
                XLI_THROW("JNI ERROR: Failed to attach current thread");

            if (!pthread_getspecific(StaticThreadKey))
                pthread_setspecific(StaticThreadKey, (void*)_env);
        }

        jmethodID AndroidJni::FindMethod(const char* className, const char* methodName, const char* methodSig)
        {
            jclass cls = _env->FindClass(className);
            if (!cls)
                XLI_THROW((String)"Failed to get JNI class '" + className + "'");

            jmethodID method = _env->GetMethodID(cls, methodName, methodSig);
            if (!method)
                XLI_THROW((String)"Failed to get JNI method '" + className + "." + methodName + methodSig + "'");

            return method;
        }

        jobject AndroidJni::CallObjectMethod(jobject inst, const char* name, const char* sig)
        {
            jclass cls = _env->GetObjectClass(inst);

#ifdef DEBUG_JNI
            if (!cls)
                XLI_THROW((String)"Failed to get JNI class for method '" + name + "'");
#endif

            jmethodID method = _env->GetMethodID(cls, name, sig);

#ifdef DEBUG_JNI
            if (!method)
                XLI_THROW((String)"Failed to get JNI method '" + name + "'");
#endif

            return _env->CallObjectMethod(inst, method);
        }

        String AndroidJni::GetString(jobject str)
        {
            const char* utf8 = _env->GetStringUTFChars((jstring)str, NULL);
            String result = utf8;
            _env->ReleaseStringUTFChars((jstring)str, utf8);
            return result;
        }

        JNIEnv* AndroidJni::GetEnv()
        {
            // TODO: Check jni exceptions
            return _env;
        }

        JavaVM* AndroidJni::GetVM()
        {
            return StaticJavaVM;
        }

        JNIEnv* AndroidJni::operator->()
        {
            // TODO: Check jni exceptions
            return _env;
        }

        jmethodID AndroidJni::GetInstanceMethod(jobject inst, const char* m_name, const char* m_sig)
        {
            return _env->GetMethodID(_env->GetObjectClass(inst), m_name, m_sig);
        }

        jobject AndroidJni::GetInstance(const char* class_name, const char* constructor_sig, ...)
        {
            jclass cls = _env->FindClass(class_name);
            if (cls) {
                jmethodID mid = _env->GetMethodID(cls, "<init>", constructor_sig);
                if (mid) {
                    jobject result = 0;
                    va_list args;
                    va_start(args, constructor_sig);
                    result = _env->NewObjectV(cls, mid, args);
                    va_end(args);
                    return result;
                } else {
                    LOGE( "GetInstance Could not get <init> for class");
                    return NULL;
                }
            } else {
                LOGE( "GetInstance Could not find class");
                return NULL;
            }
        }

        jobject AndroidJni::GetInstance(jclass cls, const char* constructor_sig, ...)
        {
            jmethodID mid = _env->GetMethodID(cls, "<init>", constructor_sig);
            jobject result = 0;
            va_list args;
            va_start(args, constructor_sig);
            result = _env->NewObjectV(cls, mid, args);
            va_end(args);
            return result;
        }
    }
}
