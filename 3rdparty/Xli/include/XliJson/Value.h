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

#ifndef __XLI_VALUE_H__
#define __XLI_VALUE_H__

#include <Xli/String.h>
#include <Xli/Managed.h>
#include <Xli/HashMap.h>
#include <Xli/Map.h>
#include <Xli/Console.h>
#include <Xli/Traits.h>

/**
    \addtogroup XliJson
    @{
*/
#define XLI_GET_X(label, x) do { try { x; } catch (const Xli::Exception& e) \
    { Xli::Error->WriteLine((Xli::String)"WARNING: " + XLI_FUNCTION + ": Unable to read '" + label + "': " + e.ToString()); } } while (0)

#define XLI_GET_VALUE(value, result)    XLI_GET_X(#result, result = value[#result])
#define XLI_GET_STRING(value, result)    XLI_GET_X(#result, Value __tmp = value[#result]; if (__tmp.IsUndefined()) XLI_THROW("Value is undefined"); else result = __tmp.ToString())
#define XLI_GET_INT(value, result)        XLI_GET_X(#result, result = value[#result].ToInt32())
#define XLI_GET_BOOL(value, result)        XLI_GET_X(#result, result = value[#result].ToBool())
#define XLI_GET_DOUBLE(value, result)    XLI_GET_X(#result, result = value[#result].ToDouble())
#define XLI_GET_FLOAT(value, result)    XLI_GET_X(#result, result = value[#result].ToFloat())
#define XLI_GET_VECTOR2I(value, result)    XLI_GET_X(#result, result = value[#result].ToVector2i())
#define XLI_GET_VECTOR3I(value, result)    XLI_GET_X(#result, result = value[#result].ToVector3i())
#define XLI_GET_VECTOR4I(value, result)    XLI_GET_X(#result, result = value[#result].ToVector4i())

/** @} */

namespace Xli
{
    class Value;

    /**
        \ingroup XliJson
    */
    enum ValueType
    {
        ValueTypeUndefined,
        ValueTypeString,
        ValueTypeInteger,
        ValueTypeInt64,
        ValueTypeDouble,
        ValueTypeBool,            
        ValueTypeArray,
        ValueTypeObject,
        ValueTypeOrderedObject,
    };

    /**
        \ingroup XliJson
    */
    class IValue: public Object
    {
    public:
        virtual ValueType GetType() const = 0;

        virtual UInt32 Hash() const;
        virtual bool Equals(const Value& v) const;

        virtual String ToString() const;
        virtual Int32 ToInt32() const;
        virtual Int64 ToInt64() const;
        virtual double ToDouble() const;
        virtual bool ToBool() const;

        virtual int Begin() const { return -1; }
        virtual int End() const { return -1; }
        virtual int Next(int it) const { return -1; }
        virtual void SetValue(int it, const Value& value);
        virtual const Value& GetValue(int it) const;
        virtual const Value& GetKey(int it) const;

        virtual bool ContainsKey(const Value& key) const;
        virtual void Insert(const Value& key, const Value& value);
        virtual void Remove(const Value& value);
        virtual void RemoveAt(const Value& key);
        virtual void Append(const Value& value);
        virtual Value& Get(const Value& key);
        virtual int Count() const;
    };

    /**
        \ingroup XliJson
    */
    class Value: public Managed<IValue>
    {
    public:
        Value();
        Value(bool b);
        Value(Int32 v);
        Value(Int64 v);
        Value(float f);
        Value(double d);
        Value(const String& s);
        Value(const char* s);
        Value(IValue* value);

        static Value CreateArray();
        static Value CreateObject();
        static Value CreateOrderedObject();

        static const char* TypeToString(ValueType type);

        UInt32 Hash() const { return object->Hash(); }
        ValueType GetType() const { return object->GetType(); }

        bool IsArray() const;
        bool IsObject() const;
        bool IsUndefined() const;

        String ToString() const { return object->ToString(); }
        Int32 ToInt32() const { return object->ToInt32(); }
        Int64 ToInt64() const { return object->ToInt64(); }
        float ToFloat() const { return (float)object->ToDouble(); }
        double ToDouble() const { return object->ToDouble(); }
        bool ToBool() const { return object->ToBool(); }

        void ToType(String& result) const { result = ToString(); } 
        void ToType(Int32& result) const { result = ToInt32(); } 
        void ToType(Int64& result) const { result = ToInt64(); }
        void ToType(float& result) const { result = ToFloat(); } 
        void ToType(double& result) const { result = ToDouble(); }
        void ToType(bool& result) const { result = ToBool(); } 

        Value(const Vector2i& v);
        Value(const Vector3i& v);
        Value(const Vector4i& v);

