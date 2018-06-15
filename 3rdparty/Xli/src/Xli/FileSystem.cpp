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

#include <Xli/FileSystem.h>
#include <Xli/Path.h>
#include <Xli/Shared.h>

namespace Xli
{
    FileSystem::~FileSystem()
    {
    }

    DataAccessor* FileSystem::OpenFileAsBuffer(const String& filename)
    {
        Managed<Stream> f = OpenFile(filename, FileModeRead);
        return f->CreateDataAccessor();
    }

    void FileSystem::CreateDirectory(const String& name) 
    { 
        XLI_THROW_NOT_SUPPORTED(XLI_FUNCTION);
    }

    void FileSystem::DeleteDirectory(const String& name)
    { 
        XLI_THROW_NOT_SUPPORTED(XLI_FUNCTION);
    }

    void FileSystem::DeleteFile(const String& name)
    { 
        XLI_THROW_NOT_SUPPORTED(XLI_FUNCTION);
    }

    void FileSystem::MoveDirectory(const String& oldName, const String& newName)
    { 
        XLI_THROW_NOT_SUPPORTED(XLI_FUNCTION);
    }
        
    void FileSystem::MoveFile(const String& oldName, const String& newName)
    { 
        XLI_THROW_NOT_SUPPORTED(XLI_FUNCTION);
    }

    bool FileSystem::GetFileInfo(const String& path, FileInfo& result) 
    { 
        //XLI_THROW_NOT_SUPPORTED(XLI_FUNCTION);
        return false;
    }

    void FileSystem::GetFiles(const String& path, Array<FileInfo>& list) 
    { 
        //XLI_THROW_NOT_SUPPORTED(XLI_FUNCTION);
    }

    void FileSystem::GetFiles(Array<FileInfo>& list) 
    { 
        GetFiles(".", list); 
    }

    bool FileSystem::Exists(const String& path)
    {
        FileInfo result;
        return GetFileInfo(path, result);
    }

    bool FileSystem::IsFile(const String& path)
    {
        FileInfo result;
        return GetFileInfo(path, result) && (result.Attributes & FileAttributesDirectory) == 0;
    }

    bool FileSystem::IsDirectory(const String& path)
    {
        FileInfo result;
        return GetFileInfo(path, result) && (result.Attributes & FileAttributesDirectory) == FileAttributesDirectory;
    }

    void FileSystem::CreateDirectories(const String& path)
    {
        Array<String> parts;
        path.Split('/', parts);

        String currentDir;
        for (int i = 0; i < parts.Length(); i++)
        {
            currentDir = currentDir + parts[i];

            if (parts[i].Length() > 0 && parts[i] != "." && parts[i] != "..")
                CreateDirectory(currentDir);

            currentDir = currentDir + "/";
        }
    }

    void FileSystem::DeleteDirectoryRecursive(const String& path)
    {
        Array<FileInfo> files;
        GetFiles(path, files);

        for (int i = 0; i < files.Length(); i++)
            if (files[i].Attributes & FileAttributesDirectory)
                DeleteDirectoryRecursive(files[i].Name);
            else
                DeleteFile(files[i].Name);

        DeleteDirectory(path);
    }

    class SubFileSystem: public FileSystem
    {
        Shared<FileSystem> fs;
        String path;

    public:
        SubFileSystem(FileSystem* fs, const String& path);

        virtual Stream* OpenFile(const String& filename, FileMode mode = FileModeRead);
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

        virtual void CreateDirectories(const String& path);

        virtual void DeleteDirectoryRecursive(const String& name);

        virtual FileSystem* CreateSubFileSystem(const String& path);
    };

    FileSystem* FileSystem::CreateSubFileSystem(const String& path)
    {
        return new SubFileSystem(this, path);
    }

    SubFileSystem::SubFileSystem(FileSystem* fs, const String& path)
    {
        this->fs = fs;
        this->path = path;
        
        if (path.Length() > 0 && path.Last() != '/') 
            this->path = this->path + "/";
    }

    Stream* SubFileSystem::OpenFile(const String& filename, FileMode mode)
    {
        return fs->OpenFile(Path::Combine(path, filename), mode);
    }

    DataAccessor* SubFileSystem::OpenFileAsBuffer(const String& filename)
    {
        return fs->OpenFileAsBuffer(Path::Combine(path, filename));
    }

    void SubFileSystem::CreateDirectory(const String& name)
    {
        fs->CreateDirectory(Path::Combine(path, name));
    }

    void SubFileSystem::DeleteDirectory(const String& name)
    {
        fs->DeleteDirectory(Path::Combine(path, name));
    }

    void SubFileSystem::DeleteFile(const String& name)
    {
        fs->DeleteFile(Path::Combine(path, name));
    }

    void SubFileSystem::MoveDirectory(const String& oldName, const String& newName)
    {
        fs->MoveDirectory(Path::Combine(path, oldName), Path::Combine(path, newName));
    }

    void SubFileSystem::MoveFile(const String& oldName, const String& newName)
    {
        fs->MoveFile(Path::Combine(path, oldName), Path::Combine(path, newName));
    }

    bool SubFileSystem::Exists(const String& name)
    {
        return fs->Exists(Path::Combine(path, name));
    }

    bool SubFileSystem::IsFile(const String& name)
    {
        return fs->IsFile(Path::Combine(path, name));
    }

    bool SubFileSystem::IsDirectory(const String& name)
    {
        return fs->IsDirectory(Path::Combine(path, name));
    }
    
    // TODO: Path::Combine
    bool SubFileSystem::GetFileInfo(const String& path, FileInfo& result)
    {
        FileInfo info;
        if (fs->GetFileInfo(this->path + path, info))
        {
            info.Name = info.Name.Substring(this->path.Length(), info.Name.Length() - this->path.Length());
            return true;
        }
        
        return false;
    }

    // TODO: Path::Combine
    void SubFileSystem::GetFiles(const String& path, Array<FileInfo>& list)
    {
        int s = list.Length();
        fs->GetFiles(this->path + path, list);

        for (int i = s; i < list.Length(); i++)
            list[i].Name = list[i].Name.Substring(this->path.Length(), list[i].Name.Length() - this->path.Length());
    }

    // TODO: Path::Combine
    void SubFileSystem::GetFiles(Array<FileInfo>& list)
    {
        int s = list.Length();
        fs->GetFiles(this->path + path, list);

        for (int i = s; i < list.Length(); i++)
            list[i].Name = list[i].Name.Substring(this->path.Length(), list[i].Name.Length() - this->path.Length());
    }

    void SubFileSystem::CreateDirectories(const String& path)
    {
        fs->CreateDirectories(Path::Combine(this->path, path));
    }

    void SubFileSystem::DeleteDirectoryRecursive(const String& name)
    {
        fs->DeleteDirectoryRecursive(Path::Combine(this->path, path));
    }

    FileSystem* SubFileSystem::CreateSubFileSystem(const String& path)
    {
        return new SubFileSystem(fs.Get(), Path::Combine(this->path, path));
    }
}
