/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "jansson.hpp"
#include "jansson_private.hpp"

#include <cassert>
#include <cerrno>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include <functional>

#include "strbuffer.hpp"
#include "utf.hpp"

#define STREAM_STATE_OK    0
#define STREAM_STATE_EOF   -1
#define STREAM_STATE_ERROR -2

#define TOKEN_INVALID -1
#define TOKEN_EOF     0
#define TOKEN_STRING  256
#define TOKEN_INTEGER 257
#define TOKEN_REAL    258
#define TOKEN_TRUE    259
#define TOKEN_FALSE   260
#define TOKEN_NULL    261

/* Locale independent versions of isxxx() functions */
constexpr bool l_isupper(char c) { return 'A' <= c && c <= 'Z'; }
constexpr bool l_islower(char c) { return 'a' <= c && c <= 'z'; }
constexpr bool l_isalpha(char c) { return l_isupper(c) || l_islower(c); }
constexpr bool l_isdigit(char c) { return '0' <= c && c <= '9'; }
constexpr bool l_isxdigit(char c) {
    return l_isdigit(c) || ('A' <= c && c <= 'F') || ('a' <= c && c <= 'f');
}

namespace jansson {
namespace load {

// Forward declarations
class Stream;
class Lexer;

// Stream class to replace stream_t
class Stream {
public:
    using get_func = std::function<int()>;
    
    Stream(get_func get_func) : get_func_(get_func) {
        buffer_pos_ = 0;
        state_ = STREAM_STATE_OK;
        line_ = 1;
        column_ = 0;
        last_column_ = 0;
        position_ = 0;
        std::fill(buffer_, buffer_ + 5, '\0');
    }
    
    int get(jansson::json_error_t* error);
    void unget(int c);
    
    int state() const { return state_; }
    int line() const { return line_; }
    int column() const { return column_; }
    size_t position() const { return position_; }
    
private:
    get_func get_func_;
    char buffer_[5];
    size_t buffer_pos_;
    int state_;
    int line_;
    int column_;
    int last_column_;
    size_t position_;
};

// Lexer class to replace lex_t
class Lexer {
public:
    Lexer(std::function<int()> get_func, size_t flags);
    ~Lexer();
    
    int get_token(jansson::json_error_t* error);
    std::string steal_string();
    
    const Stream& stream() const { return stream_; }
    size_t depth() const { return depth_; }
    void set_depth(size_t depth) { depth_ = depth; }
    
    int token() const { return token_; }
    json_int_t integer_value() const { return integer_value_; }
    double real_value() const { return real_value_; }
    const std::string& string_value() const { return string_value_; }
    
private:
    Stream stream_;
    strbuffer saved_text_;
    size_t flags_;
    size_t depth_;
    int token_;
    
    // Token values
    std::string string_value_;
    json_int_t integer_value_;
    double real_value_;
    
    // Helper methods
    int lex_get_save(jansson::json_error_t* error);
    void lex_save_cached();
    void lex_scan_string(jansson::json_error_t* error);
    int lex_scan_number(int c, jansson::json_error_t* error);
    int32_t decode_unicode_escape(const std::string& str);
};

// Error handling
} // namespace load
} // namespace jansson

using namespace jansson::load;

int Stream::get(jansson::json_error_t* error) {
    int c;
    
    if (state_ != STREAM_STATE_OK)
        return EOF;
    
    if (buffer_pos_ > 0) {
        c = buffer_[--buffer_pos_];
    } else {
        c = get_func_();
    }
    
    if (c == '\n') {
        line_++;
        last_column_ = column_;
        column_ = 0;
    } else if (c == '\r') {
        column_ = 0;
    } else if (c != EOF) {
        column_++;
    }
    
    if (c != EOF) {
        position_++;
    } else {
        state_ = STREAM_STATE_EOF;
    }
    
    return c;
}

void Stream::unget(int c) {
    if (c == EOF || state_ != STREAM_STATE_OK)
        return;
    
    if (c == '\n') {
        if (line_ > 1) {
            line_--;
            column_ = last_column_;
        }
    } else if (c != '\r') {
        if (column_ > 0)
            column_--;
    }
    
    if (buffer_pos_ < sizeof(buffer_) / sizeof(buffer_[0])) {
        buffer_[buffer_pos_++] = c;
        position_--;
    } else {
        state_ = STREAM_STATE_ERROR;
    }
}

