/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include "hashtable_wrapper.h"
#include "hashtable_simple.hpp"
#include <new>

extern "C" {

hashtable_t* hashtable_cpp_create(void) {
    try {
        jansson::HashTable* cpp_table = new (std::nothrow) jansson::HashTable();
        if (!cpp_table) {
            return nullptr;
        }
        return reinterpret_cast<hashtable_t*>(cpp_table);
    } catch (...) {
        return nullptr;
    }
}

void hashtable_cpp_destroy(hashtable_t* hashtable) {
    if (hashtable) {
        jansson::HashTable* cpp_table = reinterpret_cast<jansson::HashTable*>(hashtable);
        delete cpp_table;
    }
}

int hashtable_cpp_set(hashtable_t* hashtable, const char* key, size_t key_len, json_t* value) {
    if (!hashtable) {
        return -1;
    }
    jansson::HashTable* cpp_table = reinterpret_cast<jansson::HashTable*>(hashtable);
    return cpp_table->set(key, key_len, value);
}

json_t* hashtable_cpp_get(hashtable_t* hashtable, const char* key, size_t key_len) {
    if (!hashtable) {
        return nullptr;
    }
    jansson::HashTable* cpp_table = reinterpret_cast<jansson::HashTable*>(hashtable);
    return cpp_table->get(key, key_len);
}

int hashtable_cpp_del(hashtable_t* hashtable, const char* key, size_t key_len) {
    if (!hashtable) {
        return -1;
    }
    jansson::HashTable* cpp_table = reinterpret_cast<jansson::HashTable*>(hashtable);
    return cpp_table->del(key, key_len);
}

void hashtable_cpp_clear(hashtable_t* hashtable) {
    if (hashtable) {
        jansson::HashTable* cpp_table = reinterpret_cast<jansson::HashTable*>(hashtable);
        cpp_table->clear();
    }
}

size_t hashtable_cpp_size(const hashtable_t* hashtable) {
    if (!hashtable) {
        return 0;
    }
    const jansson::HashTable* cpp_table = reinterpret_cast<const jansson::HashTable*>(hashtable);
    return cpp_table->size();
}

int hashtable_cpp_empty(const hashtable_t* hashtable) {
    if (!hashtable) {
        return 1;
    }
    const jansson::HashTable* cpp_table = reinterpret_cast<const jansson::HashTable*>(hashtable);
    return cpp_table->empty() ? 1 : 0;
}

} // extern "C"
