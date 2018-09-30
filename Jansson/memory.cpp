/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 * Copyright (c) 2011-2012 Basile Starynkevitch <basile@starynkevitch.net>
 *
 * Jansson is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#include <stdlib.h>
#include <string.h>

#include	<jansson.hpp>
#include	<jansson_private.h>

/*
 * #define to get a message when malloc & free occur.
 */
#define     DBUG_PRINT

/* C89 allows these to be macros */
#undef malloc
#undef free

/* memory function pointers */
static json_malloc_t do_malloc = malloc;
static json_free_t do_free = free;

#ifdef      DBUG_PRINT
static int		malloc_count;
#endif

void *jsonp_malloc(size_t size)
{
    if(!size)
        return NULL;

#ifdef      DBUG_PRINT
    printf("m %d\n" , ++malloc_count);
#endif

    return (*do_malloc)(size);
}

void jsonp_free(void *ptr)
{
    if(!ptr)
        return;

#ifdef      DBUG_PRINT
    printf("f %d\n" , --malloc_count);
#endif

    (*do_free)(ptr);
}

char *jsonp_strdup(const char *str)
{
    return jsonp_strndup(str, strlen(str));
}

char *jsonp_strndup(const char *str, size_t len)
{
    char *new_str;

    new_str = (char*)jsonp_malloc(len + 1);
    if(!new_str)
        return NULL;

    memcpy(new_str, str, len);
    new_str[len] = '\0';
    return new_str;
}

void json_set_alloc_funcs(json_malloc_t malloc_fn, json_free_t free_fn)
{
    do_malloc = malloc_fn;
    do_free = free_fn;
}

void json_get_alloc_funcs(json_malloc_t *malloc_fn, json_free_t *free_fn)
{
    if (malloc_fn)
        *malloc_fn = do_malloc;
    if (free_fn)
        *free_fn = do_free;
}
