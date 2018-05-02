#include <XliImage/Png.h>
#include <XliImage/Jpeg.h>
#include <XliImage/Ktx.h>
#include <Xli/BufferStream.h>
#include <Xli/Path.h>
#include <uAsset/Internal.h>
#include <uAsset/Texture.h>
#include <cstring>

static void uTexture_SetInfoInternal(uObject* obj, uTextureInfo* result)
{
    result->Width = obj->TexturePtr->GetWidth();
    result->Height = obj->TexturePtr->GetHeight();
    result->Depth = obj->TexturePtr->GetDepth();
    result->FaceCount = obj->TexturePtr->Faces.Length();
    result->MipCount = obj->TexturePtr->GetMipCount();
    result->FileFormat = obj->TextureFileFormat;
    result->TextureType = obj->TexturePtr->Type;
    result->PixelFormat = obj->TexturePtr->GetFormat();
}

static void uTexture_LoadInternal(uObject* obj, uString filename, Xli::Stream* stream)
{
    String utf8 = Unicode::Utf16To8(filename);
    String ext = Path::GetExtension(utf8).ToLower();

    if (ext == ".jpeg" || ext == ".jpg")
    {
        obj->TextureFileFormat = uTextureFileFormatJpeg;

        Xli::Managed<Image> image = stream != NULL ? Jpeg::Load(stream) : Jpeg::Load(utf8);
        obj->TexturePtr = Texture::Create(image);
    }
    else if (ext == ".png")
    {
        obj->TextureFileFormat = uTextureFileFormatPng;

        Xli::Managed<Image> image = stream != NULL ? Png::Load(stream) : Png::Load(utf8);
        obj->TexturePtr = Texture::Create(image);
    }
    else
    {
        XLI_THROW("Unsupported texture extension: " + ext);
    }
}

int STDCALL uLoadTexture(uHandle handle, uString filename, uTextureInfo* result)
{
    U_BEGIN_FUNC;
    uTexture_LoadInternal(obj, filename, NULL);
    uTexture_SetInfoInternal(obj, result);
    return 0;
    U_END_FUNC;
}

int STDCALL uLoadTextureFromMemory(uHandle handle, uString filename, void* data, int size, uTextureInfo* result)
{
    U_BEGIN_FUNC;
    BufferStream stream(new BufferPtr(data, size, false), true, false);
    uTexture_LoadInternal(obj, filename, &stream);
    uTexture_SetInfoInternal(obj, result);
    return 0;
    U_END_FUNC;
}

int STDCALL uTexture_Convert2DToCube(uHandle handle, uTextureInfo* result)
{
    U_BEGIN_FUNC;
    obj->TexturePtr = obj->TexturePtr->Convert2DToCube();
    uTexture_SetInfoInternal(obj, result);
    return 0;
    U_END_FUNC;
}

int STDCALL uTexture_ConvertCubeTo2D(uHandle handle, int cube2DMode, uTextureInfo* result)
{
    U_BEGIN_FUNC;
    obj->TexturePtr = obj->TexturePtr->ConvertCubeTo2D((Texture::Cube2DMode)cube2DMode);
    uTexture_SetInfoInternal(obj, result);
    return 0;
    U_END_FUNC;
}

static Image* uTexture_GetImageInternal(uObject* obj, int faceIndex, int mipIndex)
{
    return obj->TexturePtr->Faces[faceIndex].MipLevels[mipIndex];
}

int STDCALL uTexture_GetDataSize(uHandle handle, int faceIndex, int mipIndex, int* result)
{
    U_BEGIN_FUNC;
    *result = uTexture_GetImageInternal(obj, faceIndex, mipIndex)->GetSizeInBytes();
    return 0;
    U_END_FUNC;
}

int STDCALL uTexture_ReadData(uHandle handle, int faceIndex, int mipIndex, void* result, int byteCount)
{
    U_BEGIN_FUNC;
    Image* image = uTexture_GetImageInternal(obj, faceIndex, mipIndex);
    if (image->GetSizeInBytes() != byteCount) XLI_THROW_INDEX_OUT_OF_BOUNDS;
    memcpy(result, image->GetPtr(), image->GetSizeInBytes());
    return 0;
    U_END_FUNC;
}

int STDCALL uTexture_SaveAsPng(uHandle handle, uString dstFilename, int faceIndex, int mipIndex)
{
    U_BEGIN_FUNC;
    Image* image = uTexture_GetImageInternal(obj, faceIndex, mipIndex);
    Managed<Bitmap> bmp = image->ToBitmap();
    Png::Save(Unicode::Utf16To8(dstFilename), bmp);
    return 0;
    U_END_FUNC;
}

int STDCALL uTexture_Compress(uHandle handle, uString dstFilename, int format, int generateMipMaps)
{
    U_BEGIN_FUNC;
    obj->SetErrorMessage("Texture compression is disabled");
    return 0;
    U_END_FUNC;
}