        Value(const Vector2& v);
        Value(const Vector3& v);
        Value(const Vector4& v);

        Vector2i ToVector2i() const;
        Vector3i ToVector3i() const;
        Vector4i ToVector4i() const;

        Vector2 ToVector2() const;
        Vector3 ToVector3() const;
        Vector4 ToVector4() const;

        void ToType(Vector2i& result) const { result = ToVector2i(); } 
        void ToType(Vector3i& result) const { result = ToVector3i(); }
        void ToType(Vector4i& result) const { result = ToVector4i(); } 

        void ToType(Vector2& result) const { result = ToVector2(); } 
        void ToType(Vector3& result) const { result = ToVector3(); } 
        void ToType(Vector4& result) const { result = ToVector4(); }

        int Begin() const { return object->Begin(); }
        int End() const { return object->End(); }
        int Next(int it) const { return object->Next(it); }
        void SetValue(int it, const Value& value) { object->SetValue(it, value); }
        const Value& GetValue(int it) const { return object->GetValue(it); }
        const Value& GetKey(int it) const { return object->GetKey(it); }

        bool ContainsKey(const Value& key) const { return object->ContainsKey(key); }
        void Insert(const Value& key, const Value& value) { object->Insert(key, value); }
        void Remove(const Value& value) { object->Remove(value); }
        void RemoveAt(const Value& key) { object->RemoveAt(key); }
        void Append(const Value& value) { object->Append(value); }
        Value& Get(const Value& key) { return object->Get(key); }
        Value& operator [] (const Value& key) { return object->Get(key); }
        const Value& Get(const Value& key) const { return object->Get(key); }
        const Value& operator [] (const Value& key) const { return object->Get(key); }

        int Count() const { return object->Count(); }

        bool operator == (const Value& v) const { return object->Equals(v); }
        bool operator != (const Value& v) const { return !object->Equals(v); }
    };

    /**
        \ingroup XliJson
    */
    class Undefined: public IValue
    {
    public:
        virtual ValueType GetType() const { return ValueTypeUndefined; }
        virtual String ToString() const { return "null"; }
    };

    /**
        \ingroup XliJson
    */
    class BoolValue: public IValue
    {
        bool value;

    public:
        virtual ValueType GetType() const { return ValueTypeBool; }
        virtual String ToString() const { return DefaultTraits::ToString(value); }
        virtual bool ToBool() const { return value; }        
        virtual Int32 ToInt32() const { return value? 1 : 0; }        
        virtual Int64 ToInt64() const { return value? 1 : 0; }        
        virtual double ToDouble() const { return value? 1.0 : 0.0; } 

        virtual UInt32 Hash() const { return DefaultTraits::Hash(value); }
        virtual bool Equals(const Value& v) const {  try { return value == v->ToBool(); } catch(const Exception&) { return false; } }

        BoolValue(bool val) { value = val; }
        operator bool() { return value; }
    };

    /**
        \ingroup XliJson
    */
    class StringValue: public IValue
    {
        String value;

    public:
        virtual ValueType GetType() const { return ValueTypeString; }
        virtual String ToString() const { return value; }
        virtual Int32 ToInt32() const { return value.ToInt(); }
        virtual bool ToBool() const { if (value=="true") return true; if (value=="false") return false; XLI_THROW("Could not convert string to bool"); }

        virtual UInt32 Hash() const { return DefaultTraits::Hash(value); }
        virtual bool Equals(const Value& v) const { try {return value == v->ToString(); } catch(const Exception&) { return false; } }

        StringValue(const String& str) { value = str; }
        operator const String&() { return value; }
    };

    /**
        \ingroup XliJson
    */
    class IntegerValue: public IValue
    {
        Int32 value;

    public:
        virtual ValueType GetType() const { return ValueTypeInteger; }
        virtual String ToString() const { return DefaultTraits::ToString(value); }
        virtual Int32 ToInt32() const { return value; }
        virtual Int64 ToInt64() const { return value; }
        virtual double ToDouble() const { return (double)value; }
        virtual bool ToBool() const { return value != 0; }

        virtual UInt32 Hash() const { return DefaultTraits::Hash(value); }
        virtual bool Equals(const Value& v) const {  try { return value == v->ToInt32(); } catch(const Exception&) { return false; } }

        IntegerValue(int val) { value = val; }
    };

    /**
        \ingroup XliJson
    */
    class Int64Value: public IValue
    {
        Int64 value;

    public:
        virtual ValueType GetType() const { return ValueTypeInteger; }
        virtual String ToString() const { return DefaultTraits::ToString(value); }
        virtual Int32 ToInt32() const { return (Int32)value; }
        virtual Int64 ToInt64() const { return value; }
        virtual double ToDouble() const { return (double)value; }
        virtual bool ToBool() const { return value != 0; }
        
