#ifndef __XLI_URI_H__
#define __XLI_URI_H__

#include <Xli/String.h>

namespace Xli
{
    /**
        \ingroup XliText
    */
    class Uri
    {
    public:
        static String Encode(const String& uri);
        static String AutoEncodeUri(const String& uri);
        static String Decode(const String& uri);
    };
}

#endif
