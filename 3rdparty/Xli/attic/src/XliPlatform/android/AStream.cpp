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
#include "AJniHelper.h"
#include "AStream.h"

namespace Xli
{
    namespace PlatformSpecific
    {
        bool AStream::midsCached = false;
        jmethodID AStream::closeReadMid = 0;
        jmethodID AStream::closeWriteMid = 0;
        jmethodID AStream::readByteMid = 0;
        jmethodID AStream::readBufferMid = 0;
        jmethodID AStream::resetMid = 0;
        jmethodID AStream::flushMid = 0;
        jmethodID AStream::writeBufferMid = 0;
        jmethodID AStream::writeBufferDetailedMid = 0;

        AStream::AStream()
        {
            this->javaStream = 0;
            this->canRead = false;
            this->canWrite = false;
            this->streamType = AStream::INVALID;
            this->atEnd = true;
            this->closed = true;
        }

        AStream::AStream(AStreamType streamType, jobject javaStream)
        {
            this->javaStream = 0;
            this->canRead = false;
            this->canWrite = false;
            this->streamType = AStream::INVALID;
            this->atEnd = false;
            this->closed = false;

            Init(streamType, javaStream);
        }

        AStream::AStream(jobject javaStream)
        {
            this->javaStream = 0;
            this->canRead = false;
            this->canWrite = false;
            this->streamType = AStream::INVALID;
            this->atEnd = false;
            this->closed = false;

            AJniHelper jni;
            jclass clsIS = jni->FindClass("java/io/InputStream");
            jclass clsOS = jni->FindClass("java/io/OutputStream");
            
            if (clsIS && clsOS)
            {
                if (jni->IsInstanceOf(javaStream, clsIS))
                {
                    Init(AStream::READ, javaStream);
                } 
                else if (jni->IsInstanceOf(javaStream, clsOS)) 
                {
                    Init(AStream::WRITE, javaStream);
                }
                else
                {
                    LOGE("Object passed as argument isnt a type of java stream");
                }
            } 
            else
            {
                LOGE("Unable to get java classes to perform verification on stream");
            }
        }

        bool AStream::Init(AStreamType streamType, jobject javaStream)
        {
            AJniHelper jni;
            bool result = false;
            switch (streamType)
            {
            case AStream::INVALID:
                LOGE("Invalid streamType");
                return false;
                break;
            case AStream::READ:
                this->javaStream = reinterpret_cast<jobject>(jni->NewGlobalRef(javaStream));
                if (jni->ExceptionCheck()) LOGE("Could not make AStream global ref");
                this->streamType = AStream::READ;
                canRead = true;
                break;
            case AStream::WRITE:
                this->javaStream = reinterpret_cast<jobject>(jni->NewGlobalRef(javaStream));
                if (jni->ExceptionCheck()) LOGE("Could not make AStream global ref");
                this->streamType = AStream::WRITE;
                canWrite = true;
                break;
            };
            if (!midsCached) return CacheMids();
            return true;
        }

        AStream::~AStream()
        {
            AJniHelper jni;
            this->Close();
        }
        
        void AStream::Flush()
        {
            AJniHelper jni;
            if (CanRead() && (streamType == AStream::WRITE))
            {
                jni->CallObjectMethod(javaStream, flushMid);
                if (jni->ExceptionCheck()) LOGE("Could not flush AStream");
            }
            else
            {
                XLI_THROW_STREAM_CANT_WRITE;
            }
        }

        void AStream::Close()
        {
            AJniHelper jni;
            if (IsClosed()) return;
            switch(streamType) 
            {
            case AStream::INVALID:
                XLI_THROW_STREAM_CLOSED;
                break;
            case AStream::READ:
                jni->CallObjectMethod(javaStream, closeReadMid);
                closed = true;
                break;
            case AStream::WRITE:
                jni->CallObjectMethod(javaStream, closeWriteMid);
                closed = true;
                break;
            };
            if (jni->ExceptionCheck()) LOGE("Could not close AStream");
            if (this->javaStream) 
            { 
                jni->DeleteGlobalRef(this->javaStream);
                this->javaStream = 0;
            }
        }