        virtual UInt32 Hash() const { return DefaultTraits::Hash(value); }
        virtual bool Equals(const Value& v) const {  try { return value == v->ToInt64(); } catch(const Exception&) { return false; } }

        Int64Value(Int64 val) { value = val; }
    };
    
    /**
        \ingroup XliJson
    */
    class DoubleValue: public IValue
    {
        double value;

    public:
        virtual ValueType GetType() const { return ValueTypeDouble; }
        virtual String ToString() const { return DefaultTraits::ToString(value); }
        virtual Int32 ToInt32() const { return (Int32)value; }
        virtual Int64 ToInt64() const { return (Int64)value; }
        virtual double ToDouble() const { return value; }
        virtual bool ToBool() const { return value != 0.0; }

        virtual UInt32 Hash() const { return DefaultTraits::Hash(value); }
        virtual bool Equals(const Value& v) const { try{return value == v->ToDouble(); } catch(const Exception&) { return false; } }

        DoubleValue(double val) { value = val; }
        operator double() { return value; }
    };

    /**
        \ingroup XliJson
    */
    class ArrayValue: public IValue
    {
    public:
        Array<Value> Values;

        virtual ValueType GetType() const { return ValueTypeArray; }

        virtual int Begin() const { return 0; }
        virtual int End() const { return Values.Length(); }
        virtual int Next(int it) const { return it + 1; }
        virtual void SetValue(int it, const Value& value) { Values[it] = value; }
        virtual const Value& GetValue(int it) const { return Values[it]; }

        virtual bool ContainsKey(const Value& key) const { int k = key.ToInt32(); return (k >= 0) && (k < Values.Length()); }
        virtual void Insert(const Value& key, const Value& value) { Values.Insert(key.ToInt32(), value); }
        virtual void Remove(const Value& value) { Values.Remove(value); }
        virtual void RemoveAt(const Value& key) { Values.RemoveAt(key.ToInt32()); }
        virtual void Append(const Value& value) { Values.Add(value); }
        virtual Value& Get(const Value& key) { return Values[key.ToInt32()]; }
        virtual int Count() const { return Values.Length(); }        
    };

    /**
        \ingroup XliJson
    */
    class ObjectValue: public IValue
    {
    public:
        HashMap<Value, Value> Values;

        virtual ValueType GetType() const { return ValueTypeObject; }

        virtual int Begin() const { return Values.Begin(); }
        virtual int End() const { return Values.End(); }
        virtual int Next(int it) const { return Values.Next(it); }
        virtual void SetValue(int it, const Value& value) { Values.SetValue(it, value); }
        virtual const Value& GetValue(int it) const { return Values.GetValue(it); }
        virtual const Value& GetKey(int it) const { return Values.GetKey(it); }

        virtual bool ContainsKey(const Value& key) const { return Values.ContainsKey(key); }
        virtual void Insert(const Value& key, const Value& value) { Values[key] = value; } // changed from .Add!!!!
        virtual void RemoveAt(const Value& key) { Values.Remove(key); }
        virtual Value& Get(const Value& key) { return Values[key]; }
        virtual int Count() const { return Values.Count(); }
    };

    /**
        \ingroup XliJson
    */
    class OrderedObjectValue: public IValue
    {
    public:
        Map<Value, Value> Values;

        virtual ValueType GetType() const { return ValueTypeOrderedObject; }

        virtual int Begin() const { return Values.Begin(); }
        virtual int End() const { return Values.End(); }
        virtual int Next(int it) const { return Values.Next(it); }
        virtual void SetValue(int it, const Value& value) { Values.SetValue(it, value); }
        virtual const Value& GetValue(int it) const { return Values.GetValue(it); }
        virtual const Value& GetKey(int it) const { return Values.GetKey(it); }

        virtual bool ContainsKey(const Value& key) const { return Values.ContainsKey(key); }
        virtual void Insert(const Value& key, const Value& value) { Values.Add(key, value); }
        virtual void RemoveAt(const Value& key) { Values.Remove(key); }
        virtual Value& Get(const Value& key) { return Values[key]; }
        virtual int Count() const { return Values.Count(); }
    };

    /**
        \ingroup XliJson
    */

    template <> struct Traits<Value>
    {
        static bool Equals(const Value& a, const Value& b) 
        {
            return a == b;
        }

        static UInt32 Hash(const Value& t) 
        {
            return t.Hash();
        }

        static String ToString(const Value& t)
        {
            return t.ToString();
        }
    };

    /** @} */
};

#endif
