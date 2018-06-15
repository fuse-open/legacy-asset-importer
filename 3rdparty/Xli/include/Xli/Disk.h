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

#ifndef __XLI_DISK_H__
#define __XLI_DISK_H__

#include <Xli/FileSystem.h>

namespace Xli
{
    /**
        \ingroup XliIO
    */
    enum SystemDirectory
    {
        SystemDirectoryConfig, //< Returns the path on the file system where to store roaming app data
        SystemDirectoryData, //< Returns the path on the file system where to store local app data
        SystemDirectoryDesktop,
        SystemDirectoryDownloads,
        SystemDirectoryTemplates,
        SystemDirectoryPublic,
        SystemDirectoryDocuments, //< Returns the path on the file system where the user keeps his personal documents. E.g. the "My Documents" folder
        SystemDirectoryMusic,
        SystemDirectoryPictures,
        SystemDirectoryVideos,
    };

    /**
        \ingroup XliIO
    */
    class NativeFileSystem : public FileSystem
    {
    public:
        /**
            Generates and returns a unique filename that can be used for creating a temporary file. The temporary file must be deleted manually by the creator.
        */
        virtual String CreateTempFilename() = 0;

        /**
            Returns a system directory
        */
        virtual String GetSystemDirectory(SystemDirectory dir) = 0;

        /**
            Returns the directory where application is run from
        */
        virtual String GetBaseDirectory() = 0;

        /**
            Returns the working directory
        */
        virtual String GetCurrentDirectory() = 0;

        /**
            Sets the working directory
        */
        virtual void SetCurrentDirectory(const String& dir) = 0;
    };

    /**
        \ingroup XliIO
    */
    class DiskAccessor
    {
    public:
        NativeFileSystem* operator ->();
        operator FileSystem*();
    };

    /**
        \ingroup XliIO
    */
    extern DiskAccessor Disk;
}


#endif
