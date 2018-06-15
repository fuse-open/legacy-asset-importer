//
// Copyright (C) 2010-2014 Outracks chnologies
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

#include <Xli/HashMap.h>
#include <XliHttpClient/HttpClient.h>
#include <XliHttpClient/Uri.h>
#include <XliPlatform/PlatformSpecific/Android.h>
#include "AHttpClient.h"
#include "../../XliPlatform/android/AJniHelper.h"

extern Xli::Window* GlobalWindow;

namespace Xli
{

    HashMap<HttpRequest*, short> SessionMap::abortedTable;

    //---- AHttpRequest ----//

    AHttpRequest::AHttpRequest(AHttpClient* client, String url, String method)
    {
        this->client = client;
        this->state = HttpRequestStateOpened;
        this->url = Uri::AutoEncodeUri(url);
        this->method = method.ToUpper();
        this->timeout = 0;
        this->verifyHost = true;
        this->aborted = false;
        this->errored = false;
        this->javaAsyncHandle = 0;
        this->javaContentHandle = 0;
        this->responseStatus = 0;
        this->responseBody = new ArrayStream(1);
        HttpEventHandler* eh = client->GetEventHandler();
        if (eh!=0) eh->OnRequestStateChanged(this);
    }

    AHttpRequest::~AHttpRequest()
    {
        if (((int)state>(int)HttpRequestStateUnsent) &&
            ((int)state<(int)HttpRequestStateDone) && !aborted)
            Abort();
        CleanHandles();
    }

    void AHttpRequest::CleanHandles()
    {
        PlatformSpecific::AJniHelper jni;
        if (javaAsyncHandle!=0) PlatformSpecific::AShim::TryReleaseObject(javaAsyncHandle);
        if (javaContentHandle!=0) PlatformSpecific::AShim::TryReleaseObject(javaContentHandle);
        javaAsyncHandle = 0;
        javaContentHandle = 0;
    }

    String AHttpRequest::GetMethod() const { return method; }
    String AHttpRequest::GetUrl() const { return url; }
    HttpRequestState AHttpRequest::GetState() const { return state; }
    int AHttpRequest::GetTimeout() const { return timeout; }
    void AHttpRequest::SetTimeout(int timeout)
    {
        if (state <= HttpRequestStateOpened)
        {
            this->timeout = timeout;
        } else {
            XLI_THROW("HttpRequest->SetTimeout(): Not in a valid state to set the timeout");
        }
    }
    bool AHttpRequest::GetVerifyHost() const { return verifyHost; }
    void AHttpRequest::SetVerifyHost(bool verify)
    {
        if (state <= HttpRequestStateOpened)
        {
            verifyHost = verify;
        } else {
            XLI_THROW("HttpRequest->SetTimeout(): Not in a valid state to set the verify host flag");
        }
    }

    void AHttpRequest::SetHeader(const String& key, const String& value)
    {
        if (state == HttpRequestStateOpened)
        {
            headers.Add(key,value);
        } else {
            XLI_THROW("HttpRequest->SetHeader(): Not in a valid state to set a header");
        }
    }
    void AHttpRequest::RemoveHeader(const String& key)
    {
        if (state == HttpRequestStateOpened)
        {
            headers.Remove(key);
        } else {
            XLI_THROW("HttpRequest->SetHeader(): Not in a valid state to set a header");
        }
    }

    int AHttpRequest::HeadersBegin() const { return headers.Begin(); }
    int AHttpRequest::HeadersEnd() const { return headers.End(); }
    int AHttpRequest::HeadersNext(int n) const { return headers.Next(n); }
    String AHttpRequest::GetHeaderKey(int n) const { return headers.GetKey(n); }
    String AHttpRequest::GetHeaderValue(int n) const { return headers.GetValue(n); }

    void AHttpRequest::SendAsync(const void* content, int byteLength)
    {
        if (state == HttpRequestStateOpened)
        {
            state = HttpRequestStateSent;
            HttpEventHandler* eh = client->GetEventHandler();
            if (eh!=0) eh->OnRequestStateChanged(this);
            javaAsyncHandle = PlatformSpecific::AShim::SendHttpAsync(this, content, byteLength);
        } else {
            XLI_THROW("HttpRequest->SendAsync(): Not in a valid state to send");
        }
    }

    void AHttpRequest::SendAsync(const String& content)
    {
        if (state == HttpRequestStateOpened)
        {
            state = HttpRequestStateSent;
            HttpEventHandler* eh = client->GetEventHandler();
            if (eh!=0) eh->OnRequestStateChanged(this);
            javaAsyncHandle = PlatformSpecific::AShim::SendHttpAsync(this, content);
        } else {
            XLI_THROW("HttpRequest->SendAsync(): Not in a valid state to send");
        }
    }

