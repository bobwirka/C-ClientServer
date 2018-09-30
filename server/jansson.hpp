/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#ifndef JANSSON_HPP
#define JANSSON_HPP

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>  /* for size_t */
#include <stdarg.h>

#include <string>

using std::string;

/*
 * Original types from jansson.h
 */
typedef enum {
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_STRING,
    JSON_INTEGER,
    JSON_REAL,
    JSON_TRUE,
    JSON_FALSE,
    JSON_NULL
} json_type;

/*****************************************************************************
 *
 * New structures and classes.
 *
 *****************************************************************************/

/*
 * Generic jansson value.
 *
 * Returned by one of the 'value()' methods of the json_t class.
 */
typedef struct
{
    json_type       type;
    string          str;
    union
    {
        int         i;
        float       f;
        bool        b;
        //
    }               num;
    //
    bool            isNull();
    bool            isString();
    bool            isInt();
    bool            isReal();
    bool            isBool();
    //
} json_value_t;

/*
 * This is the original jansson structure modified to be class.
 *
 * The intent is to make a more user friendly interface to the
 * jansson library.
 *
 * Note that all json_t must be POINTERS; you can't have a json_t
 * as an automatic local variable.
 *
 * This is because the json_t* returned by json_object() is a pointer
 * to a member of an enclosing structure:
 * 		json_object_t
 * 		json_array_t
 * 		json_string_t
 * 		json_real_t
 * 		json_integer_t
 */
class json_t
{
public:

    json_type type;
    size_t refcount;
    //
    json_t*         child(const char* key);     // For key/value pairs.
    json_t*         child(int index);           // For arrays.

    json_value_t    getValue(json_t* obj);
    json_value_t    value(const char* key);     // For key/value pairs.
    json_value_t    value(int index);           // For arrays.

    json_t*         addChild(const char* key);
    json_t*         addChild(const char* key , json_t* json);
    json_t*         addChild(const char* key , const char* value);
    json_t*         addChild(const char* key , int32_t value);
    json_t*         addChild(const char* key , uint32_t value);
    json_t*         addChild(const char* key , float value);
    json_t*         addChild(const char* key , bool value);

    json_t*         addArray(const char* key);
    json_t*         addArrayObject();                   // Adds an empty object.
    json_t*         addArrayValue(const char* value);
    json_t*         addArrayValue(int value);
    json_t*         addArrayValue(float value);
    json_t*         addArrayValue(bool value);

    int             arraySize();

    const char*     toString(string* str , int indent);
};

/*
 * Forward reference for jsonNew() functions below.
 */
struct json_error_t;

/*
 * Constructors.
 */
json_t*				jsonNew();
json_t*				jsonNew(char* data , json_error_t* error);
json_t*				jsonNew(char* data , int len , json_error_t* error);
json_t*				jsonNew(const char* filename , json_error_t* error);

/*
 * Destructor.
 */
#define				jsonDel(x)		(json_decref(x) , x=NULL)

/*****************************************************************************
 *
 * End of new structures and classes.
 *
 *****************************************************************************/

#include	<jansson.h>

#endif	// JANSON_HPP
