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

#include <Xli/Console.h>
#include <Xli/Path.h>
#include <Xli/Random.h>
#include <Xli/Unicode.h>
#include <Xli/Disk.h>
#include <ShlObj.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Undefine conflicting names
#undef MessageBox
#undef GetSystemDirectory
#undef GetCurrentDirectory
#undef SetCurrentDirectory
#undef CreateDirectory
#undef ChangeDirectory
#undef DeleteFile
#undef MoveFile
#undef CreateMutex
#undef GetCurrentThread

namespace Xli
{
    namespace PlatformSpecific
    {
        String GetLastErrorString()
        {
            LPWSTR lpMsgBuf;
            DWORD dw = GetLastError();

            FormatMessageW(
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                dw,
                MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
                (LPWSTR)&lpMsgBuf, // Cast because callee is allocating buffer
                0, NULL);

            String msg = Unicode::Utf16To8(lpMsgBuf);
            LocalFree(lpMsgBuf);

            return msg;
        }

        static UInt64 ConvertToUInt64(const DWORD& high, const DWORD& low)
        {
            return ((UInt64)high << 32) | low;
        }

        static Timestamp ConvertToTimestamp(const FILETIME& ft)
        {
            return ConvertToUInt64(ft.dwHighDateTime, ft.dwLowDateTime);
        }

        class Win32FileSystem : public NativeFileSystem
        {
        public:
            Stream* OpenFile(const String& filename, FileMode mode)
            {
                return new File(filename, mode);
            }

            virtual String GetBaseDirectory()
            {
                const size_t BufSize = 4096;

                WCHAR buf[BufSize];
                DWORD result = GetModuleFileNameW(GetModuleHandle(0), buf, BufSize);

                if (result > 0 && result < BufSize)
                {
                    String exe = Unicode::Utf16To8(buf, (int)result);

                    for (int i = 0; i < exe.Length(); i++)
                        if (exe[i] == '\\')
                            exe[i] = '/';

                    return Path::GetDirectoryName(exe);
                }

                return "";
            }

            virtual String CreateTempFilename()
            {
                Random rand(GetTickCount());
                static const char* cs = "-_1234567890abcdefghijklmnopqrstuvwxyz";
                int cl = (int)strlen(cs);

                WCHAR preW[] =
                {
                    (WCHAR)(cs[rand.NextInt(cl)]),
                    (WCHAR)(cs[rand.NextInt(cl)]),
                    (WCHAR)(cs[rand.NextInt(cl)]),
                    0
                };

                WCHAR pathW[MAX_PATH];
                WCHAR fnW[MAX_PATH];
                GetTempPathW(MAX_PATH, pathW);
                GetTempFileNameW(pathW, preW, 0, fnW);

                String result = Unicode::Utf16To8(fnW);

                for (int i = 0; i < result.Length(); i++)
                    if (result[i] == '\\')
                        result[i] = '/';

                return result;
            }

            static String GetKnownDirectory(REFKNOWNFOLDERID rfid)
            {
                String result = ".";
                PWSTR pszPathW;

                if (::SHGetKnownFolderPath(rfid, KF_FLAG_NO_ALIAS, 0, &pszPathW) == S_OK)
                {
                    result = Unicode::Utf16To8(pszPathW);
                    CoTaskMemFree(pszPathW);
                }

                for (int i = 0; i < result.Length(); i++)
                    if (result[i] == '\\')
                        result[i] = '/';

                return result;
            }

            virtual String GetSystemDirectory(SystemDirectory dir)
            {
                switch (dir)
                {
                default:
                    Error->WriteLine("WARNING: Unknown system directory: " + (String)(int)dir);
                    return GetKnownDirectory(FOLDERID_Documents);

                case SystemDirectoryDocuments:
                    return GetKnownDirectory(FOLDERID_Documents);

                case SystemDirectoryConfig:
                    return GetKnownDirectory(FOLDERID_RoamingAppData);

                case SystemDirectoryData:
                    return GetKnownDirectory(FOLDERID_LocalAppData);
                }
            }

            virtual String GetCurrentDirectory()
            {
                WCHAR bufW[4096];
                ::GetCurrentDirectoryW(4096, bufW);

                String result = Unicode::Utf16To8(bufW);

                for (int i = 0; i < result.Length(); i++)
                    if (result[i] == '\\')
                        result[i] = '/';

                return result;
            }

            virtual void SetCurrentDirectory(const String& dir)
            {
                Utf16String dirW = Unicode::Utf8To16(dir);
                ::SetCurrentDirectoryW(dirW.Ptr());
            }

