#ifndef __UASSETS_INTERNAL_H__
#define __UASSETS_INTERNAL_H__

#include <Xli/Unicode.h>
#include <Xli/Console.h>
#include <XliFont/FontFace.h>
#include <XliImage/Texture.h>
#include <Fonts/SdfFont.h>
#include <uAsset/Base.h>

#ifdef WIN32
# define EXPORT_CLASS __declspec(dllexport)
#else
# define EXPORT_CLASS
#endif

using namespace Xli;
using namespace Fonts;

class EXPORT_CLASS uStringData: public Object
{
public:
    Utf16String String;
};

class EXPORT_CLASS uObject: public Object
{
public:
    Array<Managed<Object> > AllocatedObjects;
    Utf16String ErrorMessage;
    int TextureFileFormat;

    Managed<SdfFont> SdfFontPtr;
    Managed<Texture> TexturePtr;
    Managed<FontFace> FontFacePtr;
    Managed<Bitmap> BitmapPtr;

    uString GetStringHandle(const String& str);
    void SetErrorMessage(const String& str);

    uObject();
    virtual ~uObject();
};

#define U_BEGIN_FUNC \
    uObject* obj = (uObject*)handle; \
    \
    try \
    {

#define U_END_FUNC \
    } \
    catch (const Exception& e) \
    { \
        obj->SetErrorMessage(e.GetMessage()); \
        return -1; \
    }


#endif
