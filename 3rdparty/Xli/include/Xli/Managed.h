/* * Copyright (C) 2010-2014 Outracks Technologies
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __XLI_MANAGED_H__
#define __XLI_MANAGED_H__

#include <Xli/Config.h>
#include <Xli/Exception.h>

namespace Xli
{
    /**
        \ingroup XliObject
    */
    template <typename T> class Managed
    {
    protected:
        T* object;

    public:
        Managed()
        {
            this->object = 0;
        }

        Managed(const Managed& managed)
        {
            this->object = managed.object;
            if (object) object->AddRef();
        }

        Managed(T* object)
        {
            this->object = object;
        }

        ~Managed()
        {
            if (object) 
                object->Release();
        }

        bool IsNull() const
        {
            return !object;
        }

        Managed& operator = (T* ptr)
        {
            if (ptr == object)
                return *this;
            
            if (object) 
                object->Release();

            object = ptr;
            return *this;
        }

        Managed& operator = (const Managed& managed)
        {
            if (managed.object == object)
                return *this;

            if (object) 
                object->Release();

            object = managed.object;

            if (object) 
                object->AddRef();

            return *this;
        }

        T* Get()
        {
#ifdef XLI_NULL_POINTER_CHECK
            if (!object) 
                XLI_THROW_NULL_POINTER;
#endif
            return object;
        }

        const T* Get() const
        {
#ifdef XLI_NULL_POINTER_CHECK
            if (!object) 
                XLI_THROW_NULL_POINTER;
#endif
            return object;
        }

        operator T*()
        {
            return object;
        }

        operator const T*() const
        {
            return object;
        }

        T* operator ->()
        {
            return Get();
        }

        const T* operator ->() const
        {
            return Get();
        }
    };

    /**
        \ingroup XliObject
    */
    template <typename T> 
    Managed<T> ManagePtr(T* object)
    {
        return object;
    }
}


#endif
