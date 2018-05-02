using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;

namespace Uno.Native.Fonts
{
    public class SdfFont : NativeObject
    {
        public string Filename 
        { 
            get; 
            private set; 
        }
        
        public int TextureSize 
        { 
            get; 
            private set; 
        }
        
        public string CharSet 
        { 
            get; 
            private set; 
        }

        public SdfFont(string filename, int textureSize = 0, string charset = null)
        {
            CheckError(NativeLib.uCreateSdfFont(Handle, filename, textureSize, charset));
            Filename = filename;
            TextureSize = textureSize;
            CharSet = charset;
        }

        public void SaveTextureAsPng(string filename)
        {
            IOHelper.CreateContainingDirectory(filename);
            CheckError(NativeLib.uSdfFont_SaveTextureAsPng(Handle, filename));
        }

        public void SaveData(string filename)
        {
            IOHelper.CreateContainingDirectory(filename);
            CheckError(NativeLib.uSdfFont_SaveData(Handle, filename));
        }
    }
}