    void AHttpRequest::SendAsync()
    {
        if (state == HttpRequestStateOpened)
        {
            state = HttpRequestStateSent;
            HttpEventHandler* eh = client->GetEventHandler();
            if (eh!=0) eh->OnRequestStateChanged(this);
            javaAsyncHandle = PlatformSpecific::AShim::SendHttpAsync(this);
        } else {
            HttpEventHandler* eh = client->GetEventHandler();
            if (eh!=0)
                eh->OnRequestError(const_cast<AHttpRequest*>(this),"XliHttp: Cannot find Http EventHandler");
            else
                XLI_THROW("XliHttp: Cannot find Http EventHandler");
        }
    }

    void AHttpRequest::Abort()
    {
        if (!aborted)
        {
            if ((int)state >= (int)HttpRequestStateOpened)
            {
                // {TODO} if ashim::abortasynctask works then we need some kind of callback so we know to
                // cleanup the SessionMap.
                SessionMap::MarkAborted(this);
                if (javaAsyncHandle!=0)
                    PlatformSpecific::AShim::AbortAsyncTask(javaAsyncHandle);

                CleanHandles();
                aborted = true;
                HttpEventHandler* eh = client->GetEventHandler();
                if (eh!=0) eh->OnRequestAborted(this);

            } else {
                HttpEventHandler* eh = client->GetEventHandler();
                if (eh!=0)
                    eh->OnRequestError(const_cast<AHttpRequest*>(this),"XliHttp: Cannot find Http EventHandler");
                else
                    XLI_THROW("XliHttp: Cannot find Http EventHandler");
            }
        }
    }
    void AHttpRequest::StartDownload()
    {
        if ((this->state==HttpRequestStateHeadersReceived) && (this->javaContentHandle))
        {
            this-> javaAsyncHandle = PlatformSpecific::AShim::AsyncInputStreamToByteArray(this->javaContentHandle, this);
            this->state = HttpRequestStateLoading;
            HttpEventHandler* eh = client->GetEventHandler();
            if (eh!=0) eh->OnRequestStateChanged(this);
        } else if (!this->javaContentHandle) {
            XLI_THROW("HttpRequest->PullContentArray(): In correct state to pull content array but have null contentHandle");
        } else {
            XLI_THROW("HttpRequest->PullContentArray(): Not in valid state for pulling the content array");
        }
    }

    int AHttpRequest::GetResponseHeaderCount() const
    {
        if (state >= HttpRequestStateHeadersReceived)
        {
            return responseHeaders.Count();
        } else {
            XLI_THROW("HttpRequest->GetResponseHeaderCount(): Not in a valid state to get the response header count");
        }
    }
    int AHttpRequest::ResponseHeadersBegin() const
    {
        if (state >= HttpRequestStateHeadersReceived)
        {
            return responseHeaders.Begin();
        } else {
            XLI_THROW("HttpRequest->ResponseHeaderBegin(): Not in a valid state to get the response header iterator");
        }
    }
    int AHttpRequest::ResponseHeadersNext(int n) const
    {
        if (state >= HttpRequestStateHeadersReceived)
        {
            return responseHeaders.Next(n);
        } else {
            XLI_THROW("HttpRequest->ResponseHeaderNext(): Not in a valid state to get the next response header");
        }
    }
    int AHttpRequest::ResponseHeadersEnd() const
    {
        if (state >= HttpRequestStateHeadersReceived)
        {
            return responseHeaders.End();
        } else {
            XLI_THROW("HttpRequest->ResponseHeaderEnd(): Not in a valid state to get the response header");
        }
    }
    String AHttpRequest::GetResponseHeaderValue(int n) const
    {
        if (state >= HttpRequestStateHeadersReceived)
        {
            return responseHeaders.GetValue(n);
        } else {
            XLI_THROW("HttpRequest->GetResponseHeaderN(): Not in a valid state to get the response header");
        }
    }
    String AHttpRequest::GetResponseHeaderKey(int n) const
    {
        if (state >= HttpRequestStateHeadersReceived)
        {
            return responseHeaders.GetKey(n);
        } else {
            XLI_THROW("HttpRequest->GetResponseHeaderN(): Not in a valid state to get the response header");
        }
    }

    bool AHttpRequest::TryGetResponseHeader(const String& key, String& result) const
    {
        if (state >= HttpRequestStateHeadersReceived)
        {
            return responseHeaders.TryGetValue(key, result);
        } else {
            XLI_THROW("HttpRequest->GetResponseHeader(): Not in a valid state to get the response header");
        }
    }

