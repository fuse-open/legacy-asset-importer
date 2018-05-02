using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;

namespace Uno.Native
{
    public static partial class NativeLib
    {
        public static void Initialize(string dllDir = null)
        {
            if (!IsInitialized)
            {
                Load(dllDir);
                uInitialize();

                // Initalize Plus library if available
                if (uInitializePlus != null)
                    uInitializePlus();

                IsInitialized = true;
            }
        }

        internal static bool IsInitialized
        {
            get;
            private set;
        }

        static string AssemblyDirectory
        {
            get
            {
                var codeBase = Assembly.GetExecutingAssembly().CodeBase;
                var uri = new UriBuilder(codeBase);
                var path = Uri.UnescapeDataString(uri.Path);
                return Path.GetDirectoryName(path);
            }
        }

        static void Load(string dllDir)
        {
            var dllName = PlatformDetection.IsWindows ? "uAsset.dll"
                        : PlatformDetection.IsMac ? "libuAsset.dylib"
                        : "libuAsset.so";
            var dllPlusName = PlatformDetection.IsWindows ? "uAssetPlus.dll"
                        : PlatformDetection.IsMac ? "libuAssetPlus.dylib"
                        : "libuAssetPlus.so";

            var searchPaths = GetSearchPaths(dllDir);

            foreach (var dir in searchPaths)
            {
                var filename = Path.Combine(dir, dllName);

                if (File.Exists(filename))
                {
                    LoadAssetDll(LoadLibrary(filename));

                    // Optionally load uAssetPlus.dll if it exists
                    // Other dlls than uAsset.dll can be omitted from Uno exports to save space
                    var filenamePlus = Path.Combine(dir, dllPlusName);
                    if (File.Exists(filenamePlus))
                        LoadAssetPlusDll(LoadLibrary(filenamePlus));

                    return;
                }
            }

            var sb = new StringBuilder("'" + dllName + "' was not found in any of these locations:");

            foreach (var dir in searchPaths)
                sb.AppendLine("  * " + dir);

            throw new NativeException(sb.ToString());
        }

        static string[] GetSearchPaths(string dllDir)
        {
            dllDir = Path.GetFullPath(dllDir ?? AssemblyDirectory);

            if (PlatformDetection.IsWindows)
            {
                var arch = IntPtr.Size == 8 ? "x64" : "x86";
                return new[]
                {
                    dllDir,
                    Path.Combine(dllDir, arch),
#if DEBUG
                    Path.GetFullPath(Path.Combine(AssemblyDirectory, "..", "..", "..", "..", "Native", "lib", "vs2013", arch, "Debug"))
#endif
                };
            }

            return new[]
            {
                dllDir,
#if DEBUG
                Path.GetFullPath(Path.Combine(AssemblyDirectory, "..", "..", "..", "..", "Native", "lib"))
#endif
            };
        }

        static IntPtr LoadLibrary(string filename)
        {
            if (PlatformDetection.IsWindows)
            {
                Win32.SetDllDirectory(Path.GetDirectoryName(filename));
                var result = Win32.LoadLibrary(filename);

                if (result != IntPtr.Zero)
                    return result;
                else
                    throw new NativeException("Failed to load '" + filename + "'. " + new Win32Exception(Marshal.GetLastWin32Error()).Message);
            }
            else if (PlatformDetection.IsMac)
            {
                var result = Mac.dlopen(filename, 9);

                if (result != IntPtr.Zero) 
                    return result;
            }

            throw new NativeException("Failed to load '" + filename + "'");
        }

        static T LoadEntrypoint<T>(IntPtr dll, string name) where T : class
        {
            if (!PlatformDetection.IsWindows || IntPtr.Size != 4)
                name = name.Substring(1, name.IndexOf('@') - 1);

            var address = PlatformDetection.IsWindows ? Win32.GetProcAddress(dll, name)
                        : PlatformDetection.IsMac ? Mac.dlsym(dll, name)
                        : IntPtr.Zero;
            var result = address != IntPtr.Zero 
                       ? Marshal.GetDelegateForFunctionPointer(address, typeof(T)) as T 
                       : null;

            if (result != null)
                return result;

            throw new NativeException("Failed to get address of '" + name + "'");
        }
    }
}
