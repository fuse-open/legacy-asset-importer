using System;
using System.Diagnostics;
using System.IO;
using Uno.Native.Fonts;
using Uno.Native.Textures;

namespace Uno.Native.Tests
{
    class Program
    {
        static readonly string DataPath = Path.Combine("..", "..", "..", "..", "Native", "data");

        static void LoadFontFace(string filename, float size)
        {
            Console.WriteLine("--------------------------------------------------------------------");
            Console.WriteLine("Loading Font Face: '" + filename + "', size = " + size);
            Console.WriteLine("--------------------------------------------------------------------");

            using (var font = new FontFace(filename))
            {
                Console.WriteLine("FamilyName: " + font.FamilyName);
                Console.WriteLine("StyleName: " + font.StyleName);

                Console.WriteLine("Ascender: " + font.GetAscender(size));
                Console.WriteLine("Descender: " + font.GetDescender(size));
                Console.WriteLine("LineHeight: " + font.GetLineHeight(size));                

                for (char c = ' '; c < 127; c++)
                {
                    if (font.ContainsGlyph(size, c))
                    {
                        var g = font.RenderGlyph(size, c, FontRenderMode.Normal);
                        Console.WriteLine((int)c + " (" + c + "): " + g.AdvanceX + ", " + g.AdvanceY + "; " + g.BearingX + ", " + g.BearingY + "; " + g.Width + "x" + g.Height);
                    }
                }
            }

            using (var s = File.OpenRead(filename))
                using (var font = new FontFace(s))
                    ;
        }

        static void LoadTexture(string filename)
        {
            Console.WriteLine("--------------------------------------------------------------------");
            Console.WriteLine("Loading Texture: '" + filename + "'");
            Console.WriteLine("--------------------------------------------------------------------");

			var sw = new Stopwatch();
			sw.Start();

            using (var tex = new Texture(filename))
            {
                Console.WriteLine("Depth: " + tex.Depth);
                Console.WriteLine("FaceCount: " + tex.FaceCount);
                Console.WriteLine("FileFormat: " + tex.FileFormat);
                Console.WriteLine("Filename: " + tex.Filename);
                Console.WriteLine("Height: " + tex.Height);
                Console.WriteLine("MipCount: " + tex.MipCount);
                Console.WriteLine("PixelFormat: " + tex.PixelFormat);
                Console.WriteLine("TextureType: " + tex.TextureType);
                Console.WriteLine("Width: " + tex.Width);
                
                tex.SaveAsPng("foo.png");
            }

			sw.Stop();
			Console.WriteLine("Time used loading: " + sw.ElapsedMilliseconds);
        }

        static void LoadTextureFromStream(string filename)
        {
            Console.WriteLine("--------------------------------------------------------------------");
            Console.WriteLine("Loading Texture: '" + filename + "'");
            Console.WriteLine("--------------------------------------------------------------------");

            var sw = new Stopwatch();
            sw.Start();

            using (var stream = File.OpenRead(filename))
            {
                using (var tex = new Texture(filename, stream))
                {
                    Console.WriteLine("Depth: " + tex.Depth);
                    Console.WriteLine("FaceCount: " + tex.FaceCount);
                    Console.WriteLine("FileFormat: " + tex.FileFormat);
                    Console.WriteLine("Filename: " + tex.Filename);
                    Console.WriteLine("Height: " + tex.Height);
                    Console.WriteLine("MipCount: " + tex.MipCount);
                    Console.WriteLine("PixelFormat: " + tex.PixelFormat);
                    Console.WriteLine("TextureType: " + tex.TextureType);
                    Console.WriteLine("Width: " + tex.Width);
                    
                    tex.SaveAsPng("foo.png");
                }
            }

            sw.Stop();
            Console.WriteLine("Time used loading: " + sw.ElapsedMilliseconds);
        }

