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

void TestFont(const String& filename)
{
    PrintLine("Loading font: " + filename);

    PHandle handle = PCreate();
    Utf16String filenameW = Unicode::Utf8To16(filename);
    Utf16String charsetW = Unicode::Utf8To16("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZÆØÅ[\\]^_`abcdefghijklmnopqrstuvwxyzæøå¨{|}~°");

    PrintLine("Creating SDF font");
    CheckError(handle, PCreateSdfFont(handle, filenameW.Ptr(), 1024, charsetW.Ptr()));
    CheckError(handle, PSdfFont_SaveTextureAsPng(handle, Unicode::Utf8To16(filename + "-sdf.png").Ptr()));
    CheckError(handle, PSdfFont_SaveData(handle, Unicode::Utf8To16(filename + "-sdf.pbf").Ptr()));

    PrintLine("Creating sprite font");
    CheckError(handle, PCreateSpriteFont(handle, filenameW.Ptr(), 24, 0, charsetW.Ptr()));
    CheckError(handle, PSpriteFont_SaveTextureAsPng(handle, Unicode::Utf8To16(filename + "-sprite.png").Ptr()));
    CheckError(handle, PSpriteFont_SaveData(handle, Unicode::Utf8To16(filename + "-sprite.pbf").Ptr()));

    PRelease(handle);
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
