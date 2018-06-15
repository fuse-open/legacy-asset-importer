/* * Copyright (C) 2010-2014 Outracks Technologies
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __XLI_ASHIM_ANDROID_H__
#define __XLI_ASHIM_ANDROID_H__

#include <jni.h>
#include <Xli/String.h>
#include <XliHttpClient.h>
#include "AInternal.h"

namespace Xli
{
    namespace PlatformSpecific
    {
        class AShim
        {
        private:
            static jmethodID raiseKeyboard;
            static jmethodID hideKeyboard;
            static jmethodID getKeyboardSize;
            static jmethodID showMessageBox;
            static jmethodID connectedToNetwork;
            static jmethodID newHttpConnection;
            static jmethodID httpGetOutputStream;
            static jmethodID httpGetInputStream;
            static jmethodID httpShowHeaders;
            static jmethodID initDefaultCookieManager;
            static jmethodID getAssetManager;
            static jmethodID hideStatusBar;
            static jmethodID showStatusBar;
            static jmethodID getStatusBarHeight;
            static jmethodID getDisplayMetrics;
            static jmethodID abortAsyncTask;
            static jmethodID holdObject;
            static jmethodID getObject;
            static jmethodID tryReleaseObject;
            static jmethodID asyncInputStreamToString;
            static jmethodID asyncInputStreamToByteArray;
            static jmethodID sendHttpAsyncA;
            static jmethodID sendHttpAsyncB;
            static jmethodID getHeaderMap;
            static jmethodID getUnoSurface;
            static jmethodID getUnoSurfaceWidth;
            static jmethodID getUnoSurfaceHeight;
            static jmethodID registerTimer;
            static jmethodID unregisterTimer;
            static jmethodID getActivity;

        public:
            static int kbVisible;

            static void CacheMids(JNIEnv *env, jclass shim_class);
            static void CheckExceptions();

            static void RaiseSoftKeyboard();
            static void HideSoftKeyboard();
            static bool KeyboardVisible();
            static int GetKeyboardSize();
            static void ShowStatusBar();
            static void HideStatusBar();
            static float GetStatusBarHeight();
            static int ShowMessageBox(const String& message, const String& caption, int buttons, int hints);

            static bool ConnectedToNetwork();
            static String HeadersToString(const HttpRequest* req);
            static JObjRef SendHttpAsync(const HttpRequest* req, const void* content, long byteLength);
            static JObjRef SendHttpAsync(const HttpRequest* req, String content);
            static JObjRef SendHttpAsync(const HttpRequest* req);
            static void AbortAsyncTask(JObjRef task);
            static String InputStreamToString(jobject bufferedInputStream);
            static JObjRef AsyncInputStreamToString(JObjRef bufferedInputStream, HttpRequest* request);
            static JObjRef AsyncInputStreamToByteArray(JObjRef bufferedInputStream, HttpRequest* request);
            static int ReadBytesFromInputStream(jobject bufferedInputStream, int bytesToRead, void* dst);
            static AAssetManager* GetAssetManager();
            static bool RegisterNativeFunctions(JNINativeMethod native_funcs[], int funcCount);
            static void InitDefaultCookieManager();
            static float GetDensity();
            static Vector2 GetDpi();
            static JObjRef HoldObject(jobject obj);
            static jobject GetObject(JObjRef objKey);
            static bool TryReleaseObject(JObjRef objKey);
            static jobject GetHeaderMap();
            static jobject GetUnoSurface();
            static int GetUnoSurfaceWidth();
            static int GetUnoSurfaceHeight();
            static int RegisterTimer(int millisecondsDelay);
            static void UnregisterTimer(int timerID);
            static jobject GetActivity(JNIEnv* env, jclass shim_class);
        };
    };
};

#endif
