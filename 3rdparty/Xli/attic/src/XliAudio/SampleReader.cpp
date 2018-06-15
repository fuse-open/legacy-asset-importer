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

#include <XliAudio/SampleReader.h>

#define min(a, b) ((a) < (b) ? (a) : (b))

namespace Xli
{
    float SampleReader::convertSample(Int16 sample)
    {
        return (float)((double)sample / 32768.0);
    }

    SampleReader::SampleReader(AudioStream* stream, bool forceStereo)
    {
        this->stream = stream;

        if (stream->GetChannelCount() > 2) XLI_THROW("Sample reader stream can only have one or two channels");
        if (stream->GetDataType() != Xli::DataTypeInt16) XLI_THROW("Sample reader can only read Int16 streams");

        this->forceStereo = forceStereo;
        this->internalBufferSize = 512;
        this->internalBuffer = new Int16[this->internalBufferSize];
    }

    SampleReader::~SampleReader()
    {
        delete [] internalBuffer;
    }

    int SampleReader::Read(float* buffer, int expectedSamples)
    {
        int samplesRead = 0;
        int bufferOffset = 0;
        while (samplesRead < expectedSamples)
        {
            int samplesToRead = min(expectedSamples - samplesRead, internalBufferSize);
            int currentSamplesRead;
            if (stream->GetChannelCount() != 2 && forceStereo)
            {
                currentSamplesRead = stream->Read(internalBuffer, DataTypeInfo::SizeOf(DataTypeInt16), samplesToRead / 2);
                for (int i = 0; i < currentSamplesRead; i++)
                {
                    float sample = convertSample(internalBuffer[i]);
                    buffer[bufferOffset++] = sample;
                    buffer[bufferOffset++] = sample;
                }
            }
            else
            {
                currentSamplesRead = stream->Read(internalBuffer, DataTypeInfo::SizeOf(DataTypeInt16), samplesToRead);
                for (int i = 0; i < currentSamplesRead; i++) buffer[bufferOffset++] = convertSample(internalBuffer[i]);
            }

            samplesRead += currentSamplesRead;
            if (currentSamplesRead < samplesToRead) break;
        }
        return samplesRead;
    }
}
