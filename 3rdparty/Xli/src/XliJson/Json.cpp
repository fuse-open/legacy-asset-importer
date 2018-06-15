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

#include <Xli/Console.h>
#include <Xli/StringBuilder.h>
#include <Xli/TextStream.h>
#include <XliJson/Json.h>
#include <XliJson/FormattedTextWriter.h>
#include "../../3rdparty/JSON_parser/JSON_parser.h"

namespace Xli
{
    class JsonContext
    {
    public:
        Array<Value> Values;
        Array<String> Keys;
        Value Result;
        bool PreserveOrder;

        JsonContext(bool preserveOrder): PreserveOrder(preserveOrder)
        {
        }

        void AddValue(Value v)
        {
            if (!Values.Length() && Result.IsUndefined()) 
                Result = v;
            else if (Values.Last().IsArray()) 
                Values.Last()->Append(v);
            else if (Values.Last().IsObject()) 
                Values.Last()->Insert(Keys.RemoveLast(), v);
            else 
                XLI_THROW("JSON parser failed");
        }
    };

    static int JsonCallback(void* ctx, int type, const JSON_value* value)
    {
        JsonContext* c = (JsonContext*)ctx;

        switch(type) 
        {
        case JSON_T_ARRAY_BEGIN:    
            c->Values.Add(new ArrayValue());
            break;

        case JSON_T_ARRAY_END:
            c->AddValue(c->Values.RemoveLast());
            break;

        case JSON_T_OBJECT_BEGIN:
            c->PreserveOrder ? c->Values.Add(new OrderedObjectValue()) : c->Values.Add(new ObjectValue());
            break;

        case JSON_T_OBJECT_END:
            c->AddValue(c->Values.RemoveLast());
            break;

        case JSON_T_INTEGER:
            c->AddValue(new IntegerValue(value->vu.integer_value));
            break;

        case JSON_T_FLOAT:
            c->AddValue(new DoubleValue(value->vu.float_value));
            break;

        case JSON_T_NULL:
            c->AddValue(new Undefined());
            break;

        case JSON_T_TRUE:
            c->AddValue(new BoolValue(true));
            break;

        case JSON_T_FALSE:
            c->AddValue(new BoolValue(false));
            break;

        case JSON_T_KEY:
            c->Keys.Add(String(value->vu.str.value, (int)value->vu.str.length));
            break;   

        case JSON_T_STRING:
            c->AddValue(new StringValue(String(value->vu.str.value, (int)value->vu.str.length)));
            break;
            
        default:
            XLI_THROW("JSON parser failed");
            break;
        }
    
        return 1;
    }

    Value Json::Parse(const String& code, bool preserveOrder)
    {
        JSON_config config;

        struct JSON_parser_struct* jc = NULL;
    
        init_JSON_config(&config);
    
        config.depth                  = 19;
        config.callback               = &JsonCallback;
        config.callback_ctx           = new JsonContext(preserveOrder);
        config.allow_comments         = 1;
        config.handle_floats_manually = 0;
    
        jc = new_JSON_parser(&config);

        int ln = 1;
    
        for (int i = 0; i < code.Length(); i++)
        {
            int next_char = code[i];

            if (!JSON_parser_char(jc, next_char))
            {
                delete_JSON_parser(jc);
                XLI_THROW("JSON Syntax error on line " + (String)ln);
            }

            if (next_char == '\n') ln++;
        }

        if (!JSON_parser_done(jc))
        {
            delete_JSON_parser(jc);
            XLI_THROW("JSON Syntax error on line " + (String)ln);
        }

        JsonContext* c = (JsonContext*)config.callback_ctx;
        Value val = c->Result;

        delete c;
        delete_JSON_parser(jc);

        return val;
    }

    String Json::ToStringLiteral(const String& str)
    {
        StringBuilder w;

        w.Append('"');

        for (int i = 0; i < str.Length(); i++)
        {
            switch (str[i])
            {
            case '"': w.Append("\\\""); break;
            case '\n': w.Append("\\n"); break;
            case '\r': w.Append("\\r"); break;
            case '\t': w.Append("\\t"); break;
            case '\0': w.Append("\\0"); break;
            case '\\': w.Append("\\\\"); break;
            default: w.Append(str[i]); break;
            }
        }

        w.Append('"');
        return w.ToString();
    }

    class JsonWriter: public FormattedTextWriter
    {
    public:
        Array<bool> IsArrayStack;
        bool ArrayEndSkipLine;

        JsonWriter(Stream* stream): FormattedTextWriter(stream), ArrayEndSkipLine(false)
        {
        }

        void WriteRecursive(const Value& value)
        {
            switch (value.GetType())
            {
            case ValueTypeString:
                Write(Json::ToStringLiteral(value.ToString()));
                break;

            case ValueTypeUndefined:
            case ValueTypeInteger:
            case ValueTypeInt64:
            case ValueTypeDouble:
            case ValueTypeBool:
                Write(value.ToString());
                break;

            case ValueTypeArray:
                {
                    Write("[");
                    IsArrayStack.Add(true);
                    ArrayEndSkipLine = false;

                    if (value.Count() > 0)
                    {
                        for (int i = 0; i < value.Count(); i++)
                        {
                            if (i > 0) Write(", ");
                            WriteRecursive(value.GetValue(i));
                        }
                    }

                    IsArrayStack.RemoveLast();

                    if (ArrayEndSkipLine)
                    {
                        EndLine();
                        BeginLine();
                        ArrayEndSkipLine = false;
                    }

                    Write("]");
                }
                break;

            case ValueTypeOrderedObject:
            case ValueTypeObject:
                {
                    if (IsArrayStack.Last())
                    {
                        EndLine();
                        PushIndent();
                        BeginLine();
                    }

                    Write("{");
                    IsArrayStack.Add(false);

                    if (value.Count() > 0)
                    {
                        EndLine();
                        PushIndent();
                        BeginLine();

                        bool first = true;

                        for (int it = value.Begin(); it != value.End(); it = value.Next(it))
                        {
                            const Value& key = value.GetKey(it);
                            const Value& val = value.GetValue(it);

                            if (!first)
                            {
                                Write(",");
                                EndLine();
                                BeginLine();
                            }

                            first = false;

                            Write(Json::ToStringLiteral(key.ToString()));
                            Write(": ");
                            WriteRecursive(val);
                        }

                        EndLine();
                        PopIndent();
                        BeginLine();
                    }

                    IsArrayStack.Add();
                    Write("}");

                    if (IsArrayStack.Last())
                    {
                        PopIndent();
                        ArrayEndSkipLine = true;
                    }
                }
                break;

            default:
                Error->WriteLine("JSON WARNING: Unsupported value type: " + (String)Value::TypeToString(value.GetType()));
                break;
            }
        }

        void WriteJson(const Value& value)
        {
            BeginLine();
            IsArrayStack.Add(false);
            WriteRecursive(value);
            IsArrayStack.RemoveLast();
            EndLine();
        }
    };

    void Json::Save(Stream* stream, const Value& value)
    {
        JsonWriter(stream).WriteJson(value);
    }

    String Json::Stringify(const Value& value)
    {
        TextStream ts;
        Save(&ts, value);
        return ts.GetText();
    }
}
