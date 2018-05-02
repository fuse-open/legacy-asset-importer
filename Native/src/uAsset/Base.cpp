#include <uAsset/Internal.h>
#include <uAsset/Base.h>
#include <Xli/CoreLib.h>
#include <XliFont/FontFace.h>

void STDCALL uInitialize()
{
	Xli::CoreLib::Init();
	Xli::FreeType::Init();
}

uHandle STDCALL uCreate()
{
    return new uObject();
}

void STDCALL uRelease(uHandle handle)
{
	((uObject*)handle)->Release();
}

uString STDCALL uGetErrorString(uHandle handle)
{
	return (uString)((uObject*)handle)->ErrorMessage.Ptr();
}

#include <Xli/Thread.h>

class DummyTask : public Xli::ThreadTask
{
public:
	virtual void Run()
	{
	}
};

void STDCALL uDummy()
{
	Thread t(new DummyTask());
}
