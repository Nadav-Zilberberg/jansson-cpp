/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#ifndef STRINGBUFFER_H
#define STRINGBUFFER_H

#include <string>
#include <utility>

class StringBuffer {
private:
    std::string buffer;

public:
    // Constructor
    StringBuffer() = default;
    
    // Destructor
    ~StringBuffer() = default;
    
    // Copy constructor
    StringBuffer(const StringBuffer& other) = default;
    
    // Move constructor
    StringBuffer(StringBuffer&& other) noexcept = default;
    
    // Copy assignment
    StringBuffer& operator=(const StringBuffer& other) = default;
    
    // Move assignment
    StringBuffer& operator=(StringBuffer&& other) noexcept = default;
    
    // Initialize the buffer (for compatibility)
    int init() { return 0; }
    
    // Clear the buffer
    void clear() { buffer.clear(); }
    
    // Get the current value as C string
    const char* value() const { return buffer.c_str(); }
    
    // Get the current length
    size_t length() const { return buffer.length(); }
    
    // Get the current capacity
    size_t capacity() const { return buffer.capacity(); }
    
    // Append a single byte
    int append_byte(char byte) {
        buffer.push_back(byte);
        return 0;
    }
    
    // Append multiple bytes
    int append_bytes(const char* data, size_t size) {
        buffer.append(data, size);
        return 0;
    }
    
    // Pop the last byte
    char pop() {
        if (buffer.empty()) {
            return '\0';
        }
        char last = buffer.back();
        buffer.pop_back();
        return last;
    }
    
    // Steal the value (transfer ownership)
    char* steal_value() {
        char* result = new char[buffer.length() + 1];
        std::copy(buffer.begin(), buffer.end(), result);
        result[buffer.length()] = '\0';
        buffer.clear();
        return result;
    }
    
    // Close/cleanup (for compatibility)
    void close() { buffer.clear(); }
};

#endif /* STRINGBUFFER_H */
