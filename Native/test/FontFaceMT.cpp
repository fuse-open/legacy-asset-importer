#include <Xli.h>
#include <Punch.h>

using namespace Xli;

void CheckError(PHandle handle, int code)
{
  if (code != 0)
  {
    PString err = PGetErrorString(handle);
    XLI_THROW(Unicode::Utf16To8(err));
  }
}

class FontFaceTask : public Xli::ThreadTask
{
  String filename;
public:
  FontFaceTask(const String& filename)
  {
    this->filename = filename;
  }

  virtual void Run()
  {
    PrintLine("Loading font: " + filename);

    PHandle handle = PCreate();
    Utf16String filenameW = Unicode::Utf8To16(filename);
    Utf16String charsetW = Unicode::Utf8To16("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZÆØÅ[\\]^_`abcdefghijklmnopqrstuvwxyzæøå¨{|}~°");

    PFontFaceInfo font;
    PLoadFontFace(handle, filenameW.Ptr(), &font);

    for (int i = 0; i < charsetW.Length(); i++)
    {
      PFontGlyphInfo glyph;
      PFontFace_RenderGlyph(handle, 16, charsetW[i], PFontRenderModeNormal, &glyph);
    }

    PRelease(handle);
  }
};

void TestFont(const String& filename)
{
  Thread t0(new FontFaceTask(filename));
  Thread t1(new FontFaceTask(filename));
  Thread t2(new FontFaceTask(filename));
  Thread t3(new FontFaceTask(filename));
}

void TestFile(const String& filename)
{
  String ext = Path::GetExtension(filename).ToUpper();

  if (ext == ".TTF" || ext == ".OTF")
    TestFont(filename);
  }

  void TestDirectory(const String& dirname)
{
  Array<FileInfo> files;
  Disk->GetFiles(dirname, files);

  for (int i = 0; i < files.Length(); i++)
    TestFile(files[i].Name);
  }

  int main(int argc, const char* argv[])
{
    if (argc > 1)
    {
        String filter = argv[1];

        if (Disk->Exists(filter))
        {
            PInitialize();

            if (Disk->IsDirectory(filter))
                TestDirectory(filter);
                else
                    TestFile(filter);

                    return 0;
                }
            }

            XLI_THROW("File or directory does not exist");
            return 0;
        }