        bool AStream::IsClosed() const
        {
            return closed;
        }

        bool AStream::CanRead() const
        {
            return canRead;
        }

        bool AStream::CanWrite() const
        {
            return canWrite;
        }

        bool AStream::AtEnd() const
        {
            return this->atEnd;
        }

        int AStream::Read(void* dst, int elmSize, int elmCount)
        {

            if (!CanRead()) XLI_THROW_STREAM_CANT_READ;
            if (IsClosed()) XLI_THROW_STREAM_CLOSED;
            int fetchBytes = elmCount * elmSize;
            int recievedBytes = PlatformSpecific::AShim::ReadBytesFromInputStream(this->javaStream, fetchBytes, dst);
            if (recievedBytes < 0) {
                atEnd = true;
                return -1;
            }
            return (recievedBytes / elmSize);
        }

    //[TODO] memoverflow?
        void AStream::Write(const void* data, int elmSize, int elmCount)
        {
            if (!CanWrite()) XLI_THROW_STREAM_CANT_WRITE;
            if (IsClosed()) XLI_THROW_STREAM_CLOSED;

            AJniHelper jni;

            int pushBytes = elmCount * elmSize;
            jbyteArray arr = jni->NewByteArray(pushBytes);

            jni->SetByteArrayRegion(arr, 0, pushBytes, (jbyte *)data);
            jni->CallObjectMethod(javaStream, writeBufferDetailedMid, arr, 0, pushBytes);
            if (jni->ExceptionCheck()) {
                LOGE("Index out of bounds for data");
                XLI_THROW_STREAM_CANT_WRITE;
            }

            jni->GetByteArrayRegion(arr, 0, pushBytes, (jbyte*)data);
            jni->DeleteLocalRef(arr);
        }

        bool AStream::CacheMids()
        {
            AJniHelper jni;
            
            jclass rcls = jni->FindClass("java/io/BufferedInputStream");
            if (rcls) 
            {
                closeReadMid = jni->GetMethodID(rcls, "close", "()V");
                if (jni->ExceptionCheck()) LOGE("Could not get 'close' method");
                readByteMid = jni->GetMethodID(rcls, "read", "()I");
                if (jni->ExceptionCheck()) LOGE("Could not get 'read' method (1)");
                readBufferMid = jni->GetMethodID(rcls, "read", "([BII)I");
                if (jni->ExceptionCheck()) LOGE("Could not get 'read' method (2)");
                resetMid = jni->GetMethodID(rcls, "reset", "()V");
                if (jni->ExceptionCheck()) LOGE("Could not get 'reset' method");
                if (closeReadMid && readByteMid && readBufferMid && resetMid)
                {
                    AStream::midsCached = 1;
                } else {
                    LOGE("Some read mids were not found");
                    return false;
                }
            }
            else
            {
                LOGE("Could not get java cls InputStream to cache mids");
                return false;
            }

            jclass wcls = jni->FindClass("java/io/OutputStream");
            if (wcls) 
            {
                closeWriteMid = jni->GetMethodID(wcls, "close", "()V");
                if (jni->ExceptionCheck()) LOGE("Could not get 'close' method");
                flushMid = jni->GetMethodID(wcls, "flush", "()V");
                if (jni->ExceptionCheck()) LOGE("Could not get 'flush' method");
                writeBufferMid = jni->GetMethodID(wcls, "write", "([B)V");
                if (jni->ExceptionCheck()) LOGE("Could not get 'write' method (1)");
                writeBufferDetailedMid = jni->GetMethodID(wcls, "write", "([BII)V");
                if (jni->ExceptionCheck()) LOGE("Could not get 'write' method (2)");
                if (closeWriteMid && flushMid && writeBufferMid && writeBufferDetailedMid)
                {
                    AStream::midsCached = 1;
                } else {
                    LOGE("Some write mids were not found");
                    return false;
                }
            }
            else
            {
                LOGE("Could not get java cls OutputStream to cache mids");
                return false;
            }
            return true;
        }
    }
}
