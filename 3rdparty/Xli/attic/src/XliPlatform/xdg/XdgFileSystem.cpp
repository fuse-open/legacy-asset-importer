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

#include "../unix/UnixFileSystemBase.h"
#include <Xli/Console.h>
#include <Xli/Path.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <climits>
#include <cstdlib>
#include <cstring>

namespace Xli
{
    namespace PlatformSpecific
    {
        class XdgFileSystem: public UnixFileSystemBase
        {
        public:
            virtual String GetBaseDirectory()
            {
                char path[PATH_MAX];
                char dest[PATH_MAX];
                struct stat info;
                pid_t pid = getpid();
                sprintf(path, "/proc/%d/exe", pid);
                
                if (readlink(path, dest, PATH_MAX) == -1)
                    return "";
                
                return Path::GetDirectoryName(dest);
            }

            virtual String GetTempDirectory()
            {
                const char* tmpdir = getenv("TMPDIR");
                if (tmpdir == 0 || strlen(tmpdir) == 0) tmpdir = getenv("TEMP");
                if (tmpdir == 0 || strlen(tmpdir) == 0) tmpdir = getenv("TEMPDIR");
                if (tmpdir == 0 || strlen(tmpdir) == 0) tmpdir = getenv("TMP");
                if (tmpdir == 0 || strlen(tmpdir) == 0) tmpdir = "/tmp";
                return tmpdir;
            }

            virtual String GetSystemDirectory(SystemDirectory dir)
            {
                const char* homedir = getenv("HOME");

                if (homedir == 0 || strlen(homedir) == 0) 
                    homedir = "~";

                switch (dir)
                {
                case SystemDirectoryConfig:
                    {
                        const char* dir = getenv("XDG_CONFIG_HOME");
                        return dir != 0 && strlen(dir) ? dir : (String)homedir + "/.config";
                    }

                case SystemDirectoryData:
                    {
                        const char* dir = getenv("XDG_DATA_HOME");
                        return dir != 0 && strlen(dir) ? dir : (String)homedir + "/.local/share";
                    }

                case SystemDirectoryDesktop:
                    {
                        const char* dir = getenv("XDG_DESKTOP_DIR");
                        return dir != 0 && strlen(dir) ? dir : (String)homedir + "/Desktop";
                    }

                case SystemDirectoryDownloads:
                    {
                        const char* dir = getenv("XDG_DOWNLOAD_DIR");
                        return dir != 0 && strlen(dir) ? dir : (String)homedir + "/Downloads";
                    }

                case SystemDirectoryTemplates:
                    {
                        const char* dir = getenv("XDG_TEMPLATES_DIR");
                        return dir != 0 && strlen(dir) ? dir : (String)homedir + "/Templates";
                    }

                case SystemDirectoryPublic:
                    {
                        const char* dir = getenv("XDG_PUBLICSHARE_DIR");
                        return dir != 0 && strlen(dir) ? dir : (String)homedir + "/Public";
                    }

                case SystemDirectoryDocuments:
                    {
                        const char* dir = getenv("XDG_DOCUMENTS_DIR");
                        return dir != 0 && strlen(dir) ? dir : (String)homedir + "/Documents";
                    }

                case SystemDirectoryMusic:
                    {
                        const char* dir = getenv("XDG_MUSIC_DIR");
                        return dir != 0 && strlen(dir) ? dir : (String)homedir + "/Music";
                    }

                case SystemDirectoryPictures:
                    {
                        const char* dir = getenv("XDG_PICTURES_DIR");
                        return dir != 0 && strlen(dir) ? dir : (String)homedir + "/Pictures";
                    }

                case SystemDirectoryVideos:
                    {
                        const char* dir = getenv("XDG_VIDEOS_DIR");
                        return dir != 0 && strlen(dir) ? dir : (String)homedir + "/Videos";
                    }

                default:
                    Error->WriteLine("XLI WARNING: Unknown system directory: " + (String)(int)dir);
                    return homedir;
                }
            }
        };
    }

    NativeFileSystem* CreateNativeFileSystem()
    {
        return new PlatformSpecific::XdgFileSystem();
    }
}