    int AHttpRequest::GetResponseStatus() const
    {
        if (state >= HttpRequestStateHeadersReceived)
        {
            return responseStatus;
        } else {
            XLI_THROW("HttpRequest->GetResponseStatus(): Not in a valid state to get the response status");
        }
    }

    DataAccessor* AHttpRequest::GetResponseBody() const
    {
        if (state == HttpRequestStateDone)
        {
            return (DataAccessor*)responseBodyRef.Get();
        } else {
            XLI_THROW("HttpRequest->GetResponseBody(): Not in a valid state to get the response body");
        }
    }

    //---- Entry points fors calls from java to c++ ----//
    extern "C"
    {
        void JNICALL XliJ_HttpCallback (JNIEnv *env , jobject obj, jint body,
                                        jobjectArray headers, jint responseCode,
                                        jstring responseMessage, jlong requestPointer)
        {
            if (requestPointer)
            {
                Xli::AHttpRequest* request = ((Xli::AHttpRequest*)((void*)requestPointer));

                if (SessionMap::IsAborted(request)) return;

                if (body!=-1)
                {
                    request->javaContentHandle = (JObjRef)body;
                } else {
                    request->javaContentHandle = 0;
                }
                request->responseStatus = (int)responseCode;
                if (headers)
                {
                    int headerCount = (env->GetArrayLength(headers) / 2);
                    int index = 0;
                    jstring jkey;
                    jstring jval;
                    char const* ckey;
                    char const * cval;
                    for (int i=0; i<headerCount; i++)
                    {
                        index = i * 2;

                        jkey = (jstring) env->GetObjectArrayElement(headers, index);
                        jval = (jstring) env->GetObjectArrayElement(headers, (index + 1));

                        if (!jkey) {
                            ckey = "";
                        } else {
                            ckey = env->GetStringUTFChars(jkey, NULL);
                        }
                        if (!jval) {
                            cval = "";
                        } else {
                            cval = env->GetStringUTFChars(jval, NULL);
                        }

                        if (!request->responseHeaders.ContainsKey(ckey)) {
                            request->responseHeaders.Add(ckey,cval);
                        }
                        if (responseMessage!=0)
                        {
                            char const* rmess = env->GetStringUTFChars(responseMessage, NULL);
                            env->ReleaseStringUTFChars(responseMessage, rmess);
                        }

                        env->ReleaseStringUTFChars(jkey, ckey);
                        env->ReleaseStringUTFChars(jval, cval);
                    }
                }
                env->DeleteLocalRef(headers);
                env->DeleteLocalRef(responseMessage);
                request->javaAsyncHandle = 0;
                request->client->EnqueueAction(new Xli::AHttpStateAction(request, Xli::HttpRequestStateHeadersReceived));
            } else {
                LOGE("CRITICAL HTTP ERROR: No callback pointer error");
            }
        }

        void JNICALL XliJ_HttpBytesDownloadedCallback (JNIEnv *env , jclass clazz, jbyteArray content, jint byteLength, jlong requestPointer)
        {
            if (requestPointer)
            {
                Xli::AHttpRequest* request = ((Xli::AHttpRequest*)((void*)requestPointer));

                if (SessionMap::IsAborted(request))
                {
                    SessionMap::RemoveSession(request);
                    return;
                }

                if (content != 0 && byteLength!=-1) {
                    jbyte* jbyteArrayPtr = env->GetByteArrayElements(content, NULL);
                    request->responseBody->Write((UInt8*)jbyteArrayPtr, 1, (int)byteLength);
                    env->ReleaseByteArrayElements(content, jbyteArrayPtr, 0);
                }
                if (byteLength == -1)
                {
                    request->responseBodyRef = new BufferRef((void*)request->responseBody->GetPtr(), request->responseBody->GetLength(), (Object*)request->responseBody.Get());
                    request->client->EnqueueAction(new Xli::AHttpStateAction(request, Xli::HttpRequestStateDone, true));
                }
            } else {
                LOGE("CRITICAL HTTP ERROR: No callback pointer error");
            }
        }

        void JNICALL XliJ_HttpTimeoutCallback (JNIEnv *env , jclass clazz, jlong requestPointer)
        {
            if (requestPointer)
            {
                Xli::AHttpRequest* request = ((Xli::AHttpRequest*)((void*)requestPointer));

                if (SessionMap::IsAborted(request))
                {
                    SessionMap::RemoveSession(request);
                    return;
                }

                request->client->EnqueueAction(new Xli::AHttpTimeoutAction(request));
            } else {
                LOGE("CRITICAL HTTP ERROR: No callback pointer error");
            }
        }

        void JNICALL XliJ_HttpErrorCallback (JNIEnv *env , jclass clazz, jlong requestPointer, jint errorCode, jstring errorMessage)
        {
            if (requestPointer)
            {
                Xli::AHttpRequest* request = ((Xli::AHttpRequest*)((void*)requestPointer));

                if (SessionMap::IsAborted(request))
                {
                    SessionMap::RemoveSession(request);
                    return;
                }

                char const* cerrorMessage = env->GetStringUTFChars(errorMessage, NULL);
                Xli::AHttpErrorAction* err =  new Xli::AHttpErrorAction(request, (int)errorCode, Xli::String(cerrorMessage));
                env->ReleaseStringUTFChars(errorMessage, cerrorMessage);

                request->client->EnqueueAction(err);
            } else {
                LOGE("CRITICAL HTTP ERROR: No callback pointer error");
            }
        }

        void JNICALL XliJ_HttpProgressCallback (JNIEnv *env , jclass clazz, jlong requestPointer, jlong position, jlong totalLength, jboolean lengthKnown, jint direction)
        {
            if (requestPointer)
            {
                Xli::AHttpRequest* request = ((Xli::AHttpRequest*)((void*)requestPointer));

                if (SessionMap::IsAborted(request)) return;

                request->client->EnqueueAction(new Xli::AHttpProgressAction(request, position, totalLength, lengthKnown));
            } else {
                LOGE("CRITICAL HTTP ERROR: No callback pointer error");
            }
        }

        void JNICALL XliJ_HttpAbortedCallback (JNIEnv *env , jclass clazz, jlong requestPointer)
        {
            LOGD("XliHttpRequest: Abort callback");
            if (requestPointer)
            {
                Xli::AHttpRequest* request = ((Xli::AHttpRequest*)((void*)requestPointer));
                SessionMap::RemoveSession(request);
            } else {
                LOGE("CRITICAL HTTP ERROR: No callback pointer error");
            }
        }
    }

