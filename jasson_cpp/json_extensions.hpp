#ifndef JASSON_JSON_EXTENSIONS_HPP
#define JASSON_JSON_EXTENSIONS_HPP

#include "json_value.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <optional>

namespace jasson {

// JSON dump format options
enum class JsonDumpFormat {
    COMPACT = 0,
    INDENT_2 = 2,
    INDENT_4 = 4,
    INDENT_8 = 8,
    PRETTY = INDENT_2,
    MINIFIED = COMPACT
};

// JSON dump flags
enum class JsonDumpFlags {
    NONE = 0,
    ENSURE_ASCII = 1 << 0,
    COMPACT = 1 << 1,
    SORT_KEYS = 1 << 2,
    ENCODE_ANY = 1 << 3,
    ESCAPE_SLASH = 1 << 4,
    REAL_PRECISION = 1 << 5,
    INDENT = 1 << 6
};

inline JsonDumpFlags operator|(JsonDumpFlags a, JsonDumpFlags b) {
    return static_cast<JsonDumpFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inline JsonDumpFlags operator&(JsonDumpFlags a, JsonDumpFlags b) {
    return static_cast<JsonDumpFlags>(static_cast<int>(a) & static_cast<int>(b));
}

inline bool has_flag(JsonDumpFlags flags, JsonDumpFlags flag) {
    return (static_cast<int>(flags) & static_cast<int>(flag)) != 0;
}

// JSON pack/unpack format specifiers
enum class JsonPackFormat {
    // Basic types
    OBJECT = 'o',
    ARRAY = 'a',
    STRING = 's',
    INTEGER = 'i',
    REAL = 'f',
    BOOLEAN = 'b',
    NULL_VALUE = 'n',
    
    // Extended types
    STRV = 'v',     // String view
    STRP = 'p',     // String pointer
    INT8 = 'b',     // 8-bit integer
    UINT8 = 'B',    // 8-bit unsigned integer
    INT16 = 'h',    // 16-bit integer
    UINT16 = 'H',   // 16-bit unsigned integer
    INT32 = 'l',    // 32-bit integer
    UINT32 = 'I',   // 32-bit unsigned integer
    INT64 = 'L',    // 64-bit integer
    UINT64 = 'K',   // 64-bit unsigned integer
    FLOAT = 'f',    // Float
    DOUBLE = 'd',   // Double
    
    // Special formats
    IGNORE = '!',   // Ignore this value
    STRICT = '+',   // Strict type checking
    COMPACT_FORMAT = 'c',  // Compact format
    PRETTY_FORMAT = 'P'    // Pretty format
};

// JSON value visitor (similar to std::variant visitor)
template<typename... Visitors>
class JsonVisitor : public Visitors... {
public:
    using Visitors::operator()...;
};

template<typename... Visitors>
JsonVisitor(Visitors...) -> JsonVisitor<Visitors...>;

// JSON path support
class JsonPath {
public:
    JsonPath() = default;
    explicit JsonPath(const std::string& path);
    
    // Path manipulation
    JsonPath& operator/(const std::string& key);
    JsonPath& operator/(size_t index);
    JsonPath& operator/=(const std::string& key);
    JsonPath& operator/=(size_t index);
    
    // Path string representation
    std::string str() const { return path_; }
    operator std::string() const { return path_; }
    
    // Path components
    std::vector<std::string> components() const;
    
    // Path queries
    bool is_root() const { return path_.empty(); }
    bool is_absolute() const { return !path_.empty() && path_[0] == '$'; }
    bool is_relative() const { return !is_absolute(); }
    
    // Path navigation
    JsonPath parent() const;
    std::string name() const;
    
private:
    std::string path_;
};

// JSON pointer support (RFC 6901)
class JsonPointer {
public:
    JsonPointer() = default;
    explicit JsonPointer(const std::string& pointer);
    
    // Pointer manipulation
    JsonPointer& operator/(const std::string& token);
    JsonPointer& operator/(size_t index);
    JsonPointer& operator/=(const std::string& token);
    JsonPointer& operator/=(size_t index);
    
    // Pointer string representation
    std::string str() const { return pointer_; }
    operator std::string() const { return pointer_; }
    
    // Token access
    const std::vector<std::string>& tokens() const { return tokens_; }
    
    // Pointer queries
    bool is_root() const { return tokens_.empty(); }
    
    // Pointer navigation
    JsonPointer parent() const;
    std::string token() const;
    
    // JSON value access
    JsonValue* get(JsonValue& root) const;
    const JsonValue* get(const JsonValue& root) const;
    JsonValue& at(JsonValue& root) const;
    const JsonValue& at(const JsonValue& root) const;
    
private:
    std::string pointer_;
    std::vector<std::string> tokens_;
    
    void parse_pointer(const std::string& pointer);
    static std::string unescape_token(const std::string& token);
    static std::string escape_token(const std::string& token);
};

// JSON patch support (RFC 6902)
class JsonPatch {
public:
    enum class OperationType {
        ADD,
        REMOVE,
        REPLACE,
        MOVE,
        COPY,
        TEST
    };
    
    struct Operation {
        OperationType op;
        JsonPointer path;
        JsonPointer from;  // For move and copy operations
        JsonValuePtr value;  // For add, replace, and test operations
    };
    
    JsonPatch() = default;
    explicit JsonPatch(const JsonArray& patch);
    explicit JsonPatch(const std::string& patch);
    
    // Patch operations
    void add(const JsonPointer& path, JsonValuePtr value);
    void remove(const JsonPointer& path);
    void replace(const JsonPointer& path, JsonValuePtr value);
    void move(const JsonPointer& from, const JsonPointer& path);
    void copy(const JsonPointer& from, const JsonPointer& path);
    void test(const JsonPointer& path, JsonValuePtr value);
    
    // Patch application
    JsonValuePtr apply(JsonValuePtr target) const;
    bool apply_in_place(JsonValue& target) const;
    
    // Patch validation
    bool is_valid() const;
    std::vector<std::string> validate() const;
    
    // Patch serialization
    JsonArray to_json() const;
    std::string to_string() const;
    
private:
    std::vector<Operation> operations_;
    
    JsonValuePtr apply_operation(const Operation& op, JsonValuePtr target) const;
    bool apply_operation_in_place(const Operation& op, JsonValue& target) const;
};

// JSON merge patch support (RFC 7396)
class JsonMergePatch {
public:
    JsonMergePatch() = default;
    explicit JsonMergePatch(const JsonValue& patch);
    explicit JsonMergePatch(const std::string& patch);
    
    // Patch application
    JsonValuePtr apply(JsonValuePtr target) const;
    bool apply_in_place(JsonValue& target) const;
    
    // Patch generation
    static JsonValuePtr create(JsonValuePtr source, JsonValuePtr target);
    
private:
    JsonValuePtr patch_;
    
    JsonValuePtr merge(JsonValuePtr target, JsonValuePtr patch) const;
    void merge_in_place(JsonValue& target, const JsonValue& patch) const;
};

// JSON schema validation (draft-07)
class JsonSchema {
public:
    JsonSchema() = default;
    explicit JsonSchema(const JsonValue& schema);
    explicit JsonSchema(const std::string& schema);
    
    // Validation
    bool validate(const JsonValue& instance) const;
    std::vector<std::string> validate_detailed(const JsonValue& instance) const;
    
    // Schema properties
    bool is_valid() const { return schema_ != nullptr; }
    JsonValuePtr schema() const { return schema_; }
    
    // Schema loading
    void load(const JsonValue& schema);
    void load(const std::string& schema);
    
private:
    JsonValuePtr schema_;
    
    bool validate_instance(const JsonValue& instance, const JsonValue& schema, const JsonPointer& path) const;
    std::vector<std::string> validate_instance_detailed(const JsonValue& instance, const JsonValue& schema, const JsonPointer& path) const;
};

// JSON query support (JSONPath-like)
class JsonQuery {
public:
    JsonQuery() = default;
    explicit JsonQuery(const std::string& query);
    
    // Query execution
    std::vector<JsonValue*> execute(JsonValue& root);
    std::vector<const JsonValue*> execute(const JsonValue& root) const;
    
    // Query validation
    bool is_valid() const { return !query_.empty(); }
    
    // Query compilation
    void compile(const std::string& query);
    
private:
    std::string query_;
    std::vector<std::string> tokens_;
    
    void parse_query(const std::string& query);
    std::vector<JsonValue*> execute_tokens(JsonValue& current, size_t token_index);
    std::vector<const JsonValue*> execute_tokens(const JsonValue& current, size_t token_index) const;
};

// JSON streaming support
class JsonStreamReader {
public:
    using ReadCallback = std::function<void(JsonValuePtr)>;
    using ErrorCallback = std::function<void(const std::string&)>;
    
    JsonStreamReader() = default;
    
    // Stream configuration
    void set_read_callback(ReadCallback callback) { read_callback_ = callback; }
    void set_error_callback(ErrorCallback callback) { error_callback_ = callback; }
    
    // Stream operations
    void feed(const std::string& data);
    void feed(const char* data, size_t length);
    void end();
    void reset();
    
    // Stream state
    bool is_valid() const { return valid_; }
    size_t bytes_processed() const { return bytes_processed_; }
    
private:
    ReadCallback read_callback_;
    ErrorCallback error_callback_;
    std::string buffer_;
    bool valid_ = true;
    size_t bytes_processed_ = 0;
    
    void process_buffer();
    void handle_error(const std::string& error);
};

class JsonStreamWriter {
public:
    using WriteCallback = std::function<void(const std::string&)>;
    using ErrorCallback = std::function<void(const std::string&)>;
    
    JsonStreamWriter() = default;
    
    // Stream configuration
    void set_write_callback(WriteCallback callback) { write_callback_ = callback; }
    void set_error_callback(ErrorCallback callback) { error_callback_ = callback; }
    void set_format(JsonDumpFormat format) { format_ = format; }
    void set_flags(JsonDumpFlags flags) { flags_ = flags; }
    
    // Stream operations
    void write(JsonValuePtr value);
    void reset();
    
    // Stream state
    bool is_valid() const { return valid_; }
    size_t bytes_written() const { return bytes_written_; }
    
private:
    WriteCallback write_callback_;
    ErrorCallback error_callback_;
    JsonDumpFormat format_ = JsonDumpFormat::COMPACT;
    JsonDumpFlags flags_ = JsonDumpFlags::NONE;
    bool valid_ = true;
    size_t bytes_written_ = 0;
    
    void write_value(const JsonValue& value);
    void handle_error(const std::string& error);
};

// Convenience functions
std::string json_dump(const JsonValue& value, JsonDumpFormat format = JsonDumpFormat::COMPACT);
std::string json_dump(const JsonValue& value, JsonDumpFlags flags, size_t indent = 0);

} // namespace jasson

#endif // JASSON_JSON_EXTENSIONS_HPP
