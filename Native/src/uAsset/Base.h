#ifndef __UASSETS_BASE_H__
#define __UASSETS_BASE_H__

#ifdef WIN32
# define EXPORT extern "C" __declspec(dllexport)
# define STDCALL __stdcall
#else
# define EXPORT extern "C"
# define STDCALL
#endif

/**
    \addtogroup Base
    @{
*/

#ifdef WIN32
typedef const wchar_t* uString;
#else
typedef const unsigned short* uString;
#endif

typedef void* uHandle;

EXPORT void STDCALL uInitialize();
EXPORT void STDCALL uInitializePlus();

EXPORT uHandle STDCALL uCreate();
EXPORT void STDCALL uRelease(uHandle handle);
EXPORT uString STDCALL uGetErrorString(uHandle handle);

/** @} */

#endif
