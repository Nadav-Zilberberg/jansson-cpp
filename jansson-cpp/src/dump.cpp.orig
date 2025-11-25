/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <clocale>
#include <cctype>
#include <cmath>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>

// Include the C++ header first to get the proper definitions
#include "jansson.hpp"

// Define the C-style enum that the private header expects
enum json_error_code {
    json_error_unknown,
    json_error_out_of_memory,
    json_error_stack_overflow,
    json_error_cannot_open_file,
    json_error_invalid_argument,
    json_error_invalid_utf8,
    json_error_premature_end_of_input,
    json_error_end_of_input_expected,
    json_error_invalid_syntax,
    json_error_invalid_format,
    json_error_wrong_type,
    json_error_null_character,
    json_error_null_value,
    json_error_null_byte_in_key,
    json_error_duplicate_key,
    json_error_numeric_overflow,
    json_error_item_not_found,
    json_error_index_out_of_range
};

// Then include the private header, but we'll need to work around the issues
#define json_t jansson::json_t
#define json_int_t jansson::json_int_t
#define json_error_t jansson::json_error_t
#include "jansson_private.hpp"
#undef json_t
#undef json_int_t
#undef json_error_t

#include "strbuffer.hpp"
#include "utf.hpp"

// Add missing includes
#ifndef _WIN32
#include <unistd.h>
#endif

