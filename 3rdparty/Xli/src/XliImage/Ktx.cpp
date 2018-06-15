#include <XliImage/Ktx.h>
#include <XliImage/CompressedImage.h>
#include <cstring>

namespace Xli
{
    struct KTX_header
    {
        UInt8 identifier[12];
        UInt32 endianness;
        UInt32 glType;
        UInt32 glTypeSize;
        UInt32 glFormat;
        UInt32 glInternalFormat;
        UInt32 glBaseInternalFormat;
        UInt32 pixelWidth;
        UInt32 pixelHeight;
        UInt32 pixelDepth;
        UInt32 numberOfArrayElements;
        UInt32 numberOfFaces;
        UInt32 numberOfMipmapLevels;
        UInt32 bytesOfKeyValueData;
    };

    static const UInt32 GL_R = 0x1903, GL_RG = 0x8227, GL_RGB = 0x1907, GL_RGBA = 0x1908;
    static const UInt32 GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG = 0x8C00;
    static const UInt32 GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG = 0x8C01;
    static const UInt32 GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG = 0x8C02;
    static const UInt32 GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG = 0x8C03;
    static const UInt32 GL_ETC1_RGB8_OES = 0x8d64;

    static const UInt8 KTX_IDENTIFIER_REF[12] = { 171 /*'«'*/, 'K', 'T', 'X', ' ', '1', '1', 187 /*'»'*/, '\r', '\n', '\x1A', '\n' };
    static const UInt32 KTX_ENDIAN_REF = 0x04030201;
    static const UInt32 KTX_ENDIAN_REF_REV = 0x01020304;

    void Ktx::Save(Stream* output, Texture* tex)
    {
        // TODO: Make sure contents in tex is sane

        KTX_header header;
        memcpy(header.identifier, KTX_IDENTIFIER_REF, 12);
        header.endianness = KTX_ENDIAN_REF;
        header.glType = 0;
        header.glTypeSize = 1;
        header.glFormat = 0;

        switch (tex->Faces[0].MipLevels[0]->GetFormat())
        {
        case FormatCompressedRGB_ETC1:
            header.glInternalFormat = GL_ETC1_RGB8_OES;
            header.glBaseInternalFormat = GL_RGB;
            break;

        case FormatCompressedRGB_PVRTC_2BPP:
            header.glInternalFormat = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
            header.glBaseInternalFormat = GL_RGB;
            break;

        case FormatCompressedRGB_PVRTC_4BPP:
            header.glInternalFormat = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
            header.glBaseInternalFormat = GL_RGB;
            break;

        case FormatCompressedRGBA_PVRTC_2BPP:
            header.glInternalFormat = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
            header.glBaseInternalFormat = GL_RGBA;
            break;

        case FormatCompressedRGBA_PVRTC_4BPP:
            header.glInternalFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
            header.glBaseInternalFormat = GL_RGBA;
            break;

        default:
            XLI_THROW((String)"Unsupported texture format: " + FormatInfo::ToString(tex->Faces[0].MipLevels[0]->GetFormat()));
            break;
        }

        header.pixelWidth = tex->Faces[0].MipLevels[0]->GetWidth();
        header.pixelHeight = tex->Faces[0].MipLevels[0]->GetHeight();
        header.pixelDepth = 0;
        header.numberOfArrayElements = 0;
        header.numberOfFaces = tex->Faces.Length();
        header.numberOfMipmapLevels = tex->Faces[0].MipLevels.Length();
        header.bytesOfKeyValueData = 0;

        output->Write(&header, sizeof(KTX_header), 1);

        for (int i = 0; i < tex->Faces[0].MipLevels.Length(); i++)
        {
            int imageSize = tex->Faces[0].MipLevels[i]->GetSizeInBytes();
            output->Write(&imageSize, sizeof(int), 1);

            for (int j = 0; j < tex->Faces.Length(); j++)
            {
                output->Write(tex->Faces[j].MipLevels[i]->GetPtr(), sizeof(unsigned int), tex->Faces[j].MipLevels[i]->GetSizeInBytes() / 4);
            }
        }
    }

    void Ktx::Save(const String& filename, Texture* tex)
    {
        File f(filename, FileModeWrite);
        Save(&f, tex);
    }

    Texture* Ktx::Load(Stream* input)
    {
        KTX_header header;
        input->ReadSafe(&header, sizeof(KTX_header), 1);

        if (memcmp(header.identifier, KTX_IDENTIFIER_REF, 12) != 0) 
            XLI_THROW("Unable to load KTX file: Invalid header");
        
        if (header.endianness != KTX_ENDIAN_REF) 
            XLI_THROW("Unable to load KTX file: Unhandled endianess");
        
        if (header.pixelDepth > 1) 
            XLI_THROW("Unable to load KTX file: Unsupported texture depth: " + (String)(int)header.pixelDepth);
        
        if (header.numberOfArrayElements > 1) 
            XLI_THROW("Unable to load KTX file: Unsupported array size: " + (String)(int)header.numberOfArrayElements);

        // Skip meta data
        if (header.bytesOfKeyValueData > 0)
        {
            UInt8* tmp = new UInt8[header.bytesOfKeyValueData];
            input->ReadSafe(tmp, 1, header.bytesOfKeyValueData);
            delete [] tmp;
        }

        Texture* tex = 0;
        if (header.numberOfFaces == 1) 
            tex = new Texture(TextureType2D);
        else if (header.numberOfFaces == 6)
            tex = new Texture(TextureTypeCube);
        else 
            XLI_THROW("Unable to load KTX file: Unsupported texture type");

        tex->Faces.Resize(header.numberOfFaces);

        for (UInt32 i = 0; i < header.numberOfMipmapLevels; i++)
        {
            int mw = header.pixelWidth >> i;
            int mh = header.pixelHeight >> i;
            if (mw == 0) mw = 1;
            if (mh == 0) mh = 1;

            UInt32 sizeInBytes;
            input->ReadSafe(&sizeInBytes, sizeof(UInt32), 1);

            for (UInt32 j = 0; j < header.numberOfFaces; j++)
            {
                Managed<Buffer> buf = Buffer::Create(sizeInBytes);
                input->ReadSafe(buf->Ptr(), 1, buf->Size());

                switch (header.glInternalFormat)
                {
                case GL_ETC1_RGB8_OES:
                    tex->Faces[j].MipLevels.Add(new CompressedImage(mw, mh, 0, FormatCompressedRGB_ETC1, buf));
                    break;

                case GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG:
                    tex->Faces[j].MipLevels.Add(new CompressedImage(mw, mh, 0, FormatCompressedRGB_PVRTC_2BPP, buf));
                    break;

                case GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG:
                    tex->Faces[j].MipLevels.Add(new CompressedImage(mw, mh, 0, FormatCompressedRGB_PVRTC_4BPP, buf));
                    break;

                case GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG:
                    tex->Faces[j].MipLevels.Add(new CompressedImage(mw, mh, 0, FormatCompressedRGBA_PVRTC_2BPP, buf));
                    break;

                case GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG:
                    tex->Faces[j].MipLevels.Add(new CompressedImage(mw, mh, 0, FormatCompressedRGBA_PVRTC_4BPP, buf));
                    break;

                default:
                    XLI_THROW("Unable to load KTX file: Unsupported format (" + String::HexFromInt(header.glInternalFormat) + ")");
                    break;
                }
            }
        }

        return tex;
    }

    Texture* Ktx::Load(const String& filename)
    {
        File f(filename, FileModeRead);
        return Load(&f);
    }
}