            virtual void CreateDirectory(const String& path)
            {
                // silent ignore on disk roots
                if (path.EndsWith(":")) 
                    return;

                Utf16String pathW = Unicode::Utf8To16(path);

                if (!CreateDirectoryW(pathW.Ptr(), 0))
                {
                    DWORD err = GetLastError();
                    switch (err)
                    {
                    case ERROR_ALREADY_EXISTS:
                        break;

                    case ERROR_PATH_NOT_FOUND:
                    default:
                        XLI_THROW("Couldn't create directory: " + path);
                    };
                }
            }

            virtual void DeleteDirectory(const String& name)
            {
                Utf16String nameW = Unicode::Utf8To16(name);

                if (!RemoveDirectoryW(nameW.Ptr()))
                {
                    XLI_THROW("Unable to delete directory '" + name + "': " + GetLastErrorString());
                }
            }

            virtual void DeleteFile(const String& name)
            {
                Utf16String nameW = Unicode::Utf8To16(name);

                if (!DeleteFileW(nameW.Ptr()))
                    XLI_THROW("Unable to delete file '" + name + "': " + GetLastErrorString());
            }

            virtual void MoveDirectory(const String& oldName, const String& newName)
            {
                Utf16String oldNameW = Unicode::Utf8To16(oldName);
                Utf16String newNameW = Unicode::Utf8To16(newName);

                if (!MoveFileW(oldNameW.Ptr(), newNameW.Ptr()))
                    XLI_THROW("Unable to move directory '" + oldName + "' to '" + newName + "': " + GetLastErrorString());
            }

            virtual void MoveFile(const String& oldName, const String& newName)
            {
                Utf16String oldNameW = Unicode::Utf8To16(oldName);
                Utf16String newNameW = Unicode::Utf8To16(newName);

                if (!MoveFileW(oldNameW.Ptr(), newNameW.Ptr()))
                    XLI_THROW("Unable to move file '" + oldName + "' to '" + newName + "': " + GetLastErrorString());
            }

            virtual bool GetFileInfo(const String& path, FileInfo& info)
            {
                Utf16String pathW = Unicode::Utf8To16(path);

                WIN32_FILE_ATTRIBUTE_DATA data;

                if (GetFileAttributesEx(pathW.Ptr(), GetFileExInfoStandard, &data))
                {
                    info.Name = path;
                    info.Attributes = 0;
                    info.LastAccessTime = ConvertToTimestamp(data.ftLastAccessTime);
                    info.LastWriteTime = ConvertToTimestamp(data.ftLastWriteTime);
                    info.CreationTime = ConvertToTimestamp(data.ftCreationTime);
                    info.Size = ConvertToUInt64(data.nFileSizeHigh, data.nFileSizeLow);
                    
                    if (data.dwFileAttributes & FILE_ATTRIBUTE_READONLY) 
                        info.Attributes |= FileAttributesReadOnly;
                    
                    if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
                        info.Attributes |= FileAttributesDirectory;
                    
                    return true;
                }

                return false;
            }

            virtual void GetFiles(const String& path, Array<FileInfo>& list)
            {
                String filter = path.Length() ?
                    path + (path.Last() == '/' ? "*" : "/*") :
                    "*";

                Utf16String filterW = Unicode::Utf8To16(filter);

                WIN32_FIND_DATA findData;
                HANDLE h = FindFirstFile((LPCWSTR)filterW.Ptr(), &findData);

                if (h != INVALID_HANDLE_VALUE)
                {
                    do
                    {
                        FileInfo info;
                        info.Name = Unicode::Utf16To8(findData.cFileName);

                        if (info.Name == "." || info.Name == "..")
                            continue;

                        if (path.Length() && path != "." && path != "./")
                        {
                            if (path.Last() == '/')
                                info.Name = path + info.Name;
                            else
                                info.Name = path + "/" + info.Name;
                        }

                        info.Attributes = 0;
                        info.LastAccessTime = ConvertToTimestamp(findData.ftLastAccessTime);
                        info.LastWriteTime = ConvertToTimestamp(findData.ftLastWriteTime);
                        info.CreationTime = ConvertToTimestamp(findData.ftCreationTime);
                        info.Size = ConvertToUInt64(findData.nFileSizeHigh, findData.nFileSizeLow);
                        
                        if (findData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) 
                            info.Attributes |= FileAttributesReadOnly;
                        
                        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
                            info.Attributes |= FileAttributesDirectory;

                        list.Add(info);

                    } 
                    while (FindNextFile(h, &findData) != 0);
                }

                FindClose(h);
            }
        };
    }

    NativeFileSystem* CreateDiskFileSystem()
    {
        return new PlatformSpecific::Win32FileSystem();
    }
}
