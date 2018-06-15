#import <Foundation/Foundation.h>
#include <Xli/Path.h>
#include "../unix/UnixFileSystemBase.h"

namespace Xli
{
    namespace PlatformSpecific
    {
        class CocoaFileSystem: public UnixFileSystemBase
        {
        public:
            virtual Stream* Open(const String& filename, FileMode mode)
            {
                if (mode & FileModeRead)
                {
                    File* result = 0;
                    
                    @autoreleasepool
                    {
                        NSFileManager* file_manager = [NSFileManager defaultManager];
                        NSString* resource_path = [[NSBundle mainBundle] resourcePath];
                        
                        NSString* ns_string_file_component = [file_manager stringWithFileSystemRepresentation:filename.Ptr() length:filename.Length()];
                        
                        NSString* full_path_with_file_to_try = [resource_path stringByAppendingPathComponent:ns_string_file_component];
                        
                        if ([file_manager fileExistsAtPath:full_path_with_file_to_try])
                            result = new File([full_path_with_file_to_try fileSystemRepresentation], mode);
                    }
                    
                    if (result)
                        return result;
                }
            
                return new File(filename, mode);
            }
            
            // TODO: Should return shared dir accessible from global filesystem
            virtual String GetSystemDirectory(SystemDirectory dir)
            {
                NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
                NSString* documentsDirectory = [paths objectAtIndex:0];
                return [documentsDirectory UTF8String];
            }
            
            virtual String GetBaseDirectory()
            {
                NSArray* arguments = [[NSProcessInfo processInfo] arguments];
                NSString* exe = [arguments objectAtIndex:0];
                return Path::GetDirectoryName([exe UTF8String]);
            }

            virtual String GetTempDirectory()
            {
                return [NSTemporaryDirectory() UTF8String];
            }
        };
    }

    NativeFileSystem* CreateNativeFileSystem()
    {
        return new PlatformSpecific::CocoaFileSystem();
    }
}
