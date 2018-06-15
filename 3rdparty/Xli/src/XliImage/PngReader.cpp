//
// Copyright (C) 2010-2014 Outracks Technologies
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial
// portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
// OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include <Xli/File.h>
#include <Xli/Shared.h>
#include <XliImage/Png.h>
#include <XliImage/ImageReader.h>
#include <png.h>

namespace Xli
{
    class PngReader: public ImageReader
    {
        Shared<Stream> f;
        
        png_structp png_ptr;
        png_infop info_ptr;
        
        UInt32 width, height;
        Format format;
        int comps;
        bool isInterlaced;
        int interlacePasses;
        
        static void read(png_structp png_ptr, png_bytep data, png_size_t length)
        {
            Stream *stream = (Stream*)png_get_io_ptr(png_ptr);
            try
            {
                int bytes_read = stream->Read(data, 1, (int)length);
                if (bytes_read != length)
                    png_error(png_ptr, "Failed to read");
            }
            catch (const Xli::Exception &e)
            {
                png_error(png_ptr, e.what());
            }
        }
        
        static void error(png_structp png_ptr, png_const_charp err)
        {
            XLI_THROW(String("Error while reading from PNG file: ") + err);
        }
        
    public:
        PngReader(Stream* input)
        {
            char header[8];
            if (input->Read(header, 1, 8) != 8)
            {
                XLI_THROW("Unable to read PNG-header");
            }

            if (png_sig_cmp((png_const_bytep)header, 0, 8))
            {
                XLI_THROW("Not a PNG file");
            }

            f = input;
            
            png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, error, NULL);
            if (!png_ptr)
            {
                XLI_THROW("png_create_read_struct failed");
            }
            
            info_ptr = png_create_info_struct(png_ptr);
            if (!info_ptr)
            {
                png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
                XLI_THROW("png_create_info_struct failed");
            }
            
            png_set_read_fn (png_ptr, (png_voidp)f.Get(), read);
            
            png_set_sig_bytes(png_ptr, 8);
            png_read_info(png_ptr, info_ptr);
            
            int bit_depth = png_get_bit_depth (png_ptr, info_ptr);
            int color_type = png_get_color_type (png_ptr, info_ptr);
            
            if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);
            else if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png_ptr);
            
            if (png_get_valid (png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha (png_ptr);
            
            if (bit_depth == 16) png_set_strip_16 (png_ptr);
            else if (bit_depth < 8) png_set_packing (png_ptr);
            
            isInterlaced = png_get_interlace_type (png_ptr, info_ptr) != 0;
            interlacePasses = isInterlaced ? png_set_interlace_handling (png_ptr) : 0;
            png_read_update_info (png_ptr, info_ptr);
            
            png_get_IHDR (png_ptr, info_ptr,
                          (png_uint_32*)(&width),
                          (png_uint_32*)(&height),
                          &bit_depth, &color_type,
                          NULL, NULL, NULL);
            
            switch (color_type)
            {
            case PNG_COLOR_TYPE_GRAY:
                format = FormatL_8_UInt_Normalize;
                comps = 1;
                break;
                
            case PNG_COLOR_TYPE_GRAY_ALPHA:
                format = FormatLA_8_8_UInt_Normalize;
                comps = 2;
                break;
                
            case PNG_COLOR_TYPE_RGB: 
                format = FormatRGB_8_8_8_UInt_Normalize; 
                comps = 3;
                break;
                
            case PNG_COLOR_TYPE_RGB_ALPHA: 
                format = FormatRGBA_8_8_8_8_UInt_Normalize; 
                comps = 4;
                break;
                
            default:
                XLI_THROW("Unsupported PNG pixel format");
                break;
            }
        }
        
        virtual ~PngReader()
        {
            png_destroy_read_struct (&png_ptr, &info_ptr, NULL);
        }
        
        virtual int GetWidth()
        {
            return width;
        }
        
        virtual int GetHeight()
        {
            return height;
        }
        
        virtual int GetDepth()
        {
            return 0;
        }
        
        virtual Format GetFormat()
        {
            return format;
        }
        
        virtual int GetBufferSize()
        {
            return comps * width * height;
        }
        
        virtual void Read(void* targetBuffer, ProgressCallback* callback)
        {
            png_bytep row = (png_bytep)targetBuffer;
            int pitch = width * comps;
            
            if (isInterlaced)
            {
                png_bytep *row_pointers = new png_bytep[height];
                for (unsigned int y = 0; y < height; y++)
                    row_pointers[y] = new png_byte[png_get_rowbytes(png_ptr, info_ptr)];
                
                for (int i = 0; i < interlacePasses; i++)
                {
                    for (unsigned int y = 0; y < height; y++)
                    {
                        if (callback) callback->Update(i * height + y, interlacePasses * height);
                        png_read_row (png_ptr, row_pointers[y], NULL);
                    }
                }
                
                for (unsigned int y = 0; y < height; y++)
                {
                    if (callback) callback->Update(y, height);
                    memcpy(row, row_pointers[y], pitch);
                    row += pitch;
                }
                
                for (unsigned int y = 0; y < height; y++)
                    delete [] row_pointers[y];
                delete [] row_pointers;
            }
            else
            {
                for (unsigned int i = 0; i < height; ++i)
                {
                    if (callback) callback->Update(i, height);
                    png_read_row(png_ptr, row, NULL);
                    row += pitch;
                }
            }
            
            png_read_end(png_ptr, NULL);
            if (callback) callback->Update(height, height);
        }
    };
    
    ImageReader* Png::CreateReader(Stream* input)
    {
        return new PngReader(input);
    }
    
    Bitmap* Png::Load(Stream* input)
    {
        PngReader r(input);
        return r.ReadBitmap();
    }
    
    Bitmap* Png::Load(const String& filename)
    {
        File f(filename, FileModeRead);
        return Load(&f);
    }
}
