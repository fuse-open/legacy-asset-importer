using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;

namespace Uno.Native.Fonts
{
    public class SpriteFont : NativeObject
    {
        public string Filename 
        { 
            get; 
            private set; 
        }
        
        public int FontSize
        { 
            get; 
            private set; 
        }

        public bool Pow2
        {
            get;
            private set;
        }

        public string CharSet 
        { 
            get; 
            private set; 
        }

        public SpriteFont(string filename, int fontSize, bool pow2 = false, string charset = null)
        {
            CheckError(NativeLib.uCreateSpriteFont(Handle, filename, fontSize, pow2, charset));
            Filename = filename;
            FontSize = fontSize;
            Pow2 = pow2;
            CharSet = charset;
        }

        public void SaveTextureAsPng(string filename)
        {
            IOHelper.CreateContainingDirectory(filename);
            CheckError(NativeLib.uSpriteFont_SaveTextureAsPng(Handle, filename));
        }

        public void SaveData(string filename)
        {
            IOHelper.CreateContainingDirectory(filename);
            CheckError(NativeLib.uSpriteFont_SaveData(Handle, filename));
        }
    }
}
