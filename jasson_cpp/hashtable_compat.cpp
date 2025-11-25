/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include "hashtable_compat.h"
#include <new>

extern "C" {

int hashtable_wrapper_init(struct hashtable_wrapper* wrapper) {
    if (!wrapper) {
        return -1;
    }
    
    try {
        wrapper->cpp_table = new (std::nothrow) jansson::HashTable();
        if (!wrapper->cpp_table) {
            return -1;
        }
        return 0;
    } catch (...) {
        return -1;
    }
}

void hashtable_wrapper_close(struct hashtable_wrapper* wrapper) {
    if (wrapper && wrapper->cpp_table) {
        delete wrapper->cpp_table;
        wrapper->cpp_table = nullptr;
    }
}

jansson::HashTable* hashtable_wrapper_get_cpp_table(struct hashtable_wrapper* wrapper) {
    return wrapper ? wrapper->cpp_table : nullptr;
}

} // extern "C"
