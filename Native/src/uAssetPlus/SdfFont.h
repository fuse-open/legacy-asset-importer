#ifndef __UASSETS_SDF_FONT_H__
#define __UASSETS_SDF_FONT_H__

#include <uAsset/Base.h>

/**
    \addtogroup uSdfFont
    @{
*/

EXPORT int STDCALL uCreateSdfFont(uHandle handle, uString filename, int textureSize, uString charset);

EXPORT int STDCALL uSdfFont_SaveTextureAsPng(uHandle handle, uString filename);
EXPORT int STDCALL uSdfFont_SaveData(uHandle handle, uString filename);

/** @} */

#endif
