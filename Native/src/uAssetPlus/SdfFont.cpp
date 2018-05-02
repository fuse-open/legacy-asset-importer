#include <Fonts/SdfFontBuilder.h>
#include <uAsset/Internal.h>
#include <uAssetPlus/SdfFont.h>

int STDCALL uCreateSdfFont(uHandle handle, uString filename, int textureSize, uString charset)
{
    U_BEGIN_FUNC;
    SdfFontBuilder b;
        
    if (textureSize != 0)
        b.TextureSize = textureSize;
        
    if (charset != 0) 
        b.CharSet = charset;
        
    obj->SdfFontPtr = b.Build(Unicode::Utf16To8(filename));
    return 0;
    U_END_FUNC;
}

int STDCALL uSdfFont_SaveTextureAsPng(uHandle handle, uString filename)
{
    U_BEGIN_FUNC;
    obj->SdfFontPtr->SaveBitmapAsPng(Unicode::Utf16To8(filename));
    return 0;
    U_END_FUNC;
}

int STDCALL uSdfFont_SaveData(uHandle handle, uString filename)
{
    U_BEGIN_FUNC;
    obj->SdfFontPtr->SaveMetricsAsPsf(Unicode::Utf16To8(filename));
    return 0;
    U_END_FUNC;
}