Lexer::Lexer(std::function<int()> get_func, size_t flags) 
    : stream_(get_func), flags_(flags), depth_(0), token_(TOKEN_INVALID) {
    saved_text_.clear();
}

Lexer::~Lexer() = default;

void error_set(jansson::json_error_t* error, const Lexer* lex, jansson::json_error_code code,
               const char* msg, ...) {
    va_list ap;
    char msg_text[jansson::JSON_ERROR_TEXT_LENGTH];
    char msg_with_context[jansson::JSON_ERROR_TEXT_LENGTH];
    
    int line = -1, col = -1;
    size_t pos = 0;
    const char* result = msg_text;
    
    if (!error)
        return;
    
    va_start(ap, msg);
    std::vsnprintf(msg_text, jansson::JSON_ERROR_TEXT_LENGTH, msg, ap);
    msg_text[jansson::JSON_ERROR_TEXT_LENGTH - 1] = '\0';
    va_end(ap);
    
    if (lex) {
        line = lex->stream().line();
        col = lex->stream().column();
        pos = lex->stream().position();
        
        std::snprintf(msg_with_context, jansson::JSON_ERROR_TEXT_LENGTH, "%s at line %d, column %d",
                     msg_text, line, col);
        msg_with_context[jansson::JSON_ERROR_TEXT_LENGTH - 1] = '\0';
        result = msg_with_context;
    }
    
    std::strncpy(error->text, result, jansson::JSON_ERROR_TEXT_LENGTH);
    error->text[jansson::JSON_ERROR_TEXT_LENGTH - 1] = static_cast<char>(code);
    
    if (lex) {
        error->line = line;
        error->column = col;
        error->position = pos;
    } else {
        error->line = -1;
        error->column = -1;
        error->position = 0;
    }
}

int Lexer::lex_get_save(jansson::json_error_t* error) {
    int c = stream_.get(error);
    if (c != EOF)
        saved_text_.append_byte(c);
    return c;
}

void Lexer::lex_save_cached() {
    if (saved_text_.length() == 0)
        return;
    
    for (size_t i = saved_text_.length(); i > 0; i--) {
        stream_.unget(saved_text_.value()[i - 1]);
    }
    saved_text_.clear();
}

int32_t Lexer::decode_unicode_escape(const std::string& str) {
    int32_t value = 0;
    for (size_t i = 0; i < 4; i++) {
        int digit;
        char c = str[i];
        if (l_isdigit(c))
            digit = c - '0';
        else if (c >= 'A' && c <= 'F')
            digit = c - 'A' + 10;
        else if (c >= 'a' && c <= 'f')
            digit = c - 'a' + 10;
        else
            return -1;
        
        value = value * 16 + digit;
    }
    return value;
}

void Lexer::lex_scan_string(jansson::json_error_t* error) {
    int c;
    std::string str;
    
    saved_text_.clear();
    
    c = stream_.get(error);
    if (c != '"') {
        error_set(error, this, jansson::json_error_code::invalid_syntax,
                  "expected string but got %c", c);
        return;
    }
    
    while (true) {
        c = stream_.get(error);
        if (c == EOF || c == '"')
            break;
        
        if (c == '\\') {
            c = stream_.get(error);
            if (c == EOF)
                break;
            
            switch (c) {
            case 'b':
                str += '\b';
                break;
            case 'f':
                str += '\f';
                break;
            case 'n':
                str += '\n';
                break;
            case 'r':
                str += '\r';
                break;
            case 't':
                str += '\t';
                break;
            case '"':
            case '\\':
            case '/':
                str += c;
                break;
            case 'u': {
                std::string hex;
                for (int i = 0; i < 4; i++) {
                    c = stream_.get(error);
                    if (c == EOF || !l_isxdigit(c)) {
                        error_set(error, this, jansson::json_error_code::invalid_syntax,
                                  "invalid Unicode escape");
                        return;
                    }
                    hex += c;
                }
                
                int32_t value = decode_unicode_escape(hex);
                if (value < 0) {
                    error_set(error, this, jansson::json_error_code::invalid_syntax,
                              "invalid Unicode escape");
                    return;
                }
                
                // Convert Unicode code point to UTF-8
                if (value <= 0x7F) {
                    str += static_cast<char>(value);
                } else if (value <= 0x7FF) {
                    str += static_cast<char>(0xC0 | (value >> 6));
                    str += static_cast<char>(0x80 | (value & 0x3F));
                } else {
                    str += static_cast<char>(0xE0 | (value >> 12));
                    str += static_cast<char>(0x80 | ((value >> 6) & 0x3F));
                    str += static_cast<char>(0x80 | (value & 0x3F));
                }
                break;
            }
            default:
                error_set(error, this, jansson::json_error_code::invalid_syntax,
                          "invalid escape sequence");
                return;
            }
        } else {
            str += c;
        }
    }
    
    if (c == EOF) {
        error_set(error, this, jansson::json_error_code::invalid_syntax,
                  "unexpected end of string");
        return;
    }
    
    string_value_ = str;
    token_ = TOKEN_STRING;
}

