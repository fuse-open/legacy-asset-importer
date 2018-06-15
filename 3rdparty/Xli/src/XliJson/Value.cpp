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

#include <XliJson/Value.h>

namespace Xli
{
    const char* Value::TypeToString(ValueType type)
    {
        switch (type)
        {
        case ValueTypeUndefined: return "Undefined";
        case ValueTypeString: return "String";
        case ValueTypeInteger: return "Integer";
        case ValueTypeInt64: return "Integer (64-bit)";
        case ValueTypeDouble: return "Double";
        case ValueTypeBool: return "Bool";        
        case ValueTypeArray: return "Array";
        case ValueTypeObject: return "Object";
        case ValueTypeOrderedObject: return "Object (ordered)";
        default: return "<unknown>";
        }
    }

    bool Value::IsArray() const
    {
        return object->GetType() == ValueTypeArray;
    }
    
    bool Value::IsObject() const 
    {
        ValueType t = object->GetType();
        return t == ValueTypeObject || t == ValueTypeOrderedObject; 
    }

    bool Value::IsUndefined() const
    {
        return object->GetType() == ValueTypeUndefined;
    }

    String IValue::ToString() const
    {
        return Value::TypeToString(GetType());
    }

    #define UNSUPPORTED XLI_THROW((String)XLI_FUNCTION + " is not supported for value of type " + Value::TypeToString(GetType()))

    UInt32 IValue::Hash() const { UNSUPPORTED; }

    Int32 IValue::ToInt32() const { UNSUPPORTED; }
    Int64 IValue::ToInt64() const { UNSUPPORTED; }
    double IValue::ToDouble() const { UNSUPPORTED; }
    bool IValue::ToBool() const  { UNSUPPORTED; }
    
    void IValue::SetValue(int it, const Value& value) { UNSUPPORTED; }
    const Value& IValue::GetValue(int it) const { UNSUPPORTED; }
    const Value& IValue::GetKey(int it) const { UNSUPPORTED; }

    bool IValue::ContainsKey(const Value& key) const { UNSUPPORTED; }
    void IValue::Insert(const Value& key, const Value& value) { UNSUPPORTED; }
    void IValue::Remove(const Value& value) { UNSUPPORTED; }
    void IValue::RemoveAt(const Value& key) { UNSUPPORTED; }
    void IValue::Append(const Value& value) { UNSUPPORTED; }
    Value& IValue::Get(const Value& key) { UNSUPPORTED; }
    int IValue::Count() const { UNSUPPORTED; }

    bool IValue::Equals(const Value& v) const { UNSUPPORTED; }

    #undef UNSUPPORTED

    Value::Value(): Managed<IValue>(new Undefined()) {}
    Value::Value(bool b): Managed<IValue>(new BoolValue(b)) {}
    Value::Value(Int32 v): Managed<IValue>(new IntegerValue(v)) {}
    Value::Value(Int64 v): Managed<IValue>(new Int64Value(v)) {}
    Value::Value(float f): Managed<IValue>(new DoubleValue(f)) {}
    Value::Value(double d): Managed<IValue>(new DoubleValue(d)) {}
    Value::Value(const String& s): Managed<IValue>(new StringValue(s)) {}
    Value::Value(const char* s): Managed<IValue>(new StringValue(String(s))) {}
    Value::Value(IValue* v): Managed<IValue>(v) {}

    Value Value::CreateArray()
    {
        return Value(new ArrayValue());
    }

    Value Value::CreateObject()
    {
        return Value(new ObjectValue());
    }

    Value Value::CreateOrderedObject()
    {
        return Value(new OrderedObjectValue());
    }

    template <typename T> 
    static void SetToArray(Value& v, const T* values, int count)
    {
        ArrayValue* a = new ArrayValue();
        a->Values.Resize(count);

        for (int i = 0; i < count; i++) 
            a->Values[i] = values[i];

        v = a;
    }

    static void GetFloatArray(const Value& v, float* values, int count)
    {
        for (int i = 0; i < count; i++) 
            values[i] = v.GetValue(i).ToFloat();
    }

    static void GetIntArray(const Value& v, int* values, int count)
    {
        for (int i = 0; i < count; i++) 
            values[i] = v.GetValue(i).ToInt32();
    }

    Value::Value(const Vector2i& v)
    {
        SetToArray(*this, v.Data, 2);
    }

    Value::Value(const Vector3i& v)
    {
        SetToArray(*this, v.Data, 3);
    }

    Value::Value(const Vector4i& v)
    {
        SetToArray(*this, v.Data, 4);
    }

    Value::Value(const Vector2& v)
    {
        SetToArray(*this, v.Data, 2);
    }

    Value::Value(const Vector3& v)
    {
        SetToArray(*this, v.Data, 3);
    }

    Value::Value(const Vector4& v)
    {
        SetToArray(*this, v.Data, 4);
    }

    Vector2i Value::ToVector2i() const
    {
        Vector2i v;
        GetIntArray(*this, v.Data, 2);
        return v;
    }

    Vector3i Value::ToVector3i() const
    {
        Vector3i v;
        GetIntArray(*this, v.Data, 3);
        return v;
    }

    Vector4i Value::ToVector4i() const
    {
        Vector4i v;
        GetIntArray(*this, v.Data, 4);
        return v;
    }

    Vector2 Value::ToVector2() const
    {
        Vector2 v;
        GetFloatArray(*this, v.Data, 2);
        return v;
    }

    Vector3 Value::ToVector3() const
    {
        Vector3 v;
        GetFloatArray(*this, v.Data, 3);
        return v;
    }

    Vector4 Value::ToVector4() const
    {
        Vector4 v;
        GetFloatArray(*this, v.Data, 4);
        return v;
    }
}
