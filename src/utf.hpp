/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#ifndef UTF_H
#define UTF_H

#include <string_view>
#include <optional>
#include <string>
#include <vector>
#include <cstdint>

// Unicode code point type
using CodePoint = uint32_t;

// Validated UTF-8 string class
class Utf8String {
public:
    // Constructors
    Utf8String() = default;
    explicit Utf8String(std::string_view str);
    explicit Utf8String(const std::string& str) : Utf8String(std::string_view(str)) {}
    explicit Utf8String(const char* str) : Utf8String(std::string_view(str)) {}
    
    // Getters
    const std::string& str() const { return str_; }
    std::string_view view() const { return str_; }
    const char* c_str() const { return str_.c_str(); }
    size_t size() const { return str_.size(); }
    bool empty() const { return str_.empty(); }
    
    // Iteration
    class iterator;
    iterator begin() const;
    iterator end() const;
    
    // Validation
    bool is_valid() const { return valid_; }
    explicit operator bool() const { return valid_; }
    
    // Comparison
    bool operator==(const Utf8String& other) const { return str_ == other.str_; }
    bool operator!=(const Utf8String& other) const { return str_ != other.str_; }
    bool operator<(const Utf8String& other) const { return str_ < other.str_; }
    
private:
    std::string str_;
    bool valid_ = false;
};

// Iterator for UTF-8 code points
class Utf8String::iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = CodePoint;
    using difference_type = std::ptrdiff_t;
    using pointer = const CodePoint*;
    using reference = const CodePoint&;
    
    iterator() = default;
    iterator(std::string_view str, size_t pos);
    
    value_type operator*() const { return codepoint_; }
    iterator& operator++();
    iterator operator++(int);
    
    bool operator==(const iterator& other) const {
        return pos_ == other.pos_ && str_ == other.str_;
    }
    bool operator!=(const iterator& other) const {
        return !(*this == other);
    }
    
private:
    std::string_view str_;
    size_t pos_ = 0;
    CodePoint codepoint_ = 0;
};

// UTF-8 encoding/decoding functions
std::optional<std::string> utf8_encode(CodePoint codepoint);
std::optional<CodePoint> utf8_decode(std::string_view str, size_t& pos);
std::optional<CodePoint> utf8_decode(std::string_view str);

// UTF-8 validation functions
bool utf8_is_valid(std::string_view str);
size_t utf8_sequence_length(char first_byte);
std::optional<size_t> utf8_validate_sequence(std::string_view str, size_t pos);

// UTF-8 iteration
std::optional<CodePoint> utf8_iterate(std::string_view str, size_t& pos);

// Convert string to Utf8String (validates)
std::optional<Utf8String> utf8_make_valid(std::string_view str);

#endif
