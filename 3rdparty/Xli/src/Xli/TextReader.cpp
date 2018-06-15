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

#include <Xli/TextReader.h>
#include <Xli/File.h>

namespace Xli
{
    TextReader::TextReader(Stream* stream)
        : StreamReaderBase(stream)
    {
    }

    String TextReader::Read(int len)
    {
        String s = String::Create(len);
        _stream->ReadSafe(s.Ptr(), 1, len);
        return s;
    }

    String TextReader::ReadAll()
    {
        if (_stream->CanSeek())
            return Read(_stream->GetLength());
        
        Array<char> str;

        char buf[1024];
        int len = 0;
        
        while ((len = _stream->Read(buf, 1, 1024)))
            str.AddRange(buf, len);
        
        return String(str.Ptr(), str.Length());
    }

    char TextReader::ReadChar()
    {
        char c;
        _stream->ReadSafe(&c, 1, 1);
        return c;
    }

    String TextReader::ReadTo(char terminal)
    {
        Array<char> s;
        char c;
        
        while (_stream->Read(&c, 1, 1) == 1)
        {
            s.Add(c);
            
            if (c == terminal)
                break;
        }

        return String(s.Ptr(), s.Length());
    }

    String TextReader::ReadLine()
    {
        return ReadTo('\n');
    }

    bool TextReader::AtEndOfFile()
    {
        return _stream->AtEnd();
    }
}
