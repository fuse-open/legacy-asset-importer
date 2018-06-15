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
#include "AndroidJni.h"

namespace Xli
{
    namespace PlatformSpecific
    {
        class AndroidFileSystem: public UnixFileSystemBase
        {
            //jmethodID Context_getCacheDir;
            //jmethodID File_getAbsolutePath;

        public:
            AndroidFileSystem()
            {
                //AndroidJni jni;
                //Context_getCacheDir = jni.FindMethod("android/content/Context", "getCacheDir", "()Ljava/io/File;");
                //File_getAbsolutePath = jni.FindMethod("java/io/File", "getAbsolutePath", "()Ljava/lang/String;");
            }

            virtual String GetBaseDirectory()
            {
                return "/sdcard";
            }

            virtual String GetTempDirectory()
            {
                //AndroidJni jni;
                //jobject cacheDir = jni->CallObjectMethod(AndroidJni::GetActivity(), Context_getCacheDir);
                //jobject absolutePath = jni->CallObjectMethod(cacheDir, File_getAbsolutePath);
                //return jni.GetString(absolutePath);
                return "/sdcard";
            }

            virtual String GetSystemDirectory(SystemDirectory dir)
            {
                // TODO: Conform to Android specifications on proper handling of system directories
                return "/sdcard";
            }
        };
    }

    NativeFileSystem* CreateDiskFileSystem()
    {
        return new PlatformSpecific::AndroidFileSystem();
    }
}
