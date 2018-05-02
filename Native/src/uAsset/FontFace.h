#ifndef __UASSETS_FONT_FACE_H__
#define __UASSETS_FONT_FACE_H__

#include <uAsset/Base.h>

/**
    \ingroup uFontFace
*/
struct uFontFaceInfo
{
    uString FamilyName;
    uString StyleName;
};

/**
    \ingroup uFontFace
*/
struct uFontLineMetricInfo
{
    float Ascender;
    float Descender;
    float LineHeight;
};

/**
    \ingroup uFontFace
*/
struct uFontKerningInfo
{
    float KerningX;
    float KerningY;
};

/**
    \ingroup uFontFace
*/
enum uFontRenderMode
{
    uFontRenderModeNone = 0,
    uFontRenderModeNormal = 1,
    uFontRenderModeMonochrome = 2,
    uFontRenderModeForceInt = 0x7fffffff,
};

/**
    \ingroup uFontFace
*/
struct uFontGlyphInfo
{
    float AdvanceX;
    float AdvanceY;
    float BearingX;
    float BearingY;
    int BitmapWidth;
    int BitmapHeight;
    int PixelFormat;
};

/**
    \addtogroup uFontFace
    @{
*/

EXPORT int STDCALL uLoadFontFace(uHandle handle, uString filename, uFontFaceInfo* result);
EXPORT int STDCALL uLoadFontFaceFromMemory(uHandle handle, void* data, int size, uFontFaceInfo* result);

EXPORT int STDCALL uFontFace_GetLineMetrics(uHandle handle, float pixelSize, uFontLineMetricInfo* result);
EXPORT int STDCALL uFontFace_GetKerning(uHandle handle, float pixelSize, unsigned int left, unsigned int right, uFontKerningInfo* result);

EXPORT int STDCALL uFontFace_ContainsGlyph(uHandle handle, float pixelSize, unsigned int glyph, int* result);
EXPORT int STDCALL uFontFace_RenderGlyph(uHandle handle, float pixelSize, unsigned int glyph, uFontRenderMode mode, uFontGlyphInfo* result);
EXPORT int STDCALL uFontFace_ReadGlyphBitmap(uHandle handle, void* result, int byteCount);

/** @} */

#endif