int Lexer::lex_scan_number(int c, jansson::json_error_t* error) {
    std::string num_str;
    bool is_float = false;
    
    if (c == '-') {
        num_str += c;
        c = stream_.get(error);
        if (c == EOF) {
            error_set(error, this, jansson::json_error_code::invalid_syntax,
                      "unexpected end of number");
            return TOKEN_INVALID;
        }
    }
    
    if (c == '0') {
        num_str += c;
        c = stream_.get(error);
    } else if (l_isdigit(c)) {
        while (l_isdigit(c)) {
            num_str += c;
            c = stream_.get(error);
        }
    } else {
        error_set(error, this, jansson::json_error_code::invalid_syntax,
                  "invalid number");
        return TOKEN_INVALID;
    }
    
    if (c == '.' && l_isdigit(stream_.get(error))) {
        is_float = true;
        num_str += '.';
        c = stream_.get(error);
        while (l_isdigit(c)) {
            num_str += c;
            c = stream_.get(error);
        }
    }
    
    if (c == 'e' || c == 'E') {
        is_float = true;
        num_str += c;
        c = stream_.get(error);
        if (c == '+' || c == '-') {
            num_str += c;
            c = stream_.get(error);
        }
        if (!l_isdigit(c)) {
            error_set(error, this, jansson::json_error_code::invalid_syntax,
                      "invalid exponent");
            return TOKEN_INVALID;
        }
        while (l_isdigit(c)) {
            num_str += c;
            c = stream_.get(error);
        }
    }
    
    stream_.unget(c);
    
    if (is_float) {
        try {
            real_value_ = std::stod(num_str);
            token_ = TOKEN_REAL;
            return TOKEN_REAL;
        } catch (const std::exception&) {
            error_set(error, this, jansson::json_error_code::invalid_syntax,
                      "invalid floating point number");
            return TOKEN_INVALID;
        }
    } else {
        try {
            integer_value_ = std::stoll(num_str);
            token_ = TOKEN_INTEGER;
            return TOKEN_INTEGER;
        } catch (const std::exception&) {
            error_set(error, this, jansson::json_error_code::invalid_syntax,
                      "invalid integer");
            return TOKEN_INVALID;
        }
    }
}

int Lexer::get_token(jansson::json_error_t* error) {
    int c;
    
    while (true) {
        c = stream_.get(error);
        
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
            continue;
        
        if (c == EOF) {
            token_ = TOKEN_EOF;
            return TOKEN_EOF;
        }
        
        break;
    }
    
    saved_text_.clear();
    
    switch (c) {
    case '{':
    case '}':
    case '[':
    case ']':
    case ':':
    case ',':
        return c;
        
    case '"':
        stream_.unget(c);
        lex_scan_string(error);
        return token_;
        
    case 't':
        if (stream_.get(error) == 'r' && stream_.get(error) == 'u' && 
            stream_.get(error) == 'e') {
            token_ = TOKEN_TRUE;
            return TOKEN_TRUE;
        }
        error_set(error, this, jansson::json_error_code::invalid_syntax,
                  "invalid literal");
        return TOKEN_INVALID;
        
    case 'f':
        if (stream_.get(error) == 'a' && stream_.get(error) == 'l' && 
            stream_.get(error) == 's' && stream_.get(error) == 'e') {
            token_ = TOKEN_FALSE;
            return TOKEN_FALSE;
        }
        error_set(error, this, jansson::json_error_code::invalid_syntax,
                  "invalid literal");
        return TOKEN_INVALID;
        
    case 'n':
        if (stream_.get(error) == 'u' && stream_.get(error) == 'l' && 
            stream_.get(error) == 'l') {
            token_ = TOKEN_NULL;
            return TOKEN_NULL;
        }
        error_set(error, this, jansson::json_error_code::invalid_syntax,
                  "invalid literal");
        return TOKEN_INVALID;
        
    default:
        if (c == '-' || l_isdigit(c)) {
            stream_.unget(c);
            return lex_scan_number(c, error);
        }
        error_set(error, this, jansson::json_error_code::invalid_syntax,
                  "unexpected character");
        return TOKEN_INVALID;
    }
}

