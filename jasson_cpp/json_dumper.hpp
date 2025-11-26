#ifndef JASSON_JSON_DUMPER_HPP
#define JASSON_JSON_DUMPER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <filesystem>
#include "json_types.hpp"
#include "error.hpp"

namespace jasson {

enum class JsonDumpFormat {
    COMPACT = 0,
    INDENTED = 1,
    SORTED = 2,
    ENSURE_ASCII = 4,
    ESCAPE_SLASH = 8,
    EMBED = 16,
    ENCODE_ANY = 32
};

inline JsonDumpFormat operator|(JsonDumpFormat a, JsonDumpFormat b) {
    return static_cast<JsonDumpFormat>(static_cast<int>(a) | static_cast<int>(b));
}

inline bool operator&(JsonDumpFormat a, JsonDumpFormat b) {
    return static_cast<int>(a) & static_cast<int>(b);
}

class JsonDumper {
public:
    explicit JsonDumper(JsonDumpFormat format = JsonDumpFormat::COMPACT,
                       int indent_size = 2,
                       int precision = 6);
    
    virtual ~JsonDumper() = default;

    // Main dump functions
    std::string dump(const JsonValue& json);
    size_t dump_to_buffer(const JsonValue& json, char* buffer, size_t size);
    void dump_to_stream(const JsonValue& json, std::ostream& stream);
    void dump_to_file(const JsonValue& json, const std::filesystem::path& path);

    // Configuration
    void set_format(JsonDumpFormat format);
    void set_indent_size(int size);
    void set_precision(int precision);

protected:
    virtual int write_to_output(const char* buffer, size_t size, void* data);
    
private:
    JsonDumpFormat format_;
    int indent_size_;
    int precision_;
    std::unique_ptr<class JsonDumperImpl> impl_;
};

// Convenience functions
std::string json_dump(const JsonValue& json, JsonDumpFormat format = JsonDumpFormat::COMPACT);
void json_dump_to_file(const JsonValue& json, const std::filesystem::path& path,
                      JsonDumpFormat format = JsonDumpFormat::COMPACT);

} // namespace jasson

#endif // JASSON_JSON_DUMPER_HPP
