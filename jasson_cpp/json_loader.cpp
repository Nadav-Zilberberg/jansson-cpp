#include <unistd.h>
#include "json_loader.hpp"
#include "json_factory.hpp"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <limits>

namespace jasson {

// JsonLoadException implementation
// JsonLoader implementation
JsonLoader::JsonLoader(JsonLoadFlags flags) : flags_(flags), max_depth_(2048) {}

JsonValuePtr JsonLoader::load_from_string(const std::string& input) {
    return load_from_string(input.c_str(), input.length());
}

JsonValuePtr JsonLoader::load_from_string(const char* input, size_t length) {
    ParseContext ctx(input, input + length);
    skip_whitespace(ctx);
    
    if (ctx.current >= ctx.end) {
        throw_parse_error(ctx, "Empty input");
    }
    
    auto result = parse_value(ctx);
    skip_whitespace(ctx);
    
    if (ctx.current < ctx.end && !has_flag(flags_, JsonLoadFlags::DISABLE_EOF_CHECK)) {
        throw_parse_error(ctx, "Unexpected content after JSON value");
    }
    
    return result;
}

JsonValuePtr JsonLoader::load_from_stream(std::istream& input) {
    // Read entire stream into memory
    std::string content((std::istreambuf_iterator<char>(input)),
                       std::istreambuf_iterator<char>());
    
    if (input.bad()) {
        throw JsonLoadException("Failed to read from stream");
    }
    
    return load_from_string(content);
}

JsonValuePtr JsonLoader::load_from_file(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw JsonLoadException("Cannot open file: " + path.string());
    }
    
    return load_from_stream(file);
}

JsonValuePtr JsonLoader::load_from_file(const std::string& path) {
    return load_from_file(std::filesystem::path(path));
}

JsonValuePtr JsonLoader::load_from_fd(int fd) {
    // Read from file descriptor
    std::string content;
    char buffer[4096];
    ssize_t bytes_read;
    
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        content.append(buffer, bytes_read);
    }
    
    if (bytes_read < 0) {
        throw JsonLoadException("Failed to read from file descriptor");
    }
    
    return load_from_string(content);
}

template<typename InputIterator>
JsonValuePtr JsonLoader::load_from_iterators(InputIterator begin, InputIterator end) {
    std::string content(begin, end);
    return load_from_string(content);
}

// Explicit template instantiation for common iterator types
template JsonValuePtr JsonLoader::load_from_iterators<std::string::iterator>(
    std::string::iterator, std::string::iterator);
template JsonValuePtr JsonLoader::load_from_iterators<std::string::const_iterator>(
    std::string::const_iterator, std::string::const_iterator);
template JsonValuePtr JsonLoader::load_from_iterators<const char*>(
    const char*, const char*);

// Core parsing functions
JsonValuePtr JsonLoader::parse_value(ParseContext& ctx) {
    skip_whitespace(ctx);
    
    if (ctx.current >= ctx.end) {
        throw_unexpected_eof(ctx);
    }
    
    char ch = *ctx.current;
    
    switch (ch) {
        case '{':
            return parse_object(ctx);
        case '[':
            return parse_array(ctx);
        case '"':
            return parse_string(ctx);
        case 't':
            return parse_literal(ctx, "true", JsonType::TRUE);
        case 'f':
            return parse_literal(ctx, "false", JsonType::FALSE);
        case 'n':
            return parse_literal(ctx, "null", JsonType::NULL_VALUE);
        case '-':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return parse_number(ctx);
        default:
            throw_unexpected_char(ctx, ch);
    }
}

JsonValuePtr JsonLoader::parse_object(ParseContext& ctx) {
    expect_char(ctx, '{');
    
    auto obj = json_object();
    bool first = true;
    
    while (true) {
        skip_whitespace(ctx);
        
        if (ctx.current >= ctx.end) {
            throw_unexpected_eof(ctx);
        }
        
        if (*ctx.current == '}') {
            consume_char(ctx);
            break;
        }
        
        if (!first) {
            expect_char(ctx, ',');
            skip_whitespace(ctx);
        }
        first = false;
        
        // Parse key
        if (ctx.current >= ctx.end || *ctx.current != '"') {
            throw_parse_error(ctx, "Expected string key");
        }
        
        auto key_ptr = parse_string(ctx);
        std::string key = std::static_pointer_cast<JsonString>(key_ptr)->value();
        
        skip_whitespace(ctx);
        expect_char(ctx, ':');
        
        // Parse value
        auto value = parse_value(ctx);
        
        // Check for duplicate keys
        if (has_flag(flags_, JsonLoadFlags::REJECT_DUPLICATES) && obj->get(key)) {
            throw_parse_error(ctx, "Duplicate key: " + key);
        }
        
        obj->set(key, value);
    }
    
    return obj;
}

