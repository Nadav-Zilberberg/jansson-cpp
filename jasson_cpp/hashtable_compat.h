/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#ifndef JANSSON_HASHTABLE_COMPAT_H
#define JANSSON_HASHTABLE_COMPAT_H

#include "../src/hashtable.h"
#include "hashtable_simple.hpp"

#ifdef __cplusplus
extern "C" {
#endif

// Compatibility wrapper that allows C code to use the C++ hashtable
struct hashtable_wrapper {
    jansson::HashTable* cpp_table;
};

// Initialize a hashtable wrapper
int hashtable_wrapper_init(struct hashtable_wrapper* wrapper);

// Close a hashtable wrapper
void hashtable_wrapper_close(struct hashtable_wrapper* wrapper);

// Get the underlying C++ hashtable
jansson::HashTable* hashtable_wrapper_get_cpp_table(struct hashtable_wrapper* wrapper);

#ifdef __cplusplus
}
#endif

#endif // JANSSON_HASHTABLE_COMPAT_H