    static int HttpInitialized = 0;
    static void InitAndroidHttp()
    {
        if (!HttpInitialized)
        {
            PlatformSpecific::AShim::InitDefaultCookieManager();
            static JNINativeMethod nativeFuncs[] = {
                {(char* const)"XliJ_HttpCallback", (char* const)"(I[Ljava/lang/String;ILjava/lang/String;J)V",
                 (void *)&XliJ_HttpCallback},
                {(char* const)"XliJ_HttpTimeoutCallback", (char* const)"(J)V", (void *)&XliJ_HttpTimeoutCallback},
                {(char* const)"XliJ_HttpErrorCallback", (char* const)"(JILjava/lang/String;)V",
                 (void *)&XliJ_HttpErrorCallback},
                {(char* const)"XliJ_HttpProgressCallback", (char* const)"(JJJZI)V", (void *)&XliJ_HttpProgressCallback},
                {(char* const)"XliJ_HttpAbortedCallback", (char* const)"(J)V", (void *)&XliJ_HttpAbortedCallback},
                {(char* const)"XliJ_HttpBytesDownloadedCallback", (char* const)"([BIJ)V", (void *)&XliJ_HttpBytesDownloadedCallback},
            };
            bool result = PlatformSpecific::AShim::RegisterNativeFunctions(nativeFuncs, 6);
            if (result)
            {
                LOGD("XliHttp: Registered the java->c++ callbacks");
            } else {
                XLI_THROW("XliHttp: Could not register the java->c++ callbacks");
            }
            HttpInitialized = 1;
        }
    }

    AHttpRequest* AHttpClient::CreateRequest(const String& method, const String& url)
    {
        return new AHttpRequest(this, url, method);
    }

    void AHttpClient::Update()
    {
        while ((actionQueue.Count() > 0))
        {
            HttpAction* action = actionQueue.Dequeue();
            action->Execute();
            delete action;
        }
    }

    void AHttpClient::SetEventHandler(HttpEventHandler* eventHandler)
    {
        this->eventHandler = eventHandler;
    }
    HttpEventHandler* AHttpClient::GetEventHandler()
    {
        return eventHandler;
    }

    void AHttpClient::EnqueueAction(HttpAction* action)
    {
        actionQueue.Enqueue(action);
    }

    HttpClient* HttpClient::Create()
    {
        if (!HttpInitialized) InitAndroidHttp();
        return new AHttpClient();
    }
};
