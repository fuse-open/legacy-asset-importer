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

#include <Xli/Path.h>
#include <Xli/Exception.h>
#include <Xli/String.h>

namespace Xli
{
    static bool IsRooted(const String& path)
    {
        return 
            (path.Length() > 0 && (path[0] == '/' || path[0] == '\\')) ||
            (path.Length() > 1 && path[1] == ':');
    }

    String Path::Combine(const String& path1, const String& path2)
    {
        return 
            IsRooted(path2) ?
                path2 :
                path1.Length() && path1.Last() != '/' && path1.Last() != '\\' ?
                    path1 + "/" + path2 :
                    path1 + path2;
    }

    String Path::GetFilename(const String& path)
    {
        for (int i = path.Length() - 1; i >= 0; i--)
            switch (path[i])
            {
                case '/':
                case '\\':
                    return path.Substring(i + 1);
            }

        return path;
    }

    String Path::GetFilenameWithoutExtension(const String& path)
    {
        String filename = GetFilename(path);

        for (int i = filename.Length() - 1; i >= 0; i--)
            if (filename[i] == '.')
                return filename.Substring(0, i);

        return filename;
    }

    String Path::GetExtension(const String& path)
    {
        for (int i = path.Length() - 1; i >= 0; i--)
            switch (path[i])
            {
            case '.':
                return path.Substring(i);

            case '/':
            case '\\':
                return "";
            }

        return "";
    }

    String Path::GetDirectoryName(const String& path)
    {
        for (int i = path.Length() - 1; i >= 0; i--)
            switch (path[i])
            {
            case '/':
            case '\\':
                return path.Substring(0, i);
            }

        return "";
    }
}
