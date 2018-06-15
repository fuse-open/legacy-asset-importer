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

#include <XliPlatform/PlatformSpecific/Android.h>
#include "../unix/UnixFileSystemBase.h"
#include "AJniHelper.h"

namespace Xli
{
    namespace PlatformSpecific
    {
        class AAssetStream: public Stream
        {
            AAsset* asset;

        public:
            AAssetStream(String filename, FileMode mode)
            {
                if (mode != FileModeRead && mode != FileModeReadRandom) 
                    XLI_THROW("Unsupported asset file mode: " + FileModeInfo::ToString(mode));
                                
                asset = AAssetManager_open(AShim::GetAssetManager(), filename.Ptr(), ((mode & FileModeRandom) != 0) ? AASSET_MODE_RANDOM : AASSET_MODE_STREAMING);
                //asset = 0;
                
                if (asset == 0)
                    XLI_THROW_CANT_OPEN_FILE(filename);
            }
            
            virtual ~AAssetStream()
            {
                AAsset_close(asset);
            }

            virtual bool CanRead() const
            {
                return true;
            }

            virtual bool CanSeek() const
            {
                return true;
            }

            int GetPosition() const
            {
                return (int)(AAsset_getLength(asset) - AAsset_getRemainingLength(asset));
            }

            int GetLength() const
            {
                return AAsset_getLength(asset);
            }

            int Read(void* data, int elmSize, int elmCount)
            {
                return AAsset_read(asset, data, elmSize * elmCount) / elmSize;
            }

            void Seek(int offset, SeekOrigin origin)
            {
                switch (origin)
                {
                case SeekOriginBegin:
                    AAsset_seek(asset, offset, SEEK_SET);
                    return;

                case SeekOriginCurrent:
                    AAsset_seek(asset, offset, SEEK_CUR);
                    return;

                case SeekOriginEnd:
                    AAsset_seek(asset, offset, SEEK_END);
                    return;
                }

                XLI_THROW_STREAM_CANT_SEEK;
            }
        };

        class AAssetBuffer: public DataAccessor
        {
            AAsset* asset;

        public:
            AAssetBuffer(String filename)
            {
                asset = AAssetManager_open(AShim::GetAssetManager(), filename.Ptr(), AASSET_MODE_BUFFER);
                // asset = 0;
                
                if (asset == 0) 
                    XLI_THROW_CANT_OPEN_FILE(filename);
            }

            virtual ~AAssetBuffer()
            {
                AAsset_close(asset);
            }

            virtual const UInt8* GetPtr() const
            {
                return (const UInt8*)AAsset_getBuffer(asset);
            }

            virtual int GetSizeInBytes() const
            {
                return AAsset_getLength(asset);
            }
        };

        class AAssetFileSystem: public FileSystem
        {
        public:
            virtual Stream* OpenFile(const String& filename, FileMode mode)
            {
                return new AAssetStream(filename, mode);
            }

            virtual DataAccessor* OpenFileAsBuffer(const String& filename)
            {
                return new AAssetBuffer(filename);
            }
        };

        class AFileSystem: public UnixFileSystemBase
        {
            jmethodID Context_getCacheDir;
            jmethodID File_getAbsolutePath;

        public:
            AFileSystem()
            {
                AJniHelper jni;
                Context_getCacheDir = jni.FindMethod("android/content/Context", "getCacheDir", "()Ljava/io/File;");
                File_getAbsolutePath = jni.FindMethod("java/io/File", "getAbsolutePath", "()Ljava/lang/String;");
            }

            virtual String GetBaseDirectory()
            {
                return "";
            }

            virtual String GetTempDirectory()
            {
                AJniHelper jni;
                jobject cacheDir = jni->CallObjectMethod(AJniHelper::GetActivity(), Context_getCacheDir);
                jobject absolutePath = jni->CallObjectMethod(cacheDir, File_getAbsolutePath);
                return jni.GetString(absolutePath);
            }

            virtual String GetSystemDirectory(SystemDirectory dir)
            {
                switch (dir)
                {
                case SystemDirectoryConfig:
                    return "";//(String)AndroidActivity->externalDataPath;

                case SystemDirectoryData:
                    return "";//(String)AndroidActivity->internalDataPath;

                // TODO: Conform to Android specifications on proper handling of system directories
                default:
                    return "/sdcard";
                }
            }
        };
    }

    FileSystem* CreateBundleFileSystem()
    {
        return new PlatformSpecific::AAssetFileSystem();
    }

    NativeFileSystem* CreateNativeFileSystem()
    {
        return new PlatformSpecific::AFileSystem();
    }
}
