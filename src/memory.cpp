/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 * Copyright (c) 2011-2012 Basile Starynkevitch <basile@starynkevitch.net>
 *
 * Jansson is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#include <stdlib.h>
#include <string.h>
#include "jansson.h"
#include "memory.hpp"

typedef struct {
    json_malloc_t malloc_fn;
    json_realloc_t realloc_fn;
    json_free_t free_fn;
} memory_resource;

static memory_resource* current_memory_resource = NULL;

static memory_resource default_memory_resource = {
    malloc,
    realloc,
    free
};

void *jsonp_malloc(size_t size) {
    if (!current_memory_resource)
        current_memory_resource = &default_memory_resource;
    return current_memory_resource->malloc_fn(size);
}

void jsonp_free(void *ptr) {
    if (!current_memory_resource)
        current_memory_resource = &default_memory_resource;
    current_memory_resource->free_fn(ptr);
}

void *jsonp_realloc(void *ptr, size_t originalSize, size_t newSize) {
    if (!current_memory_resource)
        current_memory_resource = &default_memory_resource;
    if (current_memory_resource->realloc_fn)
        return current_memory_resource->realloc_fn(ptr, newSize);
    
    // Fallback to malloc/free emulation
    if (newSize == 0) {
        current_memory_resource->free_fn(ptr);
        return NULL;
    }
    
    void* new_ptr = current_memory_resource->malloc_fn(newSize);
    if (new_ptr && ptr) {
        memcpy(new_ptr, ptr, originalSize < newSize ? originalSize : newSize);
        current_memory_resource->free_fn(ptr);
    }
    return new_ptr;
}

char *jsonp_strndup(const char *str, size_t len) {
    char *new_str;

    new_str = (char*)jsonp_malloc(len + 1);
    if (!new_str)
        return NULL;

    memcpy(new_str, str, len);
    new_str[len] = '\0';
    return new_str;
}

void json_set_alloc_funcs(json_malloc_t malloc_fn, json_free_t free_fn) {
    static memory_resource custom_mr;
    custom_mr.malloc_fn = malloc_fn;
    custom_mr.realloc_fn = NULL;
    custom_mr.free_fn = free_fn;
    current_memory_resource = &custom_mr;
}

void json_set_alloc_funcs2(json_malloc_t malloc_fn, json_realloc_t realloc_fn,
                           json_free_t free_fn) {
    static memory_resource custom_mr;
    custom_mr.malloc_fn = malloc_fn;
    custom_mr.realloc_fn = realloc_fn;
    custom_mr.free_fn = free_fn;
    current_memory_resource = &custom_mr;
}

void json_get_alloc_funcs(json_malloc_t *malloc_fn, json_free_t *free_fn) {
    if (!current_memory_resource)
        current_memory_resource = &default_memory_resource;
    if (malloc_fn) {
        *malloc_fn = current_memory_resource->malloc_fn;
    }
    if (free_fn) {
        *free_fn = current_memory_resource->free_fn;
    }
}

void json_get_alloc_funcs2(json_malloc_t *malloc_fn, json_realloc_t *realloc_fn,
                           json_free_t *free_fn) {
    if (!current_memory_resource)
        current_memory_resource = &default_memory_resource;
    if (malloc_fn) {
        *malloc_fn = current_memory_resource->malloc_fn;
    }
    if (realloc_fn) {
        *realloc_fn = current_memory_resource->realloc_fn;
    }
    if (free_fn) {
        *free_fn = current_memory_resource->free_fn;
    }
}
