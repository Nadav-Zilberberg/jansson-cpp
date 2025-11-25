/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#ifndef JANSSON_HASHTABLE_WRAPPER_H
#define JANSSON_HASHTABLE_WRAPPER_H

#include "jansson.h"

#ifdef __cplusplus
extern "C" {
#endif

// Forward declaration to avoid including the original hashtable.h
typedef struct hashtable_t hashtable_t;

// Wrapper functions that provide C API compatibility
hashtable_t* hashtable_cpp_create(void);
void hashtable_cpp_destroy(hashtable_t* hashtable);
int hashtable_cpp_set(hashtable_t* hashtable, const char* key, size_t key_len, json_t* value);
json_t* hashtable_cpp_get(hashtable_t* hashtable, const char* key, size_t key_len);
int hashtable_cpp_del(hashtable_t* hashtable, const char* key, size_t key_len);
void hashtable_cpp_clear(hashtable_t* hashtable);
size_t hashtable_cpp_size(const hashtable_t* hashtable);
int hashtable_cpp_empty(const hashtable_t* hashtable);

#ifdef __cplusplus
}
#endif

#endif // JANSSON_HASHTABLE_WRAPPER_H
