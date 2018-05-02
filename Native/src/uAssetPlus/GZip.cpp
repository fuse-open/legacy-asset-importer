#include <uAsset/Internal.h>
#include <uAssetPlus/GZip.h>
#include <XliZip/GZip.h>

int STDCALL uGZip_Compress(uHandle handle, uString srcFilename, uString dstFilename, int level)
{
    U_BEGIN_FUNC;
    File src(Unicode::Utf16To8(srcFilename), FileModeRead);
    File dst(Unicode::Utf16To8(dstFilename), FileModeWrite);

    Managed<Stream> gz = GZip::CreateWriter(&dst, level);
    gz->WriteStream(&src);

    return 0;
    U_END_FUNC;
}

int STDCALL uGZip_Decompress(uHandle handle, uString srcFilename, uString dstFilename)
{
    U_BEGIN_FUNC;
    File src(Unicode::Utf16To8(srcFilename), FileModeRead);
    File dst(Unicode::Utf16To8(dstFilename), FileModeWrite);

    Managed<Stream> gz = GZip::CreateReader(&src);
    dst.WriteStream(gz);

    return 0;
    U_END_FUNC;
}
