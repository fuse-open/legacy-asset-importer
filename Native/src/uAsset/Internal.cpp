#include <uAsset/Internal.h>

uObject::uObject()
{
}

uObject::~uObject()
{
}

uString uObject::GetStringHandle(const String& str)
{
    uStringData* sd = new uStringData();
    sd->String = Unicode::Utf8To16(str);
    AllocatedObjects.Add(sd);
    return sd->String.Ptr();
}

void uObject::SetErrorMessage(const String& str)
{
    ErrorMessage = Unicode::Utf8To16(str);
}