        static void LoadTextureFromStreamWithFakeFilename(string filename, string fakeFilename)
        {
            Console.WriteLine("--------------------------------------------------------------------");
            Console.WriteLine("Loading Texture: '" + filename + "'");
            Console.WriteLine("--------------------------------------------------------------------");

            var sw = new Stopwatch();
            sw.Start();

            using (var stream = File.OpenRead(filename))
            {
                using (var tex = new Texture(fakeFilename, stream))
                {
                    Console.WriteLine("Depth: " + tex.Depth);
                    Console.WriteLine("FaceCount: " + tex.FaceCount);
                    Console.WriteLine("FileFormat: " + tex.FileFormat);
                    Console.WriteLine("Filename: " + tex.Filename);
                    Console.WriteLine("Height: " + tex.Height);
                    Console.WriteLine("MipCount: " + tex.MipCount);
                    Console.WriteLine("PixelFormat: " + tex.PixelFormat);
                    Console.WriteLine("TextureType: " + tex.TextureType);
                    Console.WriteLine("Width: " + tex.Width);

                    tex.SaveAsPng("foo.png");
                }
            }

            sw.Stop();
            Console.WriteLine("Time used loading: " + sw.ElapsedMilliseconds);
        }

        static void LoadTextureFromTruncatedStream(string filename)
        {
            Console.WriteLine("--------------------------------------------------------------------");
            Console.WriteLine("Loading Texture: '" + filename + "'");
            Console.WriteLine("--------------------------------------------------------------------");

            var sw = new Stopwatch();
            sw.Start();

            using (var fileStream = File.OpenRead(filename))
            {
                byte[] buffer = new byte[fileStream.Length / 2];
                int read = fileStream.Read(buffer, 0, buffer.Length);
                if (read != buffer.Length)
                    throw new Exception("Read failed!");

                using (MemoryStream stream = new MemoryStream(buffer))
                {
                    using (var tex = new Texture (filename, stream))
                    {
                        Console.WriteLine ("Depth: " + tex.Depth);
                        Console.WriteLine ("FaceCount: " + tex.FaceCount);
                        Console.WriteLine ("FileFormat: " + tex.FileFormat);
                        Console.WriteLine ("Filename: " + tex.Filename);
                        Console.WriteLine ("Height: " + tex.Height);
                        Console.WriteLine ("MipCount: " + tex.MipCount);
                        Console.WriteLine ("PixelFormat: " + tex.PixelFormat);
                        Console.WriteLine ("TextureType: " + tex.TextureType);
                        Console.WriteLine ("Width: " + tex.Width);

                        tex.SaveAsPng ("foo.png");
                    }
                }
            }

            sw.Stop();
            Console.WriteLine("Time used loading: " + sw.ElapsedMilliseconds);
        }

        static void Main(string[] args)
        {
            NativeLib.Initialize();

            try
            {
                LoadTextureFromStreamWithFakeFilename(Path.Combine(DataPath, "textures", "foo.jpg"), "fake.png");
                throw new Exception("Expected exception!");
            }
            catch (NativeException e)
            {
                Console.WriteLine("Got expected exception: " + e.ToString ());
            }

            try
            {
                LoadTextureFromStreamWithFakeFilename(Path.Combine(DataPath, "textures", "like_button.png"), "fake.jpg");
                throw new Exception("Expected exception!");
            }
            catch (NativeException e)
            {
                Console.WriteLine("Got expected exception: " + e.ToString ());
            }

            try
            {
                LoadTextureFromTruncatedStream(Path.Combine(DataPath, "textures", "like_button.png"));
                throw new Exception("Expected exception!");
            }
            catch (NativeException e)
            {
                Console.WriteLine("Got expected exception: " + e.ToString ());
            }

            try
            {
                LoadTextureFromTruncatedStream(Path.Combine(DataPath, "textures", "foo.jpg"));
                throw new Exception("Expected exception!");
            }
            catch (NativeException e)
            {
                Console.WriteLine("Got expected exception: " + e.ToString ());
            }

            LoadFontFace(Path.Combine(DataPath, "fonts", "arial.ttf"), 100);
            LoadTexture(Path.Combine(DataPath, "textures", "foo.jpg"));			
            LoadTextureFromStream(Path.Combine(DataPath, "textures", "foo.jpg"));
            LoadTexture(Path.Combine(DataPath, "textures", "like_button.png"));
            LoadTextureFromStream(Path.Combine(DataPath, "textures", "like_button.png"));
        }
    }
}
