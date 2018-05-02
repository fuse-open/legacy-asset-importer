#ifndef __UASSETS_GZIP_H__
#define __UASSETS_GZIP_H__

#include <uAsset/Base.h>

/**
    \addtogroup uTexture
    @{
*/

EXPORT int STDCALL uGZip_Compress(uHandle handle, uString srcFilename, uString dstFilename, int level);
EXPORT int STDCALL uGZip_Decompress(uHandle handle, uString srcFilename, uString dstFilename);

/** @} */

#endif
