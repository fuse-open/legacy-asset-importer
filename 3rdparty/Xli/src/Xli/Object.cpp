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

#include <Xli/Atomic.h>
#include <Xli/Object.h>
#include <Xli/Exception.h>
#include <Xli/Traits.h>

namespace Xli
{
    Object::Object() 
    { 
        refCount = 1; 
    }

    Object::~Object() 
    {
        if (refCount > 1)
            XLI_THROW_BAD_DELETE;
    }

    int Object::GetRefCount() const
    {
        return refCount;
    }

    void Object::AddRef() 
    {
        AtomicIncrement(&refCount);
    }

    void Object::Release()
    {
        if (AtomicDecrement(&refCount) == 0)
            Delete();
    }

    void Object::Delete() 
    { 
        delete this; 
    }

    String Object::ToString() const
    {
        return "<Object " + DefaultTraits::ToString((void*)this) + ">";
    }
}
