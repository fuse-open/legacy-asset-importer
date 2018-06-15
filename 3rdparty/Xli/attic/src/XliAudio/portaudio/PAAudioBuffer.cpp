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

#include <XliAudio/AudioBuffer.h>
#include <Xli/MessageBox.h>
#include <Xli/Shared.h>
#include <portaudio.h>
#include <cstdlib>
#include <cstring>

namespace Xli
{
    class PAAudioBuffer: public AudioBuffer
    {
        Shared<Stream> src;
        DataType dataType;
        int channels;
        double sampleRate;
        PaStream* stream;

        static int paCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *outTime, PaStreamCallbackFlags statusFlags, void *userData)
        {
            PAAudioBuffer* s = (PAAudioBuffer*)userData;
            
            int elmSize = DataTypeInfo::SizeOf(s->dataType);
            int elmCount = s->channels * framesPerBuffer;

            try
            {
                for (;;)
                {
                    int result = s->src->Read(outputBuffer, elmSize, elmCount);

                    if (result < elmCount)
                    {
                        elmCount -= result;
                        outputBuffer = (UInt8*)outputBuffer + result * elmSize;

                        if (s->src->AtEnd() || result == 0)
                        {
                            memset((UInt8*)outputBuffer, 0, elmCount * elmSize);
                            return paComplete;
                        }

                        continue;
                    }

                    break;
                }

                return paContinue;
            }
            catch (const Exception& e)
            {
                memset((UInt8*)outputBuffer, 0, elmCount * elmSize);
                MessageBox::HandleException(e, "Sound: " + s->src->ToString());
                return paAbort;
            }
        }

        static PaSampleFormat XliDataTypeToPaSampleFormat(DataType dataType)
        {
            switch (dataType)
            {
            case DataTypeInt8: return paInt8;
            case DataTypeInt16: return paInt16;
            case DataTypeInt24: return paInt24;
            case DataTypeInt32: return paInt32;
            case DataTypeFloat: return paFloat32;
            case DataTypeUInt8: return paUInt8;
            default: XLI_THROW("Unsupported data type: " + (String)DataTypeInfo::ToString(dataType));
            }
        }

    public:
        PAAudioBuffer(Stream* source, DataType dataType, int channels, double sampleRate, int latency, int framesPerBuffer)
        {
            this->src = source;
            this->dataType = dataType;
            this->channels = channels;
            this->sampleRate = sampleRate;

            PaStreamParameters outputParams;
            outputParams.device = Pa_GetDefaultOutputDevice();
            outputParams.channelCount = channels;
            outputParams.sampleFormat = XliDataTypeToPaSampleFormat(dataType);
            outputParams.suggestedLatency = (double)latency / 1000.0;
            outputParams.hostApiSpecificStreamInfo = 0;

            int err = Pa_OpenStream(&stream, 0, &outputParams, sampleRate, framesPerBuffer, 0, paCallback, this);

            if (err)
            {
                XLI_THROW("Unable to open sound stream: " + (String)Pa_GetErrorText(err));
            }
        }

        PAAudioBuffer(Stream* source, DataType dataType, int channels, double sampleRate)
        {
            this->src = source;
            this->dataType = dataType;
            this->channels = channels;
            this->sampleRate = sampleRate;

            int err = Pa_OpenDefaultStream(&stream, 0, channels, XliDataTypeToPaSampleFormat(dataType), sampleRate, 0, paCallback, this);

            if (err)
            {
                XLI_THROW("Unable to open sound stream: " + (String)Pa_GetErrorText(err));
            }
        }

        virtual ~PAAudioBuffer()
        {
            Pa_CloseStream(stream);
        }

        virtual void Play()
        {
            int err = Pa_StartStream(stream);

            if (err)
            {
                XLI_THROW("Unable to start sound stream: " + (String)Pa_GetErrorText(err));
            }
        }
        
        virtual void Stop()
        {
            int err = Pa_StopStream(stream);

            if (err)
            {
                String str = "Unable to start sound stream: " + (String)Pa_GetErrorText(err);
                XLI_THROW(str);
            }
        }

        virtual double GetLatency() const
        {
            return Pa_GetStreamInfo(stream)->outputLatency;
        }

        virtual DataType GetDataType() const
        {
            return dataType;
        }

        virtual int GetChannelCount() const
        {
            return channels;
        }

        virtual double GetSampleRate() const
        {
            return sampleRate;
        }
    };

    static int SoundRefCount = 0;

    void AudioBuffer::Init()
    {
        if (SoundRefCount == 0)
        {
            int err = Pa_Initialize();

            if (err)
            {
                XLI_THROW("Unable to initialize PortAudio: " + (String)Pa_GetErrorText(err));
            }
        }

        SoundRefCount++;
    }

    void AudioBuffer::Done()
    {
        SoundRefCount--;

        if (SoundRefCount == 0)
        {
            Pa_Terminate();
        }
        if (SoundRefCount < 0)
        {
            XLI_THROW_BAD_DELETE;
        }
    }

    static void AssertInit()
    {
        if (!SoundRefCount)
        {
            AudioBuffer::Init();
            atexit(AudioBuffer::Done);
        }
    }

    AudioBuffer* AudioBuffer::Create(Stream* source, DataType dataType, int channels, double sampleRate, int latency, int framesPerBuffer)
    {
        AssertInit();
        return new PAAudioBuffer(source, dataType, channels, sampleRate, latency, framesPerBuffer);
    }

    AudioBuffer* AudioBuffer::Create(Stream* source, DataType dataType, int channels, double sampleRate)
    {
        AssertInit();
        return new PAAudioBuffer(source, dataType, channels, sampleRate);
    }
}
