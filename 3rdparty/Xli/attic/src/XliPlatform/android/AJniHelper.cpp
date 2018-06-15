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

#include <XliPlatform/PlatformSpecific/Android.h>
#include <XliPlatform/Window.h>
#include <XliPlatform/InputEventHandler.h>
#include <XliHttpClient.h>
#include <pthread.h>
#include <stdarg.h>
#include "AJniHelper.h"
#include "AShim.h"

#define DEBUG_JNI

extern Xli::InputEventHandler* GlobalEventHandler;
extern Xli::Window* GlobalWindow;
extern int GlobalInit;

namespace Xli
{
    namespace PlatformSpecific
    {
        static jobject ActivityObject = 0;
        static jclass ShimClass = 0;
        static JavaVM* VM;
        static pthread_key_t JniThreadKey;

        static void JniDestroyThread(void* value)
        {
            LOGD("JNI: Detaching current thread");

            JNIEnv* env = (JNIEnv*)value;
            VM->DetachCurrentThread();
            pthread_setspecific(JniThreadKey, NULL);
        }

        void AJniHelper::Init(JavaVM* vm, JNIEnv* env, jclass shimClass)
        {
            VM = vm;
            if (pthread_key_create(&JniThreadKey, JniDestroyThread))
                LOGE("JNI ERROR: Unable to create pthread key"); // Not fatal

            ShimClass = reinterpret_cast<jclass>(env->NewGlobalRef(shimClass));
            pthread_setspecific(JniThreadKey, (void*)env);

            AShim::CacheMids(env, shimClass);
        }

        AJniHelper::AJniHelper()
        {
            int status_ = (int)VM->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
            if (status_ != JNI_OK)
            {
            	status_ = (int)VM->AttachCurrentThread(&env, NULL);
                if (status_ != JNI_OK)
                    XLI_THROW("JNI ERROR: Failed to attach current thread");
            }
            if (!pthread_getspecific(JniThreadKey))
            {
                pthread_setspecific(JniThreadKey, (void*)env);
            }

            if (ActivityObject==0)
            {
                ActivityObject = reinterpret_cast<jclass>(env->NewGlobalRef(AShim::GetActivity(env, ShimClass)));
                if (ActivityObject==0)
                {
                    XLI_THROW("JNI ERROR: Failed to grab activity object");
                }
            }
        }

        jclass AJniHelper::GetShim()
        {
            return ShimClass;
        }

        jmethodID AJniHelper::FindMethod(const char* className, const char* methodName, const char* methodSig)
        {
            jclass cls = env->FindClass(className);
            if (!cls)
                XLI_THROW((String)"Failed to get JNI class '" + className + "'");

            jmethodID method = env->GetMethodID(cls, methodName, methodSig);
            if (!method)
                XLI_THROW((String)"Failed to get JNI method '" + className + "." + methodName + methodSig + "'");

            return method;
        }

        jobject AJniHelper::CallObjectMethod(jobject inst, const char* name, const char* sig)
        {
            jclass cls = env->GetObjectClass(inst);

#ifdef DEBUG_JNI
            if (!cls)
                XLI_THROW((String)"Failed to get JNI class for method '" + name + "'");
#endif

            jmethodID method = env->GetMethodID(cls, name, sig);

#ifdef DEBUG_JNI
            if (!method)
                XLI_THROW((String)"Failed to get JNI method '" + name + "'");
#endif

            return env->CallObjectMethod(inst, method);
        }

        String AJniHelper::GetString(jobject str)
        {
            const char* utf8 = env->GetStringUTFChars((jstring)str, NULL);
            String result = utf8;
            env->ReleaseStringUTFChars((jstring)str, utf8);
            return result;
        }

        JNIEnv* AJniHelper::GetEnv()
        {
            // TODO: Check jni exceptions
            return env;
        }
        JavaVM* AJniHelper::GetVM()
        {
            return VM;
        }

        JNIEnv* AJniHelper::operator->()
        {
            // TODO: Check jni exceptions
            return env;
        }

        jmethodID AJniHelper::GetInstanceMethod(const char* m_name, const char* m_sig)
        {
            return GetInstanceMethod(GetInstance(), m_name, m_sig);
        }

        jmethodID AJniHelper::GetInstanceMethod(jobject inst, const char* m_name, const char* m_sig)
        {
            return env->GetMethodID(env->GetObjectClass(inst), m_name, m_sig);
        }

        jobject AJniHelper::GetActivity()
        {
            return ActivityObject;
        }
        jobject AJniHelper::GetInstance()
        {
            return ActivityObject;
        }

        jobject AJniHelper::GetInstance(const char* class_name, const char* constructor_sig, ...)
        {
            jclass cls = env->FindClass(class_name);
            if (cls) {
                jmethodID mid = env->GetMethodID(cls, "<init>", constructor_sig);
                if (mid) {
                    jobject result = 0;
                    va_list args;
                    va_start(args, constructor_sig);
                    result = env->NewObjectV(cls, mid, args);
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

        jobject AJniHelper::GetInstance(jclass cls, const char* constructor_sig, ...)
        {
            jmethodID mid = env->GetMethodID(cls, "<init>", constructor_sig);
            jobject result = 0;
            va_list args;
            va_start(args, constructor_sig);
            result = env->NewObjectV(cls, mid, args);
            va_end(args);
            return result;
        }

        jclass AJniHelper::GetCustomClass(const char* class_path)
        {
            jclass activityClass = env->FindClass("android/app/NativeActivity");
            jmethodID getClassLoader = env->GetMethodID(activityClass,"getClassLoader", "()Ljava/lang/ClassLoader;");
            jobject cls = env->CallObjectMethod(ActivityObject, getClassLoader);
            jclass classLoader = env->FindClass("java/lang/ClassLoader");
            jmethodID findClass = env->GetMethodID(classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
            jstring strClassName = env->NewStringUTF(class_path);
            jclass classIWant = (jclass)env->CallObjectMethod(cls, findClass, strClassName);
        }
    }
}
