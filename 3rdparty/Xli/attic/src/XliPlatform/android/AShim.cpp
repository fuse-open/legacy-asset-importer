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

#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <XliPlatform/PlatformSpecific/Android.h>
#include "AShim.h"
#include "AJniHelper.h"
#include <XliPlatform/Window.h>

//{TODO} All these mids should be cacheable

namespace Xli
{
    namespace PlatformSpecific
    {
        int AShim::kbVisible = 0;

        jmethodID AShim::raiseKeyboard;
        jmethodID AShim::hideKeyboard;
        jmethodID AShim::getKeyboardSize;
        jmethodID AShim::showMessageBox;
        jmethodID AShim::connectedToNetwork;
        jmethodID AShim::httpShowHeaders;
        jmethodID AShim::initDefaultCookieManager;
        jmethodID AShim::getAssetManager;
        jmethodID AShim::hideStatusBar;
        jmethodID AShim::showStatusBar;
        jmethodID AShim::getStatusBarHeight;
        jmethodID AShim::getDisplayMetrics;
        jmethodID AShim::abortAsyncTask;
        jmethodID AShim::holdObject;
        jmethodID AShim::getObject;
        jmethodID AShim::tryReleaseObject;
        jmethodID AShim::asyncInputStreamToString;
        jmethodID AShim::asyncInputStreamToByteArray;
        jmethodID AShim::getHeaderMap;
        jmethodID AShim::getUnoSurface;
        jmethodID AShim::getUnoSurfaceWidth;
        jmethodID AShim::getUnoSurfaceHeight;
        jmethodID AShim::sendHttpAsyncA;
        jmethodID AShim::sendHttpAsyncB;
        jmethodID AShim::registerTimer;
        jmethodID AShim::unregisterTimer;
        jmethodID AShim::getActivity;

