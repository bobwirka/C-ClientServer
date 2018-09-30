/*
 * jansson.cpp
 *
 *  Created on: Feb 18, 2018
 *      Author: rcw
 */
#include    <jansson.hpp>

using std::string;

/**************************************************************
 *
 * struct json_value_t
 *
 **************************************************************/

bool json_value_t::isNull()
{
    return (type == JSON_NULL);
}

bool json_value_t::isString()
{
    return (type == JSON_STRING);
}

bool json_value_t::isInt()
{
    return (type == JSON_INTEGER);
}

bool json_value_t::isReal()
{
    return (type == JSON_REAL);
}

bool json_value_t::isBool()
{
    return ((type == JSON_TRUE) || (type == JSON_FALSE));
}

/**************************************************************
 *
 * class json_t
 *
 **************************************************************/

json_t* jsonNew()
{
	return json_object();
}

json_t* jsonNew(char* data , json_error_t* error)
{
    return json_loads(data , 0 , error);
}

json_t* jsonNew(char* data , int len , json_error_t* error)
{
    return json_loadb(data , len , 0 , error);
}

json_t* jsonNew(const char* filename , json_error_t* error)
{
    return json_load_file(filename , 0 , error);
}

json_t* json_t::child(const char* key)
{
    return json_object_get(this , key);
}

json_t* json_t::child(int index)
{
    if (json_typeof(this) != JSON_ARRAY)
        return NULL;
    return json_array_get(this , index);
}

json_value_t json_t::getValue(json_t* obj)
{
    json_value_t    val;

    val.num.i = 0;
    if (obj == NULL)
    {
        val.type = JSON_NULL;
        return val;
    }
    val.type = json_typeof(obj);
    switch (val.type)
    {
        case JSON_STRING:
            val.str = json_string_value(obj);
            break;
        case JSON_INTEGER:
            val.num.i = json_integer_value(obj);
            break;
        case JSON_REAL:
            val.num.f = json_real_value(obj);
            break;
        case JSON_TRUE:
        case JSON_FALSE:
            val.num.b = json_boolean_value(obj);
            break;
        case JSON_OBJECT:
        case JSON_ARRAY:
        case JSON_NULL:
        default:
            val.type = JSON_NULL;
            break;
    }
    return val;
}

json_value_t json_t::value(const char* key)
{
    json_t*     json = json_object_get(this , key);

    return getValue(json);
}

json_value_t json_t::value(int index)
{
    json_t*     json = json_array_get(this , index);

    return getValue(json);
}

json_t* json_t::addChild(const char* key)
{
    json_t*     obj = json_object();

    json_object_set_new(this , key , obj);
    return json_object_get(this , key);
}

/*
 * Returns the newly added child.
 */
json_t* json_t::addChild(const char* key , json_t* json)
{
    json_object_set_new(this , key , json);
    return child(key);
}

json_t* json_t::addChild(const char* key , const char* value)
{
    json_object_set_new(this , key , json_string(value));
    return this;
}

json_t* json_t::addChild(const char* key , int32_t value)
{
    json_object_set_new(this , key , json_integer(value));
    return this;
}

json_t* json_t::addChild(const char* key , uint32_t value)
{
    json_object_set_new(this , key , json_integer(value));
    return this;
}

json_t* json_t::addChild(const char* key , float value)
{
    json_object_set_new(this , key , json_real(value));
    return this;
}

json_t* json_t::addChild(const char* key , bool value)
{
    json_object_set_new(this , key , json_boolean(value));
    return this;
}

json_t* json_t::addArray(const char* key)
{
    json_t*     json = json_array();

    json_object_set_new(this , key , json);
    return child(key);
}

/*
 * Appends an empty object to an array.
 * Returns the new object.
 */
json_t* json_t::addArrayObject()
{
    json_t*     json = json_object();

    json_array_append_new(this , json);
    return json_array_get(this , json_array_size(this) - 1);
}

json_t* json_t::addArrayValue(const char* value)
{
    json_t*     json = json_string(value);

    json_array_append_new(this , json);
    return this;
}

json_t* json_t::addArrayValue(int value)
{
    json_t*     json = json_integer(value);

    json_array_append_new(this , json);
    return this;
}

json_t* json_t::addArrayValue(float value)
{
    json_t*     json = json_real(value);

    json_array_append_new(this , json);
    return this;
}

json_t* json_t::addArrayValue(bool value)
{
    json_t*     json = json_boolean(value);

    json_array_append_new(this , json);
    return this;
}

int json_t::arraySize()
{
	return (uint32_t)json_array_size(this);
}

/*
 * Defined in jansson_private.h
 */
void jsonp_free(void *ptr);

const char* json_t::toString(string* str , int indent)
{
    char*       s = json_dumps(this , JSON_INDENT(indent));

    str->assign(s);
    jsonp_free(s);
    return str->c_str();
}