JsonValuePtr JsonLoader::parse_array(ParseContext& ctx) {
    expect_char(ctx, '[');
    
    auto arr = json_array();
    bool first = true;
    
    while (true) {
        skip_whitespace(ctx);
        
        if (ctx.current >= ctx.end) {
            throw_unexpected_eof(ctx);
        }
        
        if (*ctx.current == ']') {
            consume_char(ctx);
            break;
        }
        
        if (!first) {
            expect_char(ctx, ',');
            skip_whitespace(ctx);
        }
        first = false;
        
        auto value = parse_value(ctx);
        arr->append(value);
    }
    
    return arr;
}

JsonValuePtr JsonLoader::parse_string(ParseContext& ctx) {
    expect_char(ctx, '"');
    
    std::string content = parse_string_content(ctx);
    expect_char(ctx, '"');
    
    validate_string(content);
    return json_string(content);
}

JsonValuePtr JsonLoader::parse_number(ParseContext& ctx) {
    const char* start = ctx.current;
    bool is_negative = false;
    bool is_real = false;
    
    // Optional minus sign
    if (ctx.current < ctx.end && *ctx.current == '-') {
        is_negative = true;
        consume_char(ctx);
    }
    
    // Integer part
    if (ctx.current >= ctx.end || !is_digit(*ctx.current)) {
        throw_parse_error(ctx, "Expected digit");
    }
    
    if (*ctx.current == '0') {
        consume_char(ctx);
        // Leading zero must not be followed by more digits
        if (ctx.current < ctx.end && is_digit(*ctx.current)) {
            throw_parse_error(ctx, "Leading zeros are not allowed");
        }
    } else {
        while (ctx.current < ctx.end && is_digit(*ctx.current)) {
            consume_char(ctx);
        }
    }
    
    // Fractional part
    if (ctx.current < ctx.end && *ctx.current == '.') {
        is_real = true;
        consume_char(ctx);
        
        if (ctx.current >= ctx.end || !is_digit(*ctx.current)) {
            throw_parse_error(ctx, "Expected digit after decimal point");
        }
        
        while (ctx.current < ctx.end && is_digit(*ctx.current)) {
            consume_char(ctx);
        }
    }
    
    // Exponent part
    if (ctx.current < ctx.end && (*ctx.current == 'e' || *ctx.current == 'E')) {
        is_real = true;
        consume_char(ctx);
        
        // Optional sign
        if (ctx.current < ctx.end && (*ctx.current == '+' || *ctx.current == '-')) {
            consume_char(ctx);
        }
        
        if (ctx.current >= ctx.end || !is_digit(*ctx.current)) {
            throw_parse_error(ctx, "Expected digit in exponent");
        }
        
        while (ctx.current < ctx.end && is_digit(*ctx.current)) {
            consume_char(ctx);
        }
    }
    
    // Parse the number
    std::string num_str(start, ctx.current - start);
    
    try {
        if (is_real || has_flag(flags_, JsonLoadFlags::DECODE_INT_AS_REAL)) {
            double value = std::stod(num_str);
            
            // Check for overflow/underflow
            if (std::isinf(value)) {
                throw_parse_error(ctx, "Numeric overflow");
            }
            
            return json_real(value);
        } else {
            json_int_t value = std::stoll(num_str);
            return json_integer(value);
        }
    } catch (const std::out_of_range&) {
        throw_parse_error(ctx, "Numeric overflow");
    } catch (const std::invalid_argument&) {
        throw_parse_error(ctx, "Invalid number format");
    }
}

JsonValuePtr JsonLoader::parse_literal(ParseContext& ctx, const std::string& literal, JsonType type) {
    for (char expected : literal) {
        if (ctx.current >= ctx.end || *ctx.current != expected) {
            throw_parse_error(ctx, "Expected '" + literal + "'");
        }
        consume_char(ctx);
    }
    
    switch (type) {
        case JsonType::TRUE:
            return json_true();
        case JsonType::FALSE:
            return json_false();
        case JsonType::NULL_VALUE:
            return json_null();
        default:
            throw std::logic_error("Invalid literal type");
    }
}

// Helper functions
void JsonLoader::skip_whitespace(ParseContext& ctx) {
    while (ctx.current < ctx.end && std::isspace(*ctx.current)) {
        advance_position(ctx, *ctx.current);
        ctx.current++;
    }
}

char JsonLoader::peek_char(ParseContext& ctx) {
    if (ctx.current >= ctx.end) {
        throw_unexpected_eof(ctx);
    }
    return *ctx.current;
}

char JsonLoader::consume_char(ParseContext& ctx) {
    if (ctx.current >= ctx.end) {
        throw_unexpected_eof(ctx);
    }
    
    char ch = *ctx.current;
    advance_position(ctx, ch);
    ctx.current++;
    return ch;
}

void JsonLoader::advance_position(ParseContext& ctx, char ch) {
    ctx.position++;
    
    if (ch == '\n') {
        ctx.line++;
        ctx.column = 1;
    } else {
        ctx.column++;
    }
}