std::string Lexer::steal_string() {
    std::string result = std::move(string_value_);
    string_value_.clear();
    return result;
}

// Forward declarations for parsing functions
static jansson::json_t* parse_value(Lexer& lex, size_t flags, jansson::json_error_t* error);
static jansson::json_t* parse_object(Lexer& lex, size_t flags, jansson::json_error_t* error);
static jansson::json_t* parse_array(Lexer& lex, size_t flags, jansson::json_error_t* error);

static jansson::json_t* parse_json(Lexer& lex, size_t flags, jansson::json_error_t* error) {
    jansson::json_t* result;
    
    lex.set_depth(0);
    
    if (lex.get_token(error) != TOKEN_INVALID) {
        result = parse_value(lex, flags, error);
        if (result && (flags & jansson::JSON_DECODE_ANY) == 0 && lex.token() != TOKEN_EOF) {
            error_set(error, &lex, jansson::json_error_code::end_of_input_expected,
                      "end of file expected");
            jansson::json_decref(result);
            return nullptr;
        }
        return result;
    }
    
    return nullptr;
}

static jansson::json_t* parse_value(Lexer& lex, size_t flags, jansson::json_error_t* error) {
    int token = lex.token();
    
    switch (token) {
    case TOKEN_STRING: {
        std::string str = lex.steal_string();
        return jansson::json_string(str.c_str());
    }
    
    case TOKEN_INTEGER:
        return jansson::json_integer(lex.integer_value());
        
    case TOKEN_REAL:
        return jansson::json_real(lex.real_value());
        
    case TOKEN_TRUE:
        return jansson::json_true();
        
    case TOKEN_FALSE:
        return jansson::json_false();
        
    case TOKEN_NULL:
        return jansson::json_null();
        
    case '{':
        return parse_object(lex, flags, error);
        
    case '[':
        return parse_array(lex, flags, error);
        
    default:
        error_set(error, &lex, jansson::json_error_code::invalid_syntax,
                  "unexpected token");
        return nullptr;
    }
}

static jansson::json_t* parse_object(Lexer& lex, size_t flags, jansson::json_error_t* error) {
    jansson::json_t* object = jansson::json_object();
    int token;
    
    if (!object)
        return nullptr;
    
    lex.set_depth(lex.depth() + 1);
    if (lex.depth() > JSON_PARSER_MAX_DEPTH) {
        error_set(error, &lex, jansson::json_error_code::invalid_syntax,
                  "maximum parsing depth exceeded");
        jansson::json_decref(object);
        return nullptr;
    }
    
    token = lex.get_token(error);
    if (token == '}') {
        lex.set_depth(lex.depth() - 1);
        return object;
    }
    
    while (token == TOKEN_STRING) {
        std::string key = lex.steal_string();
        jansson::json_t* value;
        
        token = lex.get_token(error);
        if (token != ':') {
            error_set(error, &lex, jansson::json_error_code::invalid_syntax,
                      "expected ':' after object key");
            jansson::json_decref(object);
            return nullptr;
        }
        
        token = lex.get_token(error);
        value = parse_value(lex, flags, error);
        if (!value) {
            jansson::json_decref(object);
            return nullptr;
        }
        
        if (jansson::json_object_set_new(object, key.c_str(), value) < 0) {
            jansson::json_decref(object);
            return nullptr;
        }
        
        token = lex.get_token(error);
        if (token != ',')
            break;
            
        token = lex.get_token(error);
    }
    
    if (token != '}') {
        error_set(error, &lex, jansson::json_error_code::invalid_syntax,
                  "expected '}' after object");
        jansson::json_decref(object);
        return nullptr;
    }
    
    lex.set_depth(lex.depth() - 1);
    return object;
}

