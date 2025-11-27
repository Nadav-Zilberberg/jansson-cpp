#ifndef JASSON_JSON_DUMPER_HPP
#define JASSON_JSON_DUMPER_HPP

#include "json_value.hpp"
#include "json_extensions.hpp"
#include <string>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <memory>

namespace jasson {

// JSON dumper class
class JsonDumper {
public:
    explicit JsonDumper(JsonDumpFormat format = JsonDumpFormat::COMPACT);
    JsonDumper(JsonDumpFlags flags, size_t indent = 0);
    
    // Dump to string
    std::string dump(const JsonValue& value);
    std::string dump(JsonValuePtr value);
    
    // Dump to stream
    void dump(const JsonValue& value, std::ostream& output);
    void dump(JsonValuePtr value, std::ostream& output);
    
    // Configuration
    void set_format(JsonDumpFormat format) { format_ = format; }
    void set_flags(JsonDumpFlags flags) { flags_ = flags; }
    void set_indent(size_t indent) { indent_ = indent; }
    void set_precision(size_t precision) { precision_ = precision; }
    
    JsonDumpFormat format() const { return format_; }
    JsonDumpFlags flags() const { return flags_; }
    size_t indent() const { return indent_; }
    size_t precision() const { return precision_; }

private:
    JsonDumpFormat format_;
    JsonDumpFlags flags_;
    size_t indent_;
    size_t precision_;
    std::ostringstream output_;
    size_t depth_;
    
    // Core dumping functions
    void dump_value(const JsonValue& value);
    void dump_object(const JsonObject& obj);
    void dump_array(const JsonArray& arr);
    void dump_string(const JsonString& str);
    void dump_integer(const JsonInteger& num);
    void dump_real(const JsonReal& num);
    void dump_boolean(const JsonBoolean& bool_val);
    void dump_null(const JsonNull& null_val);
    
    // Helper functions
    void write_indent();
    void write_newline();
    void write_string(const std::string& str);
    void write_char(char ch);
    void write_escaped_string(const std::string& str);
    bool should_escape_ascii() const;
    bool should_sort_keys() const;
    bool should_escape_slash() const;
    
    // String escaping
    std::string escape_string(const std::string& str);
    static bool needs_escape(char ch);
    static std::string escape_char(char ch);
};

} // namespace jasson

#endif // JASSON_JSON_DUMPER_HPP
