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

#ifndef __XLI_HTTP_HTTP_CLIENT_H__
#define __XLI_HTTP_HTTP_CLIENT_H__

#include <Xli/Buffer.h>
#include <Xli/String.h>

namespace Xli
{   
    /**
        \ingroup XliHttpClient
    */
    enum HttpRequestState
    {
        HttpRequestStateUnsent=0,
        HttpRequestStateOpened=1,
        HttpRequestStateSent=2,
        HttpRequestStateHeadersReceived=3,
        HttpRequestStateLoading=4,
        HttpRequestStateDone=5,
    };

    /**
        \ingroup XliHttpClient
    */
    class HttpRequest: public Object
    {
    public:
        virtual String GetMethod() const = 0;
        virtual String GetUrl() const = 0;

        virtual HttpRequestState GetState() const = 0;

        virtual void SetTimeout(int timeout) = 0;
        virtual int GetTimeout() const = 0;

        virtual void SetHeader(const String& key, const String& value) = 0;
        virtual void RemoveHeader(const String& key) = 0;

        virtual void SetVerifyHost(bool verify) = 0;
        virtual bool GetVerifyHost() const = 0;

        virtual int HeadersBegin() const = 0;
        virtual int HeadersEnd() const = 0;
        virtual int HeadersNext(int iterator) const = 0;
        virtual String GetHeaderKey(int iterator) const = 0;
        virtual String GetHeaderValue(int iterator) const = 0;

        virtual void SendAsync(const void* content, int byteLength) = 0;
        virtual void SendAsync(const String& content) = 0;
        virtual void SendAsync() = 0;
        virtual void Abort() = 0;

        virtual int GetResponseHeaderCount() const = 0;
        virtual int ResponseHeadersBegin() const = 0;
        virtual int ResponseHeadersEnd() const = 0;
        virtual int ResponseHeadersNext(int iterator) const = 0;
        virtual String GetResponseHeaderKey(int iterator) const = 0;
        virtual String GetResponseHeaderValue(int iterator) const = 0;
        
        virtual bool TryGetResponseHeader(const String& key, String& result) const = 0;

        virtual int GetResponseStatus() const = 0;
        virtual DataAccessor* GetResponseBody() const = 0;
    };

    /**
        \ingroup XliHttpClient
    */
    class HttpEventHandler: public Object
    {
    public:
        virtual void OnRequestStateChanged(HttpRequest* request) { }
        virtual void OnRequestProgress(HttpRequest* request,int position, int total, bool totalKnown) { }
        virtual void OnRequestAborted(HttpRequest* request) { }
        virtual void OnRequestTimeout(HttpRequest* request) { }
        virtual void OnRequestError(HttpRequest* request,String message) { }
    };

    /**
        \ingroup XliHttpClient
    */
    class HttpAction : public Object
    {
    public:
        virtual void Execute() = 0;
    };

    /**
        \ingroup XliHttpClient
    */
    class HttpClient: public Object
    { 
    public:
        static HttpClient* Create();

        virtual HttpRequest* CreateRequest(const String& method, const String& url) = 0;

        virtual void SetEventHandler(HttpEventHandler* eventHandler) = 0;
        virtual HttpEventHandler* GetEventHandler() = 0;

        virtual void Update() = 0;
    };
}

#endif
