#include <uAsset/Internal.h>
#include <uAsset/FontFace.h>
#include <Xli/BufferStream.h>
#include <cstring>

int STDCALL uLoadFontFace(uHandle handle, uString filename, uFontFaceInfo* result)
{
    U_BEGIN_FUNC;
    obj->FontFacePtr = FreeType::LoadFontFace(Unicode::Utf16To8(filename));
    result->FamilyName = obj->GetStringHandle(obj->FontFacePtr->GetFamilyName());
    result->StyleName = obj->GetStringHandle(obj->FontFacePtr->GetStyleName());
    return 0;
    U_END_FUNC;
}

int STDCALL uLoadFontFaceFromMemory(uHandle handle, void* data, int size, uFontFaceInfo* result)
{
    U_BEGIN_FUNC;
    BufferStream stream(new BufferPtr(data, size, false), true, false);
    obj->FontFacePtr = FreeType::LoadFontFace(&stream);
    result->FamilyName = obj->GetStringHandle(obj->FontFacePtr->GetFamilyName());
    result->StyleName = obj->GetStringHandle(obj->FontFacePtr->GetStyleName());
    return 0;
    U_END_FUNC;
}

int STDCALL uFontFace_GetLineMetrics(uHandle handle, float pixelSize, uFontLineMetricInfo* result)
{
    U_BEGIN_FUNC;
    result->Ascender = obj->FontFacePtr->GetAscender(pixelSize);
    result->Descender = obj->FontFacePtr->GetDescender(pixelSize);
    result->LineHeight = obj->FontFacePtr->GetLineHeight(pixelSize);
    return 0;
    U_END_FUNC;
}

int STDCALL uFontFace_GetKerning(uHandle handle, float pixelSize, unsigned int left, unsigned int right, uFontKerningInfo* result)
{
    U_BEGIN_FUNC;
    obj->FontFacePtr->TryGetKerning(pixelSize, left, right, (Vector2*)result);
    return 0;    
    U_END_FUNC;
}

int STDCALL uFontFace_ContainsGlyph(uHandle handle, float pixelSize, unsigned int glyph, int* result)
{
    U_BEGIN_FUNC;
    *result = obj->FontFacePtr->ContainsGlyph(pixelSize, glyph) ? 1 : 0;
    return 0;
    U_END_FUNC;
}

int STDCALL uFontFace_RenderGlyph(uHandle handle, float pixelSize, unsigned int glyph, uFontRenderMode mode, uFontGlyphInfo* result)
{
    U_BEGIN_FUNC;
    obj->BitmapPtr = obj->FontFacePtr->RenderGlyph(pixelSize, glyph, (FontRenderMode)mode, (Vector2*)&result->AdvanceX, (Vector2*)&result->BearingX);
    result->BitmapWidth = obj->BitmapPtr->GetWidth();
    result->BitmapHeight = obj->BitmapPtr->GetHeight();
    result->PixelFormat = obj->BitmapPtr->GetFormat();
    return 0;
    U_END_FUNC;
}

int STDCALL uFontFace_ReadGlyphBitmap(uHandle handle, void* result, int byteCount)
{
    U_BEGIN_FUNC;
    int size = obj->BitmapPtr->GetBytesPerPixel() * obj->BitmapPtr->GetWidth() * obj->BitmapPtr->GetHeight();

    if (byteCount != size)
    {
        obj->SetErrorMessage("Invalid buffer size");
        return -1;
    }

    memcpy(result, obj->BitmapPtr->GetPtr(), size);
    return 0;
    U_END_FUNC;
}
