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

#include <Xli/ArrayStream.h>
#include <Xli/BufferStream.h>
#include <Xli/Buffer.h>
#include <Xli/HashMap.h>
#include <Xli/MutexQueue.h>
#include <XliHttpClient/HttpClient.h>
#include "../../XliPlatform/android/AInternal.h"

namespace Xli
{
    class AHttpClient;

    class AHttpRequest : public HttpRequest
    {
    public:
        AHttpClient* client;

        HttpRequestState state;

        String url;
        String method;
        int timeout;
        bool verifyHost;
        HashMap<String,String> headers;

        bool errored;
        bool aborted;
        JObjRef javaAsyncHandle;
        JObjRef javaContentHandle;

        bool completedSuccessfully;
        int responseStatus;
        Managed< ArrayStream > responseBody;
        Managed< BufferRef > responseBodyRef;

        HashMap<String,String> responseHeaders;


        AHttpRequest(AHttpClient* client, String url, String method);

        virtual ~AHttpRequest();

        virtual void CleanHandles();

        virtual String GetMethod() const;
        virtual String GetUrl() const;

        virtual HttpRequestState GetState() const;

        virtual void SetTimeout(int timeout);
        virtual int GetTimeout() const;

        virtual void SetVerifyHost(bool verify);
        virtual bool GetVerifyHost() const;

        virtual void SetHeader(const String& key, const String& value);
        virtual void RemoveHeader(const String& key);

        virtual int HeadersBegin() const;
        virtual int HeadersEnd() const;
        virtual int HeadersNext(int n) const;
        virtual String GetHeaderKey(int n) const;
        virtual String GetHeaderValue(int n) const;

        virtual void SendAsync(const void* content, int byteLength);
        virtual void SendAsync(const String& content);
        virtual void SendAsync();
        virtual void StartDownload();
        virtual void Abort();

        virtual int GetResponseHeaderCount() const;
        virtual int ResponseHeadersBegin() const;
        virtual int ResponseHeadersEnd() const;
        virtual int ResponseHeadersNext(int n) const;
        virtual String GetResponseHeaderKey(int n) const;
        virtual String GetResponseHeaderValue(int n) const;

        virtual bool TryGetResponseHeader(const String& key, String& result) const;

        virtual int GetResponseStatus() const;
        virtual DataAccessor* GetResponseBody() const;
    };

    //------- Client -------//
    class AHttpClient : public HttpClient
    { 
    private:
        MutexQueue<HttpAction*> actionQueue;
        Managed<HttpEventHandler> eventHandler;
    public:
        static HttpClient* Create();

        virtual AHttpRequest* CreateRequest(const String& method, const String& url);

        virtual void SetEventHandler(HttpEventHandler* eventHandler);
        virtual HttpEventHandler* GetEventHandler();

        virtual void EnqueueAction(HttpAction* action);

        virtual void Update();
    };


    //-------- Session Handling -------//
    class SessionMap
    {
    private:
        static HashMap<HttpRequest*, short> abortedTable;
    public:
        static bool IsAborted(HttpRequest* requestHandle)
        {            
            return abortedTable.ContainsKey(requestHandle);
        }
        static void RemoveSession(HttpRequest* requestHandle)
        {
            short ignore;
            bool found = abortedTable.TryGetValue(requestHandle, ignore);
            if (found)
            {
                abortedTable.Remove(requestHandle);
            } else {
                XLI_THROW("XLIHTTP: RemoveSession - request not found in abortedTable");
            }
        }
        static void MarkAborted(HttpRequest* requestHandle)
        {
            if (!abortedTable.ContainsKey(requestHandle))
                abortedTable.Add(requestHandle, 0);
        }
    };

    //------ Actions -------//
    class AHttpTimeoutAction : public HttpAction
    {
    public:
        AHttpRequest* Request;
        AHttpTimeoutAction(AHttpRequest* request) { Request = request; }
        virtual void Execute()
        {
            HttpEventHandler* eh = Request->client->GetEventHandler();
            if (eh!=0 && !Request->errored)
                eh->OnRequestTimeout(Request);
        }
    };

    class AHttpProgressAction : public HttpAction
    {
    public:
        AHttpRequest* Request;
        long Position, TotalLength;
        bool LengthKnown;
        AHttpProgressAction(AHttpRequest* request, long position, long totalLength, bool lengthKnown)
        {
            Request = request;
            Position = position;
            TotalLength = totalLength;
            LengthKnown = lengthKnown;
        }

        virtual void Execute()
        {
            HttpEventHandler* eh = Request->client->GetEventHandler();
            if (eh!=0 && !Request->errored)
                eh->OnRequestProgress(Request, Position, TotalLength,
                                      LengthKnown);
        }
    };

    class AHttpErrorAction : public HttpAction
    {
    public:
        AHttpRequest* Request;
        int ErrorCode;
        String ErrorMessage;
        AHttpErrorAction(AHttpRequest* request, int errorCode, String errorMessage)
        {
            Request = request;
            ErrorCode = errorCode;
            ErrorMessage = errorMessage;
        }
        virtual void Execute()
        {
            HttpEventHandler* eh = Request->client->GetEventHandler();
            if (eh!=0) {
                Request->errored = true;
                eh->OnRequestError(Request, ErrorMessage);
            }
        }
    };

    class AHttpStateAction : public HttpAction
    {
    public:
        AHttpRequest* Request;
        HttpRequestState State;
        bool CleanJavaObjs;

        AHttpStateAction(AHttpRequest* request, HttpRequestState state, bool cleanJavaObjs = false)
        {
            Request = request;
            State = state;
            CleanJavaObjs = cleanJavaObjs;
        }

        
        virtual void Execute()
        {
            if (CleanJavaObjs) Request->CleanHandles();
            if (State>0 && !Request->errored) {
                Request->state = State;
                HttpEventHandler* eh = Request->client->GetEventHandler();
                if (eh!=0) eh->OnRequestStateChanged(Request);
                if (State == HttpRequestStateHeadersReceived) Request->StartDownload();
            }
        }
    };
}