static jansson::json_t* parse_array(Lexer& lex, size_t flags, jansson::json_error_t* error) {
    jansson::json_t* array = jansson::json_array();
    int token;
    
    if (!array)
        return nullptr;
    
    lex.set_depth(lex.depth() + 1);
    if (lex.depth() > JSON_PARSER_MAX_DEPTH) {
        error_set(error, &lex, jansson::json_error_code::invalid_syntax,
                  "maximum parsing depth exceeded");
        jansson::json_decref(array);
        return nullptr;
    }
    
    token = lex.get_token(error);
    if (token == ']') {
        lex.set_depth(lex.depth() - 1);
        return array;
    }
    
    while (true) {
        jansson::json_t* value = parse_value(lex, flags, error);
        if (!value) {
            jansson::json_decref(array);
            return nullptr;
        }
        
        if (jansson::json_array_append_new(array, value) < 0) {
            jansson::json_decref(array);
            return nullptr;
        }
        
        token = lex.get_token(error);
        if (token != ',')
            break;
            
        token = lex.get_token(error);
    }
    
    if (token != ']') {
        error_set(error, &lex, jansson::json_error_code::invalid_syntax,
                  "expected ']' after array");
        jansson::json_decref(array);
        return nullptr;
    }
    
    lex.set_depth(lex.depth() - 1);
    return array;
}

// String loading function
jansson::json_t* json_loads(const std::string& input, size_t flags, jansson::json_error_t* error) {
    size_t pos = 0;
    
    auto get_func = [&input, &pos]() -> int {
        if (pos >= input.length())
            return EOF;
        return static_cast<unsigned char>(input[pos++]);
    };
    
    Lexer lex(get_func, flags);
    jsonp_error_init(error, "<string>");
    
    return parse_json(lex, flags, error);
}

// Buffer loading function
jansson::json_t* json_loadb(const std::string& buffer, size_t buflen, size_t flags, jansson::json_error_t* error) {
    size_t pos = 0;
    size_t len = std::min(buflen, buffer.length());
    
    auto get_func = [&buffer, &pos, len]() -> int {
        if (pos >= len)
            return EOF;
        return static_cast<unsigned char>(buffer[pos++]);
    };
    
    Lexer lex(get_func, flags);
    jsonp_error_init(error, "<buffer>");
    
    return parse_json(lex, flags, error);
}

// File loading function
jansson::json_t* json_loadf(std::FILE* input, size_t flags, jansson::json_error_t* error) {
    if (!input) {
        error_set(error, nullptr, jansson::json_error_code::invalid_argument,
                  "wrong arguments");
        return nullptr;
    }
    
    auto get_func = [input]() -> int {
        return std::fgetc(input);
    };
    
    Lexer lex(get_func, flags);
    jsonp_error_init(error, "<stream>");
    
    return parse_json(lex, flags, error);
}

// File descriptor loading function
jansson::json_t* json_loadfd(int input, size_t flags, jansson::json_error_t* error) {
    auto get_func = [input]() -> int {
        char c;
        if (read(input, &c, 1) == 1)
            return static_cast<unsigned char>(c);
        return EOF;
    };
    
    Lexer lex(get_func, flags);
    jsonp_error_init(error, "<fd>");
    
    return parse_json(lex, flags, error);
}

class CallbackData {
public:
    static const size_t MAX_BUF_LEN = 1024;
    
    CallbackData(jansson::json_load_callback_t callback, void* data) 
        : callback_(callback), user_data_(data), pos_(0), len_(0) {
        data_.resize(MAX_BUF_LEN);
    }
    
    int get_char() {
        if (pos_ >= len_) {
            pos_ = 0;
            len_ = callback_(data_.data(), MAX_BUF_LEN, user_data_);
            if (len_ == 0 || len_ == static_cast<size_t>(-1))
                return EOF;
        }
        
        char c = data_[pos_++];
        return static_cast<unsigned char>(c);
    }
    
private:
    jansson::json_load_callback_t callback_;
    void* user_data_;
    std::vector<char> data_;
    size_t pos_;
    size_t len_;
};

// Callback loading function

jansson::json_t* json_load_callback(jansson::json_load_callback_t callback, void* data, size_t flags, jansson::json_error_t* error) {
    if (!callback) {
        error_set(error, nullptr, jansson::json_error_code::invalid_argument,
                  "wrong arguments");
        return nullptr;
    }
    
    CallbackData stream_data(callback, data);
    
    auto get_func = [&stream_data]() -> int {
        return stream_data.get_char();
    };
    
    Lexer lex(get_func, flags);
    jsonp_error_init(error, "<callback>");
    
    return parse_json(lex, flags, error);
}
