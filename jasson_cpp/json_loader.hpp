#ifndef JASSON_JSON_LOADER_HPP
#define JASSON_JSON_LOADER_HPP

#include "json_value.hpp"
#include <istream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

namespace jasson {

// Exception class for JSON loading errors
class JsonLoadException : public std::runtime_error {
public:
    JsonLoadException(const std::string& message, int line = -1, int column = -1, size_t position = 0)
        : std::runtime_error(message), line_(line), column_(column), position_(position) {}
    
    int line() const { return line_; }
    int column() const { return column_; }
    size_t position() const { return position_; }

private:
    int line_;
    int column_;
    size_t position_;
};

// Configuration flags for JSON loading
enum class JsonLoadFlags {
    NONE = 0,
    REJECT_DUPLICATES = 1 << 0,
    DISABLE_EOF_CHECK = 1 << 1,
    DECODE_ANY = 1 << 2,
    DECODE_INT_AS_REAL = 1 << 3,
    ALLOW_NUL = 1 << 4
};

inline JsonLoadFlags operator|(JsonLoadFlags a, JsonLoadFlags b) {
    return static_cast<JsonLoadFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inline JsonLoadFlags operator&(JsonLoadFlags a, JsonLoadFlags b) {
    return static_cast<JsonLoadFlags>(static_cast<int>(a) & static_cast<int>(b));
}

inline bool has_flag(JsonLoadFlags flags, JsonLoadFlags flag) {
    return (static_cast<int>(flags) & static_cast<int>(flag)) != 0;
}

// JsonLoader class for loading JSON from various sources
class JsonLoader {
public:
    explicit JsonLoader(JsonLoadFlags flags = JsonLoadFlags::NONE);
    
    // Load from string
    JsonValuePtr load_from_string(const std::string& input);
    JsonValuePtr load_from_string(const char* input, size_t length);
    
    // Load from stream
    JsonValuePtr load_from_stream(std::istream& input);
    
    // Load from file
    JsonValuePtr load_from_file(const std::filesystem::path& path);
    JsonValuePtr load_from_file(const std::string& path);
    
    // Load from file descriptor (Unix-style)
    JsonValuePtr load_from_fd(int fd);
    
    // Iterator-based loading (modern replacement for callback-based loading)
    template<typename InputIterator>
    JsonValuePtr load_from_iterators(InputIterator begin, InputIterator end);
    
    // Get/set flags
    JsonLoadFlags flags() const { return flags_; }
    void set_flags(JsonLoadFlags flags) { flags_ = flags; }
    
    // Configuration
    void set_max_depth(size_t max_depth) { max_depth_ = max_depth; }
    size_t max_depth() const { return max_depth_; }

private:
    // Internal parsing structures
    struct ParseContext {
        const char* current;
        const char* end;
        size_t line;
        size_t column;
        size_t position;
        size_t depth;
        
        ParseContext(const char* begin, const char* end)
            : current(begin), end(end), line(1), column(1), position(0), depth(0) {}
    };
    
    JsonLoadFlags flags_;
    size_t max_depth_;
    
    // Core parsing functions
    JsonValuePtr parse_value(ParseContext& ctx);
    JsonValuePtr parse_object(ParseContext& ctx);
    JsonValuePtr parse_array(ParseContext& ctx);
    JsonValuePtr parse_string(ParseContext& ctx);
    JsonValuePtr parse_number(ParseContext& ctx);
    JsonValuePtr parse_literal(ParseContext& ctx, const std::string& literal, JsonType type);
    
    // Helper functions
    void skip_whitespace(ParseContext& ctx);
    char peek_char(ParseContext& ctx);
    char consume_char(ParseContext& ctx);
    void advance_position(ParseContext& ctx, char ch);
    void expect_char(ParseContext& ctx, char expected);
    std::string parse_string_content(ParseContext& ctx);
    std::string parse_escape_sequence(ParseContext& ctx);
    bool is_digit(char ch);
    bool is_hex_digit(char ch);
    int hex_to_int(char ch);
    
    // Error handling
    [[noreturn]] void throw_parse_error(ParseContext& ctx, const std::string& message);
    [[noreturn]] void throw_unexpected_char(ParseContext& ctx, char ch);
    [[noreturn]] void throw_unexpected_eof(ParseContext& ctx);
    
    // Validation
    void validate_depth(ParseContext& ctx);
    void validate_string(const std::string& str);
};

// Convenience functions (similar to C API but with exceptions)
JsonValuePtr json_loads(const std::string& input, JsonLoadFlags flags = JsonLoadFlags::NONE);
JsonValuePtr json_loads(const char* input, size_t length, JsonLoadFlags flags = JsonLoadFlags::NONE);
JsonValuePtr json_load_file(const std::filesystem::path& path, JsonLoadFlags flags = JsonLoadFlags::NONE);
JsonValuePtr json_load_file(const std::string& path, JsonLoadFlags flags = JsonLoadFlags::NONE);
JsonValuePtr json_load_stream(std::istream& input, JsonLoadFlags flags = JsonLoadFlags::NONE);

} // namespace jasson

#endif // JASSON_JSON_LOADER_HPP
