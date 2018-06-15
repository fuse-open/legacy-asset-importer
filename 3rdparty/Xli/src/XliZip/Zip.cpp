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

#include <XliZip/Zip.h>
#include <Xli/BufferStream.h>
#include <Xli/Shared.h>
#include "../../3rdparty/unzip/unzip.h"

namespace Xli
{
    class ZipFileSystem: public FileSystem
    {
        Shared<Stream> file;
        zlib_filefunc_def pzlib_filefunc_def;
        unzFile handle;

    public:
        static voidpf ZCALLBACK open_file_func(voidpf opaque, const char* filename, int mode)
        {
            return opaque;
        }

        static uLong ZCALLBACK read_file_func(voidpf opaque, voidpf stream, void* buf, uLong size)
        {
            return (uLong)((Stream*)stream)->Read(buf, size, 1);
        }

        static uLong ZCALLBACK write_file_func(voidpf opaque, voidpf stream, const void* buf, uLong size)
        {
            return ((Stream*)stream)->Write((void*)buf, size, 1), size;
        }

        static long ZCALLBACK tell_file_func(voidpf opaque, voidpf stream)
        {
            return (long)((Stream*)stream)->GetPosition();
        }

        static long ZCALLBACK seek_file_func(voidpf opaque, voidpf stream, uLong offset, int origin)
        {
            switch (origin)
            {
            case ZLIB_FILEFUNC_SEEK_CUR: 
                ((Stream*)stream)->Seek((int)offset, SeekOriginCurrent);
                return 0L;
                
            case ZLIB_FILEFUNC_SEEK_END: 
                ((Stream*)stream)->Seek((int)offset, SeekOriginEnd);
                return 0L;

            case ZLIB_FILEFUNC_SEEK_SET: 
                ((Stream*)stream)->Seek((int)offset, SeekOriginBegin);
                return 0L;

            default: 
                return -1L;
            }
        }

        static int ZCALLBACK close_file_func(voidpf opaque, voidpf stream)
        {
            return 0;
        }

        static int ZCALLBACK error_file_func(voidpf opaque, voidpf stream)
        {
            return -1;
        }

        ZipFileSystem(Stream* file, const char* password)
        {
            int fpos = file->GetPosition();
            this->file = file;

            pzlib_filefunc_def.zopen_file = open_file_func;
            pzlib_filefunc_def.zread_file = read_file_func;
            pzlib_filefunc_def.zwrite_file = write_file_func;
            pzlib_filefunc_def.ztell_file = tell_file_func;
            pzlib_filefunc_def.zseek_file = seek_file_func;
            pzlib_filefunc_def.zclose_file = close_file_func;
            pzlib_filefunc_def.zerror_file = error_file_func;
            pzlib_filefunc_def.opaque = (voidpf)this->file.Get();

            handle = unzOpen2(0, &pzlib_filefunc_def);

            if (!handle)
            {
                file->Seek(fpos, SeekOriginBegin);
                XLI_THROW("Could not open Zip-archive");
            }

            if (password)
            {
                if (unzOpenCurrentFilePassword(handle, password) != UNZ_OK)
                {
                    XLI_THROW("Wrong zip password");
                }
            }
        }

        ~ZipFileSystem()
        {
            unzClose(handle);
        }

        virtual Stream* OpenFile(const String& filename, FileMode fileMode)
        {
            if (fileMode != FileModeRead) XLI_THROW("Invalid FileMode; Zip is Read-Only");

            if (unzLocateFile((unzFile)handle, filename.Ptr(), 2) != UNZ_OK) 
                XLI_THROW(String("Couldn't locate file in zip-archive: ") + filename);

            if (unzOpenCurrentFile((unzFile)handle) != UNZ_OK) 
                XLI_THROW(String("Couldn't open file in zip-archive: ") + filename);

            unz_file_info unzFileInfo;
            unzGetCurrentFileInfo((unzFile)handle, &unzFileInfo, 0, 0, 0, 0, 0, 0);

            Managed<Buffer> buf = Buffer::Create(unzFileInfo.uncompressed_size);

            if (unzReadCurrentFile((unzFile)handle, buf->Ptr(), buf->Size()) != buf->Size()) 
                XLI_THROW(String("Couldn't read in from zip-archive: ") + filename);
            
            if (unzCloseCurrentFile((unzFile)handle) != UNZ_OK ) 
                XLI_THROW(String("Couldn't close file in zip-archive: ") + filename);

            return new BufferStream(buf, true, false);
        }

        virtual void GetFiles(const String& path, Array<FileInfo>& list)
        {
            XLI_THROW_NOT_SUPPORTED(XLI_FUNCTION);
        }
    };

    FileSystem* Zip::Open(Stream* file, const char* password)
    {
        return new ZipFileSystem(file, password);
    }
}