        void AShim::CacheMids(JNIEnv *env, jclass shimClass)
        {
            LOGD("Caching Mids");
            raiseKeyboard = env->GetStaticMethodID(shimClass, "RaiseKeyboard", "()V");
            hideKeyboard = env->GetStaticMethodID(shimClass, "HideKeyboard", "()V");
            getKeyboardSize = env->GetStaticMethodID(shimClass, "GetKeyboardSize", "()I");
            showMessageBox = env->GetStaticMethodID(shimClass, "ShowMessageBox", "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;II)I");
            connectedToNetwork = env->GetStaticMethodID(shimClass, "ConnectedToNetwork", "()Z");
            initDefaultCookieManager = env->GetStaticMethodID(shimClass, "InitDefaultCookieManager", "()V");
            abortAsyncTask = env->GetStaticMethodID(shimClass, "AbortAsyncTask", "(I)V");
            getAssetManager = env->GetStaticMethodID(shimClass, "GetAssetManager", "()Landroid/content/res/AssetManager;");
            hideStatusBar = env->GetStaticMethodID(shimClass, "HideStatusBar", "()V");
            showStatusBar = env->GetStaticMethodID(shimClass, "ShowStatusBar", "()V");
            getStatusBarHeight = env->GetStaticMethodID(shimClass, "GetStatusBarHeight", "()F");
            getDisplayMetrics = env->GetStaticMethodID(shimClass, "GetDisplayMetrics", "()Landroid/util/DisplayMetrics;");
            holdObject = env->GetStaticMethodID(shimClass, "HoldObject", "(Ljava/lang/Object;)I");
            getObject = env->GetStaticMethodID(shimClass, "GetObject", "(I)Ljava/lang/Object;");
            tryReleaseObject = env->GetStaticMethodID(shimClass, "TryReleaseObject", "(I)Z");
            asyncInputStreamToString = env->GetStaticMethodID(shimClass, "AsyncInputStreamToString", "(IJ)I");
            asyncInputStreamToByteArray = env->GetStaticMethodID(shimClass, "AsyncInputStreamToByteArray", "(IJ)I");
            getHeaderMap = env->GetStaticMethodID(shimClass, "GetHeaderMap","()Ljava/lang/Object;");
            getUnoSurface = env->GetStaticMethodID(shimClass, "GetUnoSurface","()Landroid/view/Surface;");
            getUnoSurfaceWidth = env->GetStaticMethodID(shimClass, "GetUnoSurfaceWidth","()I");
            getUnoSurfaceHeight = env->GetStaticMethodID(shimClass, "GetUnoSurfaceHeight","()I");
            sendHttpAsyncA = env->GetStaticMethodID(shimClass, "SendHttpAsync", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/nio/ByteBuffer;IJZ)I");
            sendHttpAsyncB = env->GetStaticMethodID(shimClass, "SendHttpStringAsync", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;IJZ)I");
            getActivity = env->GetStaticMethodID(shimClass, "GetActivity", "()Landroid/app/Activity;");
            // registerTimer = env->GetStaticMethodID(shimClass, "RegisterTimer", "(I)I");
            // unregisterTimer = env->GetStaticMethodID(shimClass, "UnregisterTimer", "(I)V");

            if (!raiseKeyboard) XLI_THROW("Cannot cache mid for raiseKeyboard.");
            if (!hideKeyboard) XLI_THROW("Cannot cache mid for hideKeyboard.");
            if (!getKeyboardSize) XLI_THROW("Cannot cache mid for getKeyboardSize.");
            if (!showMessageBox) XLI_THROW("Cannot cache mid for showMessageBox.");
            if (!connectedToNetwork) XLI_THROW("Cannot cache mid for connectedToNetwork.");
            if (!initDefaultCookieManager) XLI_THROW("Cannot cache mid for initDefaultCookieManager.");
            if (!getAssetManager) XLI_THROW("Cannot cache mid for getAssetManager.");
            if (!hideStatusBar) XLI_THROW("Cannot cache mid for hideStatusBar.");
            if (!showStatusBar) XLI_THROW("Cannot cache mid for showStatusBar.");
            if (!getStatusBarHeight) XLI_THROW("Cannot cache mid for getStatusBarHeight.");
            if (!getDisplayMetrics) XLI_THROW("Cannot cache mid for getDisplayMetrics.");
            if (!abortAsyncTask) XLI_THROW("Cannot cache mid for asyncTask.");
            if (!holdObject) XLI_THROW("Cannot cache mid for holdObject.");
            if (!getObject) XLI_THROW("Cannot cache mid for getObject.");
            if (!tryReleaseObject) XLI_THROW("Cannot cache mid for tryReleaseObject.");
            if (!asyncInputStreamToString) XLI_THROW("Cannot cache mid for asyncInputStreamToString.");
            if (!asyncInputStreamToByteArray) XLI_THROW("Cannot cache mid for asyncInputStreamToByteArray.");
            if (!getHeaderMap) XLI_THROW("Cannot cache mid for getHeaderMap.");
            if (!getUnoSurface) XLI_THROW("Cannot cache mid for getUnoSurface.");
            if (!getUnoSurfaceWidth) XLI_THROW("Cannot cache mid for getUnoSurfaceWidth.");
            if (!getUnoSurfaceHeight) XLI_THROW("Cannot cache mid for getUnoSurfaceHeight.");
            if (!sendHttpAsyncA) XLI_THROW("Cannot cache mid for sendHttpAsyncA.");
            if (!sendHttpAsyncB) XLI_THROW("Cannot cache mid for sendHttpAsyncB.");
            if (!getActivity) XLI_THROW("Cannot cache mid for getActivity.");
            LOGD("Mids Cached");
        }

        void AShim::RaiseSoftKeyboard()
        {
            // LOGD("in_RaiseSoftKeyboard");
            AJniHelper jni;
            jni->CallStaticVoidMethod(jni.GetShim(), raiseKeyboard);
            kbVisible = 1;
            // LOGD("out_RaiseSoftKeyboard");
        }

        void AShim::HideSoftKeyboard()
        {
            // LOGD("in_HideSoftKeyboard");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jni->CallStaticVoidMethod(shimClass, hideKeyboard);
            kbVisible = 0;
            // LOGD("out_HideSoftKeyboard");
        }

        int AShim::GetKeyboardSize()
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jint result = (jint)jni->CallStaticIntMethod(shimClass, getKeyboardSize);
            return (int)result;
        }

