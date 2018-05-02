#ifndef __UASSETS_SPRITE_FONT_H__
#define __UASSETS_SPRITE_FONT_H__

#include <uAsset/Base.h>

/**
    \addtogroup uSpriteFont
    @{
*/

EXPORT int STDCALL uCreateSpriteFont(uHandle handle, uString filename, int fontSize, int pow2, uString charset);

EXPORT int STDCALL uSpriteFont_SaveTextureAsPng(uHandle handle, uString filename);
EXPORT int STDCALL uSpriteFont_SaveData(uHandle handle, uString filename);

/** @} */

#endif