void JsonLoader::expect_char(ParseContext& ctx, char expected) {
    char actual = consume_char(ctx);
    if (actual != expected) {
        throw_parse_error(ctx, std::string("Expected '") + expected + "' but got '" + actual + "'");
    }
}

std::string JsonLoader::parse_string_content(ParseContext& ctx) {
    std::string result;
    
    while (ctx.current < ctx.end && *ctx.current != '"') {
        if (*ctx.current == '\\') {
            consume_char(ctx); // consume backslash
            result += parse_escape_sequence(ctx);
        } else if (*ctx.current < 0x20) {
            throw_parse_error(ctx, "Unescaped control character in string");
        } else {
            result += consume_char(ctx);
        }
    }
    
    return result;
}

std::string JsonLoader::parse_escape_sequence(ParseContext& ctx) {
    if (ctx.current >= ctx.end) {
        throw_unexpected_eof(ctx);
    }
    
    char ch = consume_char(ctx);
    
    switch (ch) {
        case '"': return "\"";
        case '\\': return "\\";
        case '/': return "/";
        case 'b': return "\b";
        case 'f': return "\f";
        case 'n': return "\n";
        case 'r': return "\r";
        case 't': return "\t";
        case 'u': {
            // Unicode escape sequence
            if (ctx.end - ctx.current < 4) {
                throw_parse_error(ctx, "Incomplete Unicode escape sequence");
            }
            
            std::string hex;
            for (int i = 0; i < 4; i++) {
                if (!is_hex_digit(*ctx.current)) {
                    throw_parse_error(ctx, "Invalid Unicode escape sequence");
                }
                hex += consume_char(ctx);
            }
            
            int code_point = 0;
            for (char c : hex) {
                code_point = code_point * 16 + hex_to_int(c);
            }
            
            // Convert to UTF-8
            if (code_point <= 0x7F) {
                return std::string(1, static_cast<char>(code_point));
            } else if (code_point <= 0x7FF) {
                std::string result;
                result += static_cast<char>(0xC0 | (code_point >> 6));
                result += static_cast<char>(0x80 | (code_point & 0x3F));
                return result;
            } else {
                std::string result;
                result += static_cast<char>(0xE0 | (code_point >> 12));
                result += static_cast<char>(0x80 | ((code_point >> 6) & 0x3F));
                result += static_cast<char>(0x80 | (code_point & 0x3F));
                return result;
            }
        }
        default:
            throw_parse_error(ctx, std::string("Invalid escape sequence: \\") + ch);
    }
}

bool JsonLoader::is_digit(char ch) {
    return ch >= '0' && ch <= '9';
}

bool JsonLoader::is_hex_digit(char ch) {
    return (ch >= '0' && ch <= '9') || 
           (ch >= 'a' && ch <= 'f') || 
           (ch >= 'A' && ch <= 'F');
}

int JsonLoader::hex_to_int(char ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    return 0;
}

// Error handling
void JsonLoader::throw_parse_error(ParseContext& ctx, const std::string& message) {
    throw JsonLoadException(message, ctx.line, ctx.column, ctx.position);
}

void JsonLoader::throw_unexpected_char(ParseContext& ctx, char ch) {
    throw_parse_error(ctx, std::string("Unexpected character: '") + ch + "'");
}

void JsonLoader::throw_unexpected_eof(ParseContext& ctx) {
    throw_parse_error(ctx, "Unexpected end of input");
}

// Validation
void JsonLoader::validate_depth(ParseContext& ctx) {
    if (ctx.depth > max_depth_) {
        throw_parse_error(ctx, "Maximum parsing depth exceeded");
    }
}

void JsonLoader::validate_string(const std::string& str) {
    if (!has_flag(flags_, JsonLoadFlags::ALLOW_NUL)) {
        if (str.find('\0') != std::string::npos) {
            throw JsonLoadException("NUL character not allowed in string");
        }
    }
}

// Convenience functions
JsonValuePtr json_loads(const std::string& input, JsonLoadFlags flags) {
    JsonLoader loader(flags);
    return loader.load_from_string(input);
}

JsonValuePtr json_loads(const char* input, size_t length, JsonLoadFlags flags) {
    JsonLoader loader(flags);
    return loader.load_from_string(input, length);
}

JsonValuePtr json_load_file(const std::filesystem::path& path, JsonLoadFlags flags) {
    JsonLoader loader(flags);
    return loader.load_from_file(path);
}

JsonValuePtr json_load_file(const std::string& path, JsonLoadFlags flags) {
    JsonLoader loader(flags);
    return loader.load_from_file(path);
}

JsonValuePtr json_load_stream(std::istream& input, JsonLoadFlags flags) {
    JsonLoader loader(flags);
    return loader.load_from_stream(input);
}

} // namespace jasson