        void AShim::HideStatusBar()
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jni->CallStaticVoidMethod(shimClass, hideStatusBar);
            //{TODO} When this is working, go fix AWindow statusbar methods
        }

        void AShim::ShowStatusBar()
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jni->CallStaticVoidMethod(shimClass, showStatusBar);
            //{TODO} When this is working, go fix AWindow statusbar methods
        }

        float AShim::GetStatusBarHeight()
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jfloat height = jni->CallStaticFloatMethod(shimClass, getStatusBarHeight);
            return (float)height;
        }

        float AShim::GetDensity()
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jclass metricsClass = jni->FindClass("android/util/DisplayMetrics");
            jfieldID jd = jni->GetFieldID( metricsClass, "density", "F" );
            jobject result = jni->CallStaticObjectMethod(shimClass, getDisplayMetrics);
            jfloat d = (jfloat)jni->GetFloatField(result, jd);
            jni->DeleteLocalRef(result);
            jni->DeleteLocalRef(metricsClass);
            return (float)d;
        }

        Vector2 AShim::GetDpi()
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jclass metricsClass = jni->FindClass("android/util/DisplayMetrics");
            jfieldID xDpi = jni->GetFieldID( metricsClass, "xdpi", "F" );
            jfieldID yDpi = jni->GetFieldID( metricsClass, "ydpi", "F" );
            jobject result = jni->CallStaticObjectMethod(shimClass, getDisplayMetrics);
            jfloat jx = (jfloat)jni->GetFloatField(result, xDpi);
            jfloat jy = (jfloat)jni->GetFloatField(result, yDpi);
            jni->DeleteLocalRef(result);
            jni->DeleteLocalRef(metricsClass);
            return Vector2((float)jx, (float)jy);
        }

        bool AShim::KeyboardVisible()
        {
            return kbVisible;
        }

        int AShim::ShowMessageBox(const String& message, const String& caption, int buttons, int hints)
        {
            //setup for call
            AJniHelper jni;
            jclass shimClass = jni.GetShim();

            //vars for call
            jstring jcaption = jni->NewStringUTF(caption.Ptr());
            jstring jmessage = jni->NewStringUTF(message.Ptr());

            //call
            int result = (int)jni->CallStaticIntMethod(shimClass, showMessageBox, jcaption, jmessage, (jint)buttons, (jint)hints);
            return result;
        }

        bool AShim::ConnectedToNetwork()
        {
            //setup for call
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jboolean jresult = jni->CallStaticBooleanMethod(shimClass, connectedToNetwork);
            return (bool)jresult;
        }

        JObjRef AShim::SendHttpAsync(const HttpRequest* req, const void* content, long byteLength)
        {
            AJniHelper jni;

            String url = req->GetUrl();
            String method = req->GetMethod();

            jstring jurl = jni->NewStringUTF(url.Ptr());
            jstring jmethod = jni->NewStringUTF(method.Ptr());
            jint jtimeout = (jint)req->GetTimeout();

            String headers = HeadersToString(req);
            jstring jheaders = jni->NewStringUTF(headers.Ptr());

            jobject arrayHandle = 0;
            if ((content!=0) && (byteLength>0))
            {
                // no way around the const_cast here, we dont modify the data in java so should be safe.
                arrayHandle = jni->NewDirectByteBuffer(const_cast<void*>(content), byteLength);
                if (arrayHandle == NULL) {
                    XLI_THROW("XliHttp - DirectByteBuffers not supported by this device.");
                }
            }

            jint jresult = jni->CallStaticIntMethod(jni.GetShim(), sendHttpAsyncA, jurl, jmethod, jheaders, arrayHandle,
                                                    jtimeout, (jlong)req, req->GetVerifyHost());
            AShim::CheckExceptions();

            jni->DeleteLocalRef(jurl);
            jni->DeleteLocalRef(jmethod);
            jni->DeleteLocalRef(jheaders);

            if (jresult==-1)
            {
                LOGE("AShim [SendHttpAsync]: Could not create async http request");
                return 0;
            }
            return (JObjRef)jresult;
        }

        void AShim::CheckExceptions()
        {
            AJniHelper jni;
            jthrowable err = jni->ExceptionOccurred();
            if (err != NULL)
            {
                jni->ExceptionDescribe();
                jni->ExceptionClear();

                jmethodID toString = jni->GetMethodID(jni->FindClass("java/lang/Object"), "toString", "()Ljava/lang/String;");
                jstring estring = (jstring) jni->CallObjectMethod(err, toString);
                jboolean isCopy;
                XLI_THROW(jni->GetStringUTFChars(estring, &isCopy));
            }
        }

        JObjRef AShim::SendHttpAsync(const HttpRequest* req, String content)
        {
            AJniHelper jni;

            String url = req->GetUrl();
            String method = req->GetMethod();

            jstring jurl = jni->NewStringUTF(url.Ptr());
            jstring jmethod = jni->NewStringUTF(method.Ptr());
            jint jtimeout = (jint)req->GetTimeout();

            String headers = HeadersToString(req);
            jstring jheaders = jni->NewStringUTF(headers.Ptr());

            jobject body = 0;

            if ((content.Length()>0))
            {
                body = jni->NewStringUTF(content.Ptr());
            }

            jint jresult = jni->CallStaticIntMethod(jni.GetShim(), sendHttpAsyncB, jurl, jmethod, jheaders, body,
                                                    jtimeout, (jlong)req, req->GetVerifyHost());

            jni->DeleteLocalRef(jurl);
            jni->DeleteLocalRef(jmethod);
            jni->DeleteLocalRef(jheaders);

            if (body!=0) jni->DeleteLocalRef(body);

            if (jresult==-1)
            {
                LOGE("AShim [SendHttpAsync]: Could not create async http request");
                return 0;
            }
            return (JObjRef)jresult;
        }

        JObjRef AShim::SendHttpAsync(const HttpRequest* req)
        {
            // LOGD("in_17");
            AJniHelper jni;

            String url = req->GetUrl();
            String method = req->GetMethod();

            jstring jurl = jni->NewStringUTF(url.Ptr());
            jstring jmethod = jni->NewStringUTF(method.Ptr());
            jint jtimeout = (jint)req->GetTimeout();
            jobject arrayHandle = 0;

            String headers = HeadersToString(req);
            jstring jheaders = jni->NewStringUTF(headers.Ptr());

            jint jresult = jni->CallStaticIntMethod(jni.GetShim(), sendHttpAsyncA, jurl, jmethod, jheaders, arrayHandle,
                                                    jtimeout, (jlong)req, req->GetVerifyHost());
            jni->DeleteLocalRef(jurl);
            jni->DeleteLocalRef(jmethod);
            jni->DeleteLocalRef(jheaders);
            if (jresult==0)
            {
                LOGE("AShim [SendHttpAsync]: Could not create async http request");
                // LOGD("out_17");
                return 0;
            }
            // LOGD("out_17");
            return (JObjRef)jresult;
        }

        void AShim::AbortAsyncTask(JObjRef task)
        {
            AJniHelper jni;
            jni->CallStaticVoidMethod(jni.GetShim(), abortAsyncTask, (jint)task);
        }

        String AShim::HeadersToString(const HttpRequest* req)
        {
            String result = "";
            AJniHelper jni;
            int i = req->HeadersBegin();
            while (i != req->HeadersEnd())
            {
                result += req->GetHeaderKey(i) + ":" + req->GetHeaderValue(i) + "\n";
                i = req->HeadersNext(i);
            }
            return result;
        }

        String AShim::InputStreamToString(jobject bufferedInputStream)
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shimClass, "InputStreamToString", "(Ljava/io/InputStream;)Ljava/lang/String;");
            if (!mid) {
                LOGE("Unable to get InputStreamToString mid");
                return "";
            }
            jstring jresult = (jstring)jni->CallStaticObjectMethod(shimClass, mid, bufferedInputStream,(jint)1024);
            String result = jni.GetString(jresult);
            jni->DeleteLocalRef(jresult);
            return result;
        }

        JObjRef AShim::AsyncInputStreamToString(JObjRef bufferedInputStream, HttpRequest* request)
        {
            AJniHelper jni;
            jint result = jni->CallStaticIntMethod(jni.GetShim(), asyncInputStreamToString, (jint)bufferedInputStream, (long)request);
            if (result==-1) return 0;
            return (JObjRef)result;
        }

        JObjRef AShim::AsyncInputStreamToByteArray(JObjRef bufferedInputStream, HttpRequest* request)
        {
            AJniHelper jni;
            jint result = jni->CallStaticIntMethod(jni.GetShim(), asyncInputStreamToByteArray, (jint)bufferedInputStream, (long)request);
            if (result == -1) return 0;
            return (JObjRef)result;
        }

        int AShim::ReadBytesFromInputStream(jobject bufferedInputStream, int bytesToRead, void* dst)
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();

            jmethodID mid = jni->GetStaticMethodID(shimClass, "ReadBytesFromInputStream", "(Ljava/io/BufferedInputStream;I)[B");
            if (!mid) {
                LOGE("Unable to get ReadBytesFromInputStream mid");
                return 0;
            }
            jbyteArray jbytes = (jbyteArray)jni->CallStaticObjectMethod(shimClass, mid, bufferedInputStream, (jint)bytesToRead);
            if (jbytes != 0) {
                jsize len = jni->GetArrayLength(jbytes);
                jni->GetByteArrayRegion(jbytes, 0, len, (jbyte*)dst);
                int result = (int)len;
                jni->DeleteLocalRef(jbytes);
                //jni->DeleteLocalRef((jobject)len);
                return result;
            } else {
                return -1;
            }
        }

        void AShim::InitDefaultCookieManager()
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jni->CallStaticVoidMethod(shimClass, initDefaultCookieManager);
        }


        AAssetManager* AShim::GetAssetManager()
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jobject assetManager = jni->CallStaticObjectMethod(shimClass, getAssetManager);
            jni->NewGlobalRef(assetManager);
            AAssetManager* result = AAssetManager_fromJava(jni.GetEnv(), assetManager);
            return result;
        }

        bool AShim::RegisterNativeFunctions(JNINativeMethod native_funcs[], int funcCount)
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jint attached = jni->RegisterNatives(shimClass, native_funcs, (jint)funcCount);
            return (attached >= 0);
        }

        JObjRef AShim::HoldObject(jobject obj)
        {
            AJniHelper jni;
            jint result = jni->CallStaticIntMethod(jni.GetShim(), holdObject, obj);
            return (JObjRef)result;
        }

        jobject AShim::GetObject(JObjRef objKey)
        {
            AJniHelper jni;
            jobject result = jni->CallStaticObjectMethod(jni.GetShim(), getObject, (jint)objKey);
            return result;
        }

        bool AShim::TryReleaseObject(JObjRef objKey)
        {
            AJniHelper jni;
            bool result = (bool)jni->CallStaticBooleanMethod(jni.GetShim(), tryReleaseObject, (jint)objKey);
            return result;
        }

        jobject AShim::GetHeaderMap()
        {
            AJniHelper jni;
            jobject result = jni->CallStaticObjectMethod(jni.GetShim(), getHeaderMap);
            return result;
        }

        jobject AShim::GetUnoSurface()
        {
            AJniHelper jni;
            jobject result = jni->CallStaticObjectMethod(jni.GetShim(), getUnoSurface);
            return result;
        }

        int AShim::GetUnoSurfaceWidth()
        {
            AJniHelper jni;
            int result = (int)jni->CallStaticIntMethod(jni.GetShim(), getUnoSurfaceWidth);
            return result;
        }

        int AShim::GetUnoSurfaceHeight()
        {
            AJniHelper jni;
            int result = (int)jni->CallStaticIntMethod(jni.GetShim(), getUnoSurfaceHeight);
            return result;
        }


        int AShim::RegisterTimer(int millisecondsDelay)
        {
            // AJniHelper jni;
            // return (int)jni->CallStaticIntMethod(jni.GetShim(), registerTimer, millisecondsDelay);
            return -1;
        }

        void AShim::UnregisterTimer(int timerID)
        {
            // AJniHelper jni;
            // jni->CallStaticVoidMethod(jni.GetShim(), unregisterTimer, timerID);
        }

        jobject AShim::GetActivity(JNIEnv* env, jclass shimClass)
        {
            return env->CallStaticObjectMethod(shimClass, getActivity);
        }
    }
}
