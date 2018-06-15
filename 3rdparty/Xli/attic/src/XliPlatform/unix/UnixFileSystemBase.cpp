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

#include "UnixFileSystemBase.h"

#include <Xli/Time.h>
#include <Xli/Random.h>
#include <Xli/Traits.h>
#include <Xli/DateTime.h>
#include <Xli/Path.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace Xli
{
    namespace PlatformSpecific
    {
        Timestamp ConvertToTimestamp(time_t time);

        Stream* UnixFileSystemBase::OpenFile(const String& filename, FileMode mode)
        {
            return new File(filename, mode);
        }

        String UnixFileSystemBase::CreateTempFilename()
        {
            String prefix = Path::Combine(GetTempDirectory(), (String)(int)getpid() + "-");

            Random rand(DefaultTraits::Hash(GetTimestamp()));
            static const char* cs = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
            int cl = strlen(cs);

            while (true)
            {
                String tmp = String::Create(8);

                for (int i = 0; i < tmp.Length(); i++)
                    tmp[i] = cs[rand.NextInt(cl)];

                String result = prefix + tmp;

                struct stat attributes;
                if (stat(result.Ptr(), &attributes) == -1)
                    return result;
            }
        }

        String UnixFileSystemBase::GetCurrentDirectory()
        {
            char buf[1024];
            const char* ptr = getcwd(buf, 1024);
            
            if (ptr != buf) 
                return ".";
            
            return buf;
        }
            
        void UnixFileSystemBase::SetCurrentDirectory(const String& path)
        {
            if (!path.Length()) 
                return;

            if (chdir(path.Ptr()) != 0)
                XLI_THROW("Unable to change directory to '" + path + "'");
        }

        void UnixFileSystemBase::CreateDirectory(const String& path)
        {
            struct stat st;

            if (!path.Length() || 
                path == "~" ||
                stat(path.Ptr(), &st) == 0) 
                return;

            if (mkdir(path.Ptr(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0)
            {
                if (errno == EEXIST) 
                    return;

                XLI_THROW("Unable to create directory '" + path + "'");
            }
        }

        void UnixFileSystemBase::DeleteDirectory(const String& path)
        { 
            if (rmdir(path.Ptr()) != 0)
                XLI_THROW("Unable to delete directory '" + path + "'");
        }

        void UnixFileSystemBase::DeleteFile(const String& path)
        { 
            if (unlink(path.Ptr()) != 0)
                XLI_THROW("Unable to delete file '" + path + "'");
        }

        void UnixFileSystemBase::MoveDirectory(const String& oldPath, const String& newPath)
        { 
            if (rename(oldPath.Ptr(), newPath.Ptr()) != 0)
                XLI_THROW("Unable to move directory '" + oldPath + "' to '" + newPath + "'");
        }
            
        void UnixFileSystemBase::MoveFile(const String& oldPath, const String& newPath)
        { 
            if (rename(oldPath.Ptr(), newPath.Ptr()) != 0)
                XLI_THROW("Unable to move file '" + oldPath + "' to '" + newPath + "'");
        }

        bool UnixFileSystemBase::GetFileInfo(const String& path, FileInfo& f)
        {
            struct stat attributes;

            if (stat(path.Ptr(), &attributes) == -1)
                return false;

            f.Name = path;

            f.Attributes = 0;
            if (!(((attributes.st_mode & S_IWOTH) == S_IWOTH)
                || (attributes.st_gid == getgid() && ((attributes.st_mode & S_IWGRP) == S_IWGRP))
                || (attributes.st_uid == getuid() && ((attributes.st_mode & S_IWUSR) == S_IWUSR))))
                f.Attributes |= FileAttributesReadOnly;

            if (S_ISDIR(attributes.st_mode))
                f.Attributes |= FileAttributesDirectory;

            f.CreationTime = ConvertToTimestamp(attributes.st_mtime);
            f.LastAccessTime = ConvertToTimestamp(attributes.st_atime);
            f.LastWriteTime = ConvertToTimestamp(attributes.st_mtime);

            return true;
        }

        void UnixFileSystemBase::GetFiles(const String& path, Array<FileInfo>& list)
        {
            String prefix = 
                path.Length() > 0 && path.Last() != '/' ? 
                    path + "/" : 
                    path;

            DIR *dp;
            struct dirent *ep;

            if ((dp = opendir(prefix.Ptr())) == NULL)
                XLI_THROW_FILE_NOT_FOUND(prefix);

            if (prefix == "./")
                prefix = "";

            while ((ep = readdir(dp)) != NULL)
            {
                String fn = ep->d_name;
                
                if (fn == "." || fn == "..") 
                    continue;

                FileInfo info;
                if (GetFileInfo(prefix + fn, info))
                    list.Add(info);
            }

            closedir(dp);
        }
    }
}
