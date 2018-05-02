using System;
using System.Reflection;
using System.Runtime.InteropServices;
using Uno.Native.Fonts;
using Uno.Native.Textures;

namespace Uno.Native
{
    // Generated using /Source/gen.sh
    public static partial class NativeLib
    {
        static void LoadAssetPlusDll(IntPtr dll)
        {
            uInitializePlus = LoadEntrypoint<uInitializePlus_>(dll, "_uInitializePlus@0");
            uGZip_Compress = LoadEntrypoint<uGZip_Compress_>(dll, "_uGZip_Compress@16");
            uGZip_Decompress = LoadEntrypoint<uGZip_Decompress_>(dll, "_uGZip_Decompress@12");
            uCreateSdfFont = LoadEntrypoint<uCreateSdfFont_>(dll, "_uCreateSdfFont@16");
            uSdfFont_SaveTextureAsPng = LoadEntrypoint<uSdfFont_SaveTextureAsPng_>(dll, "_uSdfFont_SaveTextureAsPng@8");
            uSdfFont_SaveData = LoadEntrypoint<uSdfFont_SaveData_>(dll, "_uSdfFont_SaveData@8");
            uCreateSpriteFont = LoadEntrypoint<uCreateSpriteFont_>(dll, "_uCreateSpriteFont@20");
            uSpriteFont_SaveTextureAsPng = LoadEntrypoint<uSpriteFont_SaveTextureAsPng_>(dll, "_uSpriteFont_SaveTextureAsPng@8");
            uSpriteFont_SaveData = LoadEntrypoint<uSpriteFont_SaveData_>(dll, "_uSpriteFont_SaveData@8");
        }

        internal delegate void uInitializePlus_();
        internal static uInitializePlus_ uInitializePlus;

        internal delegate int uGZip_Compress_(IntPtr handle, [MarshalAs(UnmanagedType.LPWStr)] string srcFilename, [MarshalAs(UnmanagedType.LPWStr)] string dstFilename, int level);
        internal static uGZip_Compress_ uGZip_Compress;

        internal delegate int uGZip_Decompress_(IntPtr handle, [MarshalAs(UnmanagedType.LPWStr)] string srcFilename, [MarshalAs(UnmanagedType.LPWStr)] string dstFilename);
        internal static uGZip_Decompress_ uGZip_Decompress;

        internal delegate int uCreateSdfFont_(IntPtr handle, [MarshalAs(UnmanagedType.LPWStr)] string filename, int textureSize, [MarshalAs(UnmanagedType.LPWStr)] string charset);
        internal static uCreateSdfFont_ uCreateSdfFont;

        internal delegate int uSdfFont_SaveTextureAsPng_(IntPtr handle, [MarshalAs(UnmanagedType.LPWStr)] string filename);
        internal static uSdfFont_SaveTextureAsPng_ uSdfFont_SaveTextureAsPng;

        internal delegate int uSdfFont_SaveData_(IntPtr handle, [MarshalAs(UnmanagedType.LPWStr)] string filename);
        internal static uSdfFont_SaveData_ uSdfFont_SaveData;

        internal delegate int uCreateSpriteFont_(IntPtr handle, [MarshalAs(UnmanagedType.LPWStr)] string filename, int fontSize, [MarshalAs(UnmanagedType.Bool)] bool pow2, [MarshalAs(UnmanagedType.LPWStr)] string charset);
        internal static uCreateSpriteFont_ uCreateSpriteFont;

        internal delegate int uSpriteFont_SaveTextureAsPng_(IntPtr handle, [MarshalAs(UnmanagedType.LPWStr)] string filename);
        internal static uSpriteFont_SaveTextureAsPng_ uSpriteFont_SaveTextureAsPng;

        internal delegate int uSpriteFont_SaveData_(IntPtr handle, [MarshalAs(UnmanagedType.LPWStr)] string filename);
        internal static uSpriteFont_SaveData_ uSpriteFont_SaveData;
    }
}
