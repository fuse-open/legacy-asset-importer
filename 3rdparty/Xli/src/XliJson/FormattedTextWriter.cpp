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

#include <XliJson/FormattedTextWriter.h>

namespace Xli
{
    FormattedTextWriter::FormattedTextWriter(Stream* stream, String indentValue)
        : StreamWriterBase(stream)
    {
        this->indentValue = indentValue;
        indent = 0;
        lineCount = 1;
        skipLine = false;
        disableSkip = false;
    }

    void FormattedTextWriter::WriteRaw(const char* str, int len)
    {
        _stream->Write(str, 1, len);
    }

    void FormattedTextWriter::SkipLine()
    {
        skipLine = true;
    }

    void FormattedTextWriter::DisableSkip()
    {
        disableSkip = true;
    }

    void FormattedTextWriter::PushIndent()
    {
        indent++;
    }

    void FormattedTextWriter::PopIndent()
    {
        indent--;
    }

    void FormattedTextWriter::WriteIndent()
    {
        for (int i = 0; i < indent; i++)
            WriteRaw(indentValue.Ptr(), indentValue.Length());
    }

    void FormattedTextWriter::Write(const String& str)
    {
        int start = 0;

        for (int i = 0; i < str.Length(); i++)
        {
            if (str[i] == '\n')
            {
                WriteRaw(str.Ptr() + start, 1 + i - start);
                start = i + 1;
                lineCount++; 
                WriteIndent();
            }
        }

        WriteRaw(str.Ptr() + start, str.Length() - start);
    }

    void FormattedTextWriter::EndLine()
    {
        WriteRaw("\n", 1);
        lineCount++;
    }

    void FormattedTextWriter::BeginLine()
    {
        if (skipLine && lineCount > 1 && !disableSkip)
        {
            skipLine = false;
            EndLine();
        }

        if (disableSkip)
        {
            skipLine = false;
            disableSkip = false;
        }

        WriteIndent();
    }

    void FormattedTextWriter::WriteLine(const String& str)
    {
        BeginLine();
        Write(str);
        EndLine();
    }
    
    void FormattedTextWriter::NewLine()
    {
        EndLine();
    }
}
