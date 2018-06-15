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

#ifndef __XLI_FILESYSTEM_H__
#define __XLI_FILESYSTEM_H__

#include <Xli/File.h>

namespace Xli
{
    /**
        \ingroup XliIO
    */
    enum FileAttributes
    {
        FileAttributesReadOnly = 1 << 0,
        FileAttributesDirectory = 1 << 1
    };

    /**
        \ingroup XliIO
    */
    struct FileInfo
    {
        String Name;
        UInt64 Size;
        UInt32 Attributes;
        Timestamp CreationTime;
        Timestamp LastAccessTime;
        Timestamp LastWriteTime;
    };

    /**
        \ingroup XliIO
    */
    class FileSystem: public Object
    {
    public:
        virtual ~FileSystem();

        virtual Stream* OpenFile(const String& filename, FileMode mode = FileModeRead) = 0;
        virtual DataAccessor* OpenFileAsBuffer(const String& filename);

        virtual void CreateDirectory(const String& name);
        virtual void DeleteDirectory(const String& name);
        virtual void DeleteFile(const String& name);

        virtual void MoveDirectory(const String& oldName, const String& newName);
        virtual void MoveFile(const String& oldName, const String& newName);

        virtual bool GetFileInfo(const String& path, FileInfo& result);
        virtual void GetFiles(const String& path, Array<FileInfo>& list);

        virtual void GetFiles(Array<FileInfo>& list);

        virtual bool Exists(const String& path);
        virtual bool IsFile(const String& path);
        virtual bool IsDirectory(const String& path);

        /**
            Makes sure all directories in the path exists.
        */
        virtual void CreateDirectories(const String& path);

        /**
            Removes directory and all contents.
        */
        virtual void DeleteDirectoryRecursive(const String& name);

        virtual FileSystem* CreateSubFileSystem(const String& path);
    };
}

#endif