namespace jansson {

namespace {

constexpr size_t MAX_INTEGER_STR_LENGTH = 100;
constexpr size_t MAX_REAL_STR_LENGTH = 100;

// Define the JSON type constants mapping to C++ enum values
constexpr json_type JSON_NULL = json_type::NULL_TYPE;
constexpr json_type JSON_TRUE = json_type::TRUE;
constexpr json_type JSON_FALSE = json_type::FALSE;
constexpr json_type JSON_INTEGER = json_type::INTEGER;
constexpr json_type JSON_REAL = json_type::REAL;
constexpr json_type JSON_STRING = json_type::STRING;
constexpr json_type JSON_ARRAY = json_type::ARRAY;
constexpr json_type JSON_OBJECT = json_type::OBJECT;

// Define JSON_INTEGER_FORMAT if not defined
#ifndef JSON_INTEGER_FORMAT
#define JSON_INTEGER_FORMAT "lld"
#endif

struct dump_buffer {
    char *buffer;
    size_t size;
    size_t used;
};

static int dump_to_strbuffer(const char *buffer, size_t size, void *data) {
    auto *strbuff = static_cast<strbuffer_t*>(data);
    return strbuff->append_bytes(buffer, size) ? 0 : -1;
}

static int dump_to_buffer(const char *buffer, size_t size, void *data) {
    auto *buf = static_cast<dump_buffer*>(data);
    
    if (buf->used + size >= buf->size) {
        size_t new_size = std::max(buf->size * 2, buf->used + size + 1);
        char *new_buffer = static_cast<char*>(jsonp_realloc(buf->buffer, buf->size, new_size));
        if (!new_buffer) return -1;
        buf->buffer = new_buffer;
        buf->size = new_size;
    }
    
    std::memcpy(buf->buffer + buf->used, buffer, size);
    buf->used += size;
    buf->buffer[buf->used] = '\0';
    return 0;
}

static int dump_to_file(const char *buffer, size_t size, void *data) {
    auto *output = static_cast<std::FILE*>(data);
    return std::fwrite(buffer, 1, size, output) == size ? 0 : -1;
}

static int dump_to_fd(const char *buffer, size_t size, void *data) {
    int *output = static_cast<int*>(data);
    
#ifdef _WIN32
    return _write(*output, buffer, static_cast<unsigned int>(size)) == static_cast<int>(size) ? 0 : -1;
#else
    return write(*output, buffer, size) == static_cast<ssize_t>(size) ? 0 : -1;
#endif
}

static int dump_indent(size_t flags, int depth, int space, json_dump_callback_t dump,
                       void *data) {
    if (flags & JSON_INDENT(0)) {
        if (dump("\n", 1, data))
            return -1;
        
        int n = flags & JSON_MAX_INDENT;
        std::string whitespace(n * depth, ' ');
        if (dump(whitespace.c_str(), n * depth, data))
            return -1;
    } else if (space && !(flags & JSON_COMPACT)) {
        return dump(" ", 1, data);
    }
    return 0;
}

static int dump_string(const char *str, size_t len, json_dump_callback_t dump, void *data,
                       size_t flags) {
    const char *pos = str;
    const char *end = pos + len;
    const char *str_begin = pos;
    
    if (dump("\"", 1, data))
        return -1;
    
    while (pos < end) {
        int32_t codepoint;
        size_t length = utf8_check_first(*pos);
        if (length == 0) return -1;
        
        if (pos + length > end) return -1;
        
        if (length == 1) {
            /* ASCII character */
            char escaped = 0;
            switch (*pos) {
                case '"': escaped = '"'; break;
                case '\\': escaped = '\\'; break;
                case '\b': escaped = 'b'; break;
                case '\f': escaped = 'f'; break;
                case '\n': escaped = 'n'; break;
                case '\r': escaped = 'r'; break;
                case '\t': escaped = 't'; break;
                default:
                    if ((flags & JSON_ESCAPE_SLASH) && *pos == '/')
                        escaped = '/';
                    else {
                        if ((flags & JSON_ENSURE_ASCII) && !std::isprint(*pos))
                            escaped = 'u';
                        else {
                            if (dump(pos, 1, data))
                                return -1;
                            pos++;
                            continue;
                        }
                    }
            }
            
            if (escaped) {
                const char *escaped_str = nullptr;
                size_t escaped_len = 0;
                
                if (escaped == 'u') {
                    /* Unicode escape */
                    char seq[7];
                    int seq_len = std::snprintf(seq, sizeof(seq), "\\u%04x", 
                                               static_cast<unsigned char>(*pos));
                    escaped_str = seq;
                    escaped_len = seq_len;
                } else {
                    /* Simple escape */
                    char seq[3] = "\\";
                    seq[1] = escaped;
                    escaped_str = seq;
                    escaped_len = 2;
                }
                
                if (pos > str_begin) {
                    if (dump(str_begin, pos - str_begin, data))
                        return -1;
                }
                
                if (dump(escaped_str, escaped_len, data))
                    return -1;
                
                str_begin = pos + 1;
            }
            
            pos++;
        } else {
            /* Non-ASCII character */
            if (utf8_check_full(pos, length, &codepoint)) {
                if ((flags & JSON_ENSURE_ASCII) && codepoint > 0x7F) {
                    if (pos > str_begin) {
                        if (dump(str_begin, pos - str_begin, data))
                            return -1;
                    }
                    
                    char seq[7];
                    int seq_len = std::snprintf(seq, sizeof(seq), "\\u%04x", codepoint);
                    if (dump(seq, seq_len, data))
                        return -1;
                    
                    str_begin = pos + length;
                }
                pos += length;
            } else
                return -1;
        }
    }
    
    if (pos > str_begin) {
        if (dump(str_begin, pos - str_begin, data))
            return -1;
    }
    
    return dump("\"", 1, data);
}

struct key_len {
    const char *key;
    size_t len;
};

static int compare_keys(const void *a, const void *b) {
    const auto *ka = static_cast<const key_len*>(a);
    const auto *kb = static_cast<const key_len*>(b);
    
    if (ka->len < kb->len) return -1;
    if (ka->len > kb->len) return 1;
    return std::memcmp(ka->key, kb->key, ka->len);
}

static int do_dump(const json_t *json, size_t flags, int depth, hashtable_t *parents,
                   json_dump_callback_t dump, void *data) {
    int embed = flags & JSON_EMBED;
    
    if (!json) return -1;
    
    switch (json_typeof(json)) {
        case JSON_NULL:
            return dump("null", 4, data);
            
        case JSON_TRUE:
            return dump("true", 4, data);
            
        case JSON_FALSE:
            return dump("false", 5, data);
            
        case JSON_INTEGER: {
            char buffer[MAX_INTEGER_STR_LENGTH];
            int size;
            
            size = std::snprintf(buffer, MAX_INTEGER_STR_LENGTH, "%" JSON_INTEGER_FORMAT,
                                json_integer_value(json));
            if (size < 0 || size >= MAX_INTEGER_STR_LENGTH)
                return -1;
            
            return dump(buffer, size, data);
        }
        
        case JSON_REAL: {
            char buffer[MAX_REAL_STR_LENGTH];
            int size;
            double value = json_real_value(json);
            
            size = jsonp_dtostr(buffer, MAX_REAL_STR_LENGTH, value,
                               JSON_REAL_PRECISION(flags));
            if (size < 0)
                return -1;
            
            return dump(buffer, size, data);
        }
        
        case JSON_STRING: {
            std::string str_value = json_string_value(json);
            return dump_string(str_value.c_str(), json_string_length(json), dump,
                              data, flags);
        }
            
        case JSON_ARRAY: {
            size_t n;
            size_t i;
            
            n = json_array_size(json);
            if (!embed && dump("[", 1, data))
                return -1;
            
            if (n == 0) {
                return embed ? 0 : dump("]", 1, data);
            }
            
            if (dump_indent(flags, depth + 1, 0, dump, data))
                return -1;
            
            for (i = 0; i < n; ++i) {
                if (do_dump(json_array_get(json, i), flags, depth + 1, parents, dump,
                           data))
                    return -1;
                
                if (i < n - 1) {
                    if (dump(",", 1, data) || dump_indent(flags, depth + 1, 1, dump, data))
                        return -1;
                } else {
                    if (dump_indent(flags, depth, 0, dump, data))
                        return -1;
                }
            }
            
            return embed ? 0 : dump("]", 1, data);
        }
        
        case JSON_OBJECT: {
            void *iter;
            char loop_key[256];
            size_t loop_key_len;
            
            std::snprintf(loop_key, sizeof(loop_key), "0x%p", json);
            loop_key_len = std::strlen(loop_key);
            
            if (jsonp_loop_check(parents, json, loop_key, sizeof(loop_key), &loop_key_len))
                return -1;
            
            if (!embed && dump("{", 1, data))
                return -1;
            
            iter = json_object_iter(const_cast<json_t*>(json));
            if (!iter) {
                hashtable_del(parents, loop_key, loop_key_len);
                return embed ? 0 : dump("}", 1, data);
            }
            
            if (flags & JSON_SORT_KEYS) {
                size_t size = json_object_size(json);
                std::vector<key_len> keys;
                keys.reserve(size);
                
                /* Collect keys */
                while (iter) {
                    key_len kl;
                    kl.key = json_object_iter_key(iter).c_str();
                    kl.len = json_object_iter_key_len(iter);
                    keys.push_back(kl);
                    iter = json_object_iter_next(const_cast<json_t*>(json), iter);
                }
                
                /* Sort keys */
                std::qsort(keys.data(), size, sizeof(key_len), compare_keys);
                
                /* Dump sorted keys */
                const char *separator = "";
                size_t separator_length = 0;
                
                for (size_t i = 0; i < size; ++i) {
                    json_t *value = json_object_getn(json, keys[i].key, keys[i].len);
                    
                    if (dump(separator, separator_length, data) ||
                        dump_string(keys[i].key, keys[i].len, dump, data, flags) ||
                        dump(flags & JSON_COMPACT ? ":" : ": ", 
                            flags & JSON_COMPACT ? 1 : 2, data) ||
                        do_dump(value, flags, depth + 1, parents, dump, data))
                        return -1;
                    
                    separator = ",";
                    separator_length = 1;
                    
                    if (dump_indent(flags, depth + 1, 1, dump, data))
                        return -1;
                }
            } else {
                const char *separator = "";
                size_t separator_length = 0;
                
                while (iter) {
                    std::string key = json_object_iter_key(iter);
                    size_t key_len = json_object_iter_key_len(iter);
                    json_t *value = json_object_iter_value(iter);
                    void *next = json_object_iter_next(const_cast<json_t*>(json), iter);
                    
                    if (dump(separator, separator_length, data) ||
                        dump_string(key.c_str(), key_len, dump, data, flags) ||
                        dump(flags & JSON_COMPACT ? ":" : ": ", 
                            flags & JSON_COMPACT ? 1 : 2, data) ||
                        do_dump(value, flags, depth + 1, parents, dump, data))
                        return -1;
                    
                    separator = ",";
                    separator_length = 1;
                    
                    if (next && dump_indent(flags, depth + 1, 1, dump, data))
                        return -1;
                    
                    iter = next;
                }
            }
            
            hashtable_del(parents, loop_key, loop_key_len);
            return embed ? 0 : dump("}", 1, data);
        }
        
        default:
            /* Not reached */
            return -1;
    }
}

} // anonymous namespace

std::string json_dumps(const json_t *json, size_t flags) {
    strbuffer_t strbuff;
    
    if (json_dump_callback(json, dump_to_strbuffer, &strbuff, flags))
        return "";
    
    return strbuff.str();
}

size_t json_dumpb(const json_t *json, char *buffer, size_t size, size_t flags) {
    dump_buffer buf = {buffer, size, 0};
    
    if (json_dump_callback(json, dump_to_buffer, &buf, flags))
        return 0;
    
    return buf.used;
}

int json_dumpf(const json_t *json, std::FILE *output, size_t flags) {
    return json_dump_callback(json, dump_to_file, output, flags);
}

int json_dumpfd(const json_t *json, int output, size_t flags) {
    return json_dump_callback(json, dump_to_fd, &output, flags);
}

int json_dump_file(const json_t *json, const std::string& path, size_t flags) {
    std::FILE *output = std::fopen(path.c_str(), "w");
    if (!output)
        return -1;
    
    int ret = json_dumpf(json, output, flags);
    std::fclose(output);
    return ret;
}

int json_dump_callback(const json_t *json, json_dump_callback_t callback, void *data, size_t flags) {
    if (!(flags & JSON_ENCODE_ANY)) {
        if (!json_is_array(json) && !json_is_object(json))
            return -1;
    }
    
    hashtable_t parents_set;
    int ret = -1;
    
    if (hashtable_init(&parents_set) == 0) {
        ret = do_dump(json, flags, 0, &parents_set, callback, data);
        hashtable_close(&parents_set);
    }
    
    return ret;
}

} // namespace jansson
