using System;
using System.IO;
using System.Runtime.InteropServices;

namespace Uno.Native
{
    public static class GZip
    {
        public static void Compress(string srcFilename, string dstFilename, int level = 6)
        {
            IOHelper.CreateContainingDirectory(dstFilename);
            using (var p = new NativeObject()) p.CheckError(NativeLib.uGZip_Compress(p.Handle, srcFilename, dstFilename, level));
        }

        public static void Decompress(string srcFilename, string dstFilename)
        {
            IOHelper.CreateContainingDirectory(dstFilename);
            using (var p = new NativeObject()) p.CheckError(NativeLib.uGZip_Decompress(p.Handle, srcFilename, dstFilename));
        }
    }
}
