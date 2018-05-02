#ifndef __UASSETS_TEXTURE_H__
#define __UASSETS_TEXTURE_H__

#include <uAsset/Base.h>

enum uTextureFileFormat
{
    uTextureFileFormatUnknown,
    uTextureFileFormatPng,
    uTextureFileFormatJpeg,
    uTextureFileFormatKtx,
};

/**
    \ingroup uTexture
*/
struct uTextureInfo
{
    int Width;
    int Height;
    int Depth;
    int FaceCount;
    int MipCount;
    int FileFormat;
    int TextureType;
    int PixelFormat;
};

/**
    \addtogroup uTexture
    @{
*/

EXPORT int STDCALL uLoadTexture(uHandle handle, uString filename, uTextureInfo* result);
EXPORT int STDCALL uLoadTextureFromMemory(uHandle handle, uString filename, void* data, int size, uTextureInfo* result);

EXPORT int STDCALL uTexture_Convert2DToCube(uHandle handle, uTextureInfo* result);
EXPORT int STDCALL uTexture_ConvertCubeTo2D(uHandle handle, int cube2DMode, uTextureInfo* result);

EXPORT int STDCALL uTexture_GetDataSize(uHandle handle, int faceIndex, int mipIndex, int* result);
EXPORT int STDCALL uTexture_ReadData(uHandle handle, int faceIndex, int mipIndex, void* result, int byteCount);

EXPORT int STDCALL uTexture_SaveAsPng(uHandle handle, uString dstFilename, int faceIndex, int mipIndex);
EXPORT int STDCALL uTexture_Compress(uHandle handle, uString dstFilename, int format, int generateMipMaps);

/** @} */

#endif
