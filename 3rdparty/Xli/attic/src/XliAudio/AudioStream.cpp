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

#include <Xli/Shared.h>
#include <XliAudio/AudioStream.h>

namespace Xli
{
    class DefaultAudioStream: public AudioStream
    {
        Shared<Stream> source;
        DataType type;
        int channels;
        double sampleRate;

    public:
        DefaultAudioStream(Stream* source, DataType dataType, int channelCount, double sampleRate)
        {
            this->source = source;
            this->type = dataType;
            this->channels = channelCount;
            this->sampleRate = sampleRate;
        }

        virtual double GetSampleRate() const
        {
            return sampleRate;
        }
        
        virtual int GetChannelCount() const
        {
            return channels;
        }

        virtual DataType GetDataType() const
        {
            return type;
        }

        virtual int Read(void* dst, int elmSize, int elmCount)
        {
            return source->Read(dst, elmSize, elmCount);
        }
    };

    int AudioStream::GetAverageBytesPerSecond() const
    {
        return (int)((double)(DataTypeInfo::SizeOf(GetDataType()) * GetChannelCount()) * GetSampleRate());
    }

    AudioStream* AudioStream::Create(Stream* source, DataType dataType, int channelCount, double sampleRate)
    {
        return new DefaultAudioStream(source, dataType, channelCount, sampleRate);
    }
}
