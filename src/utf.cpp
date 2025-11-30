/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include "utf.h"
#include <algorithm>
#include <stdexcept>
#include <cstdint>

// UTF-8 encoding table
static const struct {
    uint32_t max_codepoint;
    uint8_t mask;
    uint8_t lead_byte_mask;
    size_t sequence_length;
} utf8_table[] = {
    {0x7F, 0x7F, 0x00, 1},      // 0xxxxxxx
    {0x7FF, 0x1F, 0xC0, 2},     // 110xxxxx 10xxxxxx
    {0xFFFF, 0x0F, 0xE0, 3},    // 1110xxxx 10xxxxxx 10xxxxxx
    {0x10FFFF, 0x07, 0xF0, 4},  // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
};

std::optional<std::string> utf8_encode(CodePoint codepoint) {
    if (codepoint > 0x10FFFF) {
        return std::nullopt;  // Beyond Unicode range
    }
    
    if (codepoint >= 0xD800 && codepoint <= 0xDFFF) {
        return std::nullopt;  // UTF-16 surrogate halves
    }
    
    std::string result;
    result.reserve(4);
    
    for (const auto& entry : utf8_table) {
        if (codepoint <= entry.max_codepoint) {
            if (entry.sequence_length == 1) {
                result.push_back(static_cast<char>(codepoint));
            } else {
                // Lead byte
                result.push_back(static_cast<char>(
                    entry.lead_byte_mask | (codepoint >> (6 * (entry.sequence_length - 1)))
                ));
                
                // Continuation bytes
                for (size_t i = 1; i < entry.sequence_length; ++i) {
                    result.push_back(static_cast<char>(
                        0x80 | ((codepoint >> (6 * (entry.sequence_length - 1 - i))) & 0x3F)
                    ));
                }
            }
            return result;
        }
    }
    
    return std::nullopt;
}

size_t utf8_sequence_length(char first_byte) {
    uint8_t u = static_cast<uint8_t>(first_byte);
    
    if (u < 0x80) return 1;
    if (u < 0xC0) return 0;  // Continuation byte
    if (u < 0xE0) return 2;
    if (u < 0xF0) return 3;
    if (u < 0xF8) return 4;
    return 0;  // Invalid
}

std::optional<size_t> utf8_validate_sequence(std::string_view str, size_t pos) {
    if (pos >= str.size()) {
        return std::nullopt;
    }
    
    size_t length = utf8_sequence_length(str[pos]);
    if (length == 0) {
        return std::nullopt;
    }
    
    if (pos + length > str.size()) {
        return std::nullopt;
    }
    
    // Check continuation bytes
    for (size_t i = 1; i < length; ++i) {
        uint8_t byte = static_cast<uint8_t>(str[pos + i]);
        if (byte < 0x80 || byte > 0xBF) {
            return std::nullopt;
        }
    }
    
    // Check for overlong encoding
    if (length > 1) {
        uint8_t first_byte = static_cast<uint8_t>(str[pos]);
        if (first_byte == 0xC0 || first_byte == 0xC1) {
            return std::nullopt;
        }
        
        // Extract codepoint
        CodePoint codepoint = 0;
        if (length == 2) {
            codepoint = first_byte & 0x1F;
        } else if (length == 3) {
            codepoint = first_byte & 0x0F;
        } else if (length == 4) {
            codepoint = first_byte & 0x07;
        }
        
        for (size_t i = 1; i < length; ++i) {
            codepoint = (codepoint << 6) | (str[pos + i] & 0x3F);
        }
        
        // Check codepoint validity
        if (codepoint > 0x10FFFF) {
            return std::nullopt;
        }
        
        if (codepoint >= 0xD800 && codepoint <= 0xDFFF) {
            return std::nullopt;
        }
        
        // Check for overlong encoding
        if ((length == 2 && codepoint < 0x80) ||
            (length == 3 && codepoint < 0x800) ||
            (length == 4 && codepoint < 0x10000)) {
            return std::nullopt;
        }
    }
    
    return length;
}

std::optional<CodePoint> utf8_decode(std::string_view str, size_t& pos) {
    auto length = utf8_validate_sequence(str, pos);
    if (!length) {
        return std::nullopt;
    }
    
    if (*length == 1) {
        pos += 1;
        return static_cast<uint8_t>(str[pos - 1]);
    }
    
    CodePoint codepoint = 0;
    uint8_t first_byte = static_cast<uint8_t>(str[pos]);
    
    if (*length == 2) {
        codepoint = first_byte & 0x1F;
    } else if (*length == 3) {
        codepoint = first_byte & 0x0F;
    } else if (*length == 4) {
        codepoint = first_byte & 0x07;
    }
    
    for (size_t i = 1; i < *length; ++i) {
        codepoint = (codepoint << 6) | (str[pos + i] & 0x3F);
    }
    
    pos += *length;
    return codepoint;
}

std::optional<CodePoint> utf8_decode(std::string_view str) {
    size_t pos = 0;
    return utf8_decode(str, pos);
}

bool utf8_is_valid(std::string_view str) {
    size_t pos = 0;
    while (pos < str.size()) {
        auto length = utf8_validate_sequence(str, pos);
        if (!length) {
            return false;
        }
        pos += *length;
    }
    return true;
}

std::optional<CodePoint> utf8_iterate(std::string_view str, size_t& pos) {
    return utf8_decode(str, pos);
}

std::optional<Utf8String> utf8_make_valid(std::string_view str) {
    if (!utf8_is_valid(str)) {
        return std::nullopt;
    }
    return Utf8String(str);
}

Utf8String::Utf8String(std::string_view str) {
    str_ = str;
    valid_ = utf8_is_valid(str);
}

Utf8String::iterator Utf8String::begin() const {
    return iterator(str_, 0);
}

Utf8String::iterator Utf8String::end() const {
    return iterator(str_, str_.size());
}

Utf8String::iterator::iterator(std::string_view str, size_t pos)
    : str_(str), pos_(pos) {
    if (pos < str.size()) {
        size_t temp_pos = pos;
        auto cp = utf8_decode(str, temp_pos);
        if (cp) {
            codepoint_ = *cp;
        }
    }
}

Utf8String::iterator& Utf8String::iterator::operator++() {
    if (pos_ < str_.size()) {
        size_t temp_pos = pos_;
        auto length = utf8_validate_sequence(str_, temp_pos);
        if (length) {
            pos_ += *length;
            if (pos_ < str_.size()) {
                temp_pos = pos_;
                auto cp = utf8_decode(str_, temp_pos);
                if (cp) {
                    codepoint_ = *cp;
                }
            }
        } else {
            pos_ = str_.size();
        }
    }
    return *this;
}

Utf8String::iterator Utf8String::iterator::operator++(int) {
    iterator temp = *this;
    ++(*this);
    return temp;
}
