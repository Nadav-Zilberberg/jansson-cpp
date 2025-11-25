/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include "StringBuffer.h"
#include "strbuffer_compat.h"
#include <cstring>
#include <new>

extern "C" {

int strbuffer_init(strbuffer_t *strbuff) {
    if (!strbuff) return -1;
    
    // Create the C++ StringBuffer object
    StringBuffer *cpp_buffer = new(std::nothrow) StringBuffer();
    if (!cpp_buffer) return -1;
    
    // Initialize the C struct fields
    strbuff->value = nullptr;
    strbuff->length = 0;
    strbuff->size = 0;
    strbuff->cpp_buffer = cpp_buffer;
    
    return 0;
}

void strbuffer_close(strbuffer_t *strbuff) {
    if (strbuff && strbuff->cpp_buffer) {
        delete static_cast<StringBuffer*>(strbuff->cpp_buffer);
        strbuff->cpp_buffer = nullptr;
        strbuff->value = nullptr;
        strbuff->length = 0;
        strbuff->size = 0;
    }
}

void strbuffer_clear(strbuffer_t *strbuff) {
    if (strbuff && strbuff->cpp_buffer) {
        static_cast<StringBuffer*>(strbuff->cpp_buffer)->clear();
        // After clear, the C++ string is empty but still has a valid c_str()
        strbuff->value = const_cast<char*>(static_cast<StringBuffer*>(strbuff->cpp_buffer)->value());
        strbuff->length = 0;
        strbuff->size = static_cast<StringBuffer*>(strbuff->cpp_buffer)->capacity();
    }
}

const char *strbuffer_value(const strbuffer_t *strbuff) {
    if (!strbuff || !strbuff->cpp_buffer) return nullptr;
    
    const StringBuffer *cpp_buffer = static_cast<const StringBuffer*>(strbuff->cpp_buffer);
    // Don't update the struct fields in const function, just return the value
    return cpp_buffer->value();
}

char *strbuffer_steal_value(strbuffer_t *strbuff) {
    if (!strbuff || !strbuff->cpp_buffer) return nullptr;
    
    StringBuffer *cpp_buffer = static_cast<StringBuffer*>(strbuff->cpp_buffer);
    char *result = cpp_buffer->steal_value();
    
    // Update the C struct fields
    strbuff->value = nullptr;
    strbuff->length = 0;
    strbuff->size = 0;
    
    return result;
}

int strbuffer_append_byte(strbuffer_t *strbuff, char byte) {
    if (!strbuff || !strbuff->cpp_buffer) return -1;
    
    StringBuffer *cpp_buffer = static_cast<StringBuffer*>(strbuff->cpp_buffer);
    int result = cpp_buffer->append_byte(byte);
    
    // Update the C struct fields
    strbuff->value = const_cast<char*>(cpp_buffer->value());
    strbuff->length = cpp_buffer->length();
    strbuff->size = cpp_buffer->capacity();
    
    return result;
}

int strbuffer_append_bytes(strbuffer_t *strbuff, const char *data, size_t size) {
    if (!strbuff || !strbuff->cpp_buffer || !data) return -1;
    
    StringBuffer *cpp_buffer = static_cast<StringBuffer*>(strbuff->cpp_buffer);
    int result = cpp_buffer->append_bytes(data, size);
    
    // Update the C struct fields
    strbuff->value = const_cast<char*>(cpp_buffer->value());
    strbuff->length = cpp_buffer->length();
    strbuff->size = cpp_buffer->capacity();
    
    return result;
}

char strbuffer_pop(strbuffer_t *strbuff) {
    if (!strbuff || !strbuff->cpp_buffer) return '\0';
    
    StringBuffer *cpp_buffer = static_cast<StringBuffer*>(strbuff->cpp_buffer);
    char result = cpp_buffer->pop();
    
    // Update the C struct fields
    strbuff->value = const_cast<char*>(cpp_buffer->value());
    strbuff->length = cpp_buffer->length();
    strbuff->size = cpp_buffer->capacity();
    
    return result;
}

} // extern "C"
