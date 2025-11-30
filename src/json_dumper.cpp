#include "json_dumper.hpp"
#include "utf.hpp"
#include "strbuffer.hpp"
#include "hashtable.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <cstring>

namespace jasson {

namespace {

constexpr size_t MAX_INTEGER_STR_LENGTH = 25;
constexpr size_t MAX_REAL_STR_LENGTH = 25;
constexpr const char* WHITESPACE = "                                ";

struct OutputContext {
    std::ostream* stream = nullptr;
    std::string* string = nullptr;
    struct Buffer {
        char* data;
        size_t size;
        size_t used;
    } buffer = {};
};

int dump_to_stream(const char* buffer, size_t size, void* data) {
    auto* ctx = static_cast<OutputContext*>(data);
    if (ctx->stream) {
        ctx->stream->write(buffer, size);
        return ctx->stream->good() ? 0 : -1;
    }
    if (ctx->string) {
        ctx->string->append(buffer, size);
        return 0;
    }
    if (ctx->buffer.data && ctx->buffer.used + size <= ctx->buffer.size) {
        std::memcpy(&ctx->buffer.data[ctx->buffer.used], buffer, size);
    }
    ctx->buffer.used += size;
    return 0;
}

class JsonDumperImpl {
public:
    JsonDumperImpl(JsonDumpFormat format, int indent_size, int precision)
        : format_(format), indent_size_(indent_size), precision_(precision) {}

    int dump(const JsonValue& json, json_dump_callback_t callback, void* data) {
        parents_set_ = std::make_unique<HashTable>();
        return do_dump(json, 0, callback, data);
    }

private:
    JsonDumpFormat format_;
    int indent_size_;
    int precision_;
    std::unique_ptr<HashTable> parents_set_;

    bool has_flag(JsonDumpFormat flag) const {
        return static_cast<int>(format_) & static_cast<int>(flag);
    }

    int do_dump(const JsonValue& json, int depth, json_dump_callback_t dump, void* data) {
        bool embed = has_flag(JsonDumpFormat::EMBED);
        
        if (!has_flag(JsonDumpFormat::ENCODE_ANY)) {
            if (!json.is_array() && !json.is_object()) {
                throw JsonError(JsonErrorCode::InvalidArgument, "JSON value must be an array or object");
            }
        }

        switch (json.type()) {
            case JsonType::Null:
                return dump("null", 4, data);

            case JsonType::Boolean:
                return json.get_boolean() ? dump("true", 4, data) : dump("false", 5, data);

            case JsonType::Integer: {
                char buffer[MAX_INTEGER_STR_LENGTH];
                auto result = std::snprintf(buffer, sizeof(buffer), "%" PRId64,
                                          json.get_integer());
                if (result < 0 || result >= MAX_INTEGER_STR_LENGTH) {
                    throw JsonError(JsonErrorCode::InternalError, "Integer conversion failed");
                }
                return dump(buffer, result, data);
            }

            case JsonType::Real: {
                char buffer[MAX_REAL_STR_LENGTH];
                auto value = json.get_real();
                auto result = StringConverter::double_to_string(buffer, sizeof(buffer),
                                                               value, precision_);
                if (result < 0) {
                    throw JsonError(JsonErrorCode::InternalError, "Real conversion failed");
                }
                return dump(buffer, result, data);
            }

            case JsonType::String:
                return dump_string(json.get_string(), dump, data);

            case JsonType::Array: {
                return dump_array(json, depth, dump, data, embed);
            }

            case JsonType::Object: {
                return dump_object(json, depth, dump, data, embed);
            }

            default:
                throw JsonError(JsonErrorCode::InternalError, "Unknown JSON type");
        }
    }

    int dump_string(const std::string& str, json_dump_callback_t dump, void* data) {
        if (dump("\"", 1, data)) return -1;

        for (size_t i = 0; i < str.length(); ) {
            int32_t codepoint;
            auto next = Utf8::iterate(&str[i], str.length() - i, &codepoint);
            if (!next) {
                throw JsonError(JsonErrorCode::InvalidUtf8, "Invalid UTF-8 sequence");
            }

            if (codepoint == '\\' || codepoint == '"' || codepoint < 0x20 ||
                (has_flag(JsonDumpFormat::ESCAPE_SLASH) && codepoint == '/') ||
                (has_flag(JsonDumpFormat::ENSURE_ASCII) && codepoint > 0x7F)) {
                
                if (i < next - str.c_str()) {
                    if (dump(&str[i], next - str.c_str() - i, data)) return -1;
                }

                std::string escape_seq = get_escape_sequence(codepoint);
                if (dump(escape_seq.c_str(), escape_seq.length(), data)) return -1;
            } else if (i < next - str.c_str()) {
                if (dump(&str[i], next - str.c_str() - i, data)) return -1;
            }

            i = next - str.c_str();
        }

        return dump("\"", 1, data);
    }

    std::string get_escape_sequence(int32_t codepoint) {
        switch (codepoint) {
            case '\\': return "\\\\";
            case '"': return "\\\"";
            case '\b': return "\\b";
            case '\f': return "\\f";
            case '\n': return "\\n";
            case '\r': return "\\r";
            case '\t': return "\\t";
            case '/': return "\\/";
            default: {
                if (codepoint < 0x10000) {
                    char seq[7];
                    std::snprintf(seq, sizeof(seq), "\\u%04X", static_cast<unsigned int>(codepoint));
                    return seq;
                } else {
                    // UTF-16 surrogate pair
                    int32_t first = 0xD800 | ((codepoint & 0xffc00) >> 10);
                    int32_t last = 0xDC00 | (codepoint & 0x003ff);
                    char seq[13];
                    std::snprintf(seq, sizeof(seq), "\\u%04X\\u%04X",
                                static_cast<unsigned int>(first),
                                static_cast<unsigned int>(last));
                    return seq;
                }
            }
        }
    }

    int dump_indent(int depth, int space, json_dump_callback_t dump, void* data) {
        if (indent_size_ > 0) {
            if (dump("\n", 1, data)) return -1;
            int n_spaces = depth * indent_size_;
            while (n_spaces > 0) {
                int cur_n = std::min(n_spaces, static_cast<int>(std::strlen(WHITESPACE)));
                if (dump(WHITESPACE, cur_n, data)) return -1;
                n_spaces -= cur_n;
            }
        } else if (space && !has_flag(JsonDumpFormat::COMPACT)) {
            return dump(" ", 1, data);
        }
        return 0;
    }

    int dump_array(const JsonValue& json, int depth, json_dump_callback_t dump, void* data, bool embed) {
        std::string key = get_circular_ref_key(json);
        if (parents_set_->contains(key)) {
            throw JsonError(JsonErrorCode::CircularReference, "Circular reference detected");
        }
        parents_set_->set(key, true);

        if (!embed && dump("[", 1, data)) return -1;
        
        auto& array = json.get_array();
        if (array.empty()) {
            parents_set_->del(key);
            return embed ? 0 : dump("]", 1, data);
        }

        if (dump_indent(depth + 1, 0, dump, data)) return -1;

        for (size_t i = 0; i < array.size(); ++i) {
            if (do_dump(*array[i], depth + 1, dump, data)) return -1;

            if (i < array.size() - 1) {
                if (dump(",", 1, data) || dump_indent(depth + 1, 1, dump, data)) return -1;
            } else {
                if (dump_indent(depth, 0, dump, data)) return -1;
            }
        }

        parents_set_->del(key);
        return embed ? 0 : dump("]", 1, data);
    }

    int dump_object(const JsonValue& json, int depth, json_dump_callback_t dump, void* data, bool embed) {
        std::string key = get_circular_ref_key(json);
        if (parents_set_->contains(key)) {
            throw JsonError(JsonErrorCode::CircularReference, "Circular reference detected");
        }
        parents_set_->set(key, true);

        if (!embed && dump("{", 1, data)) return -1;

        auto& object = json.get_object();
        if (object.empty()) {
            parents_set_->del(key);
            return embed ? 0 : dump("}", 1, data);
        }

        if (dump_indent(depth + 1, 0, dump, data)) return -1;

        std::vector<std::pair<std::string, JsonValue*>> items;
        for (auto& [k, v] : object) {
            items.emplace_back(k, v.get());
        }

        if (has_flag(JsonDumpFormat::SORT_KEYS)) {
            std::sort(items.begin(), items.end(),
                     [](const auto& a, const auto& b) { return a.first < b.first; });
        }

        for (size_t i = 0; i < items.size(); ++i) {
            auto& [k, v] = items[i];
            
            if (dump_string(k, dump, data)) return -1;
            
            std::string separator = has_flag(JsonDumpFormat::COMPACT) ? ":" : ": ";
            if (dump(separator.c_str(), separator.length(), data)) return -1;
            
            if (do_dump(*v, depth + 1, dump, data)) return -1;

            if (i < items.size() - 1) {
                if (dump(",", 1, data) || dump_indent(depth + 1, 1, dump, data)) return -1;
            } else {
                if (dump_indent(depth, 0, dump, data)) return -1;
            }
        }

        parents_set_->del(key);
        return embed ? 0 : dump("}", 1, data);
    }

    std::string get_circular_ref_key(const JsonValue& json) {
        // Use the object's address as a unique key
        std::ostringstream oss;
        oss << "0x" << std::hex << reinterpret_cast<uintptr_t>(&json);
        return oss.str();
    }
};

} // namespace

JsonDumper::JsonDumper(JsonDumpFormat format, int indent_size, int precision)
    : format_(format), indent_size_(indent_size), precision_(precision),
      impl_(std::make_unique<JsonDumperImpl>(format, indent_size, precision)) {}

std::string JsonDumper::dump(const JsonValue& json) {
    std::string result;
    OutputContext ctx{.string = &result};
    if (impl_->dump(json, dump_to_stream, &ctx)) {
        throw JsonError(JsonErrorCode::InternalError, "Failed to dump JSON");
    }
    return result;
}

size_t JsonDumper::dump_to_buffer(const JsonValue& json, char* buffer, size_t size) {
    OutputContext ctx{.buffer = {buffer, size, 0}};
    if (impl_->dump(json, dump_to_stream, &ctx)) {
        throw JsonError(JsonErrorCode::InternalError, "Failed to dump JSON to buffer");
    }
    return ctx.buffer.used;
}

void JsonDumper::dump_to_stream(const JsonValue& json, std::ostream& stream) {
    OutputContext ctx{.stream = &stream};
    if (impl_->dump(json, dump_to_stream, &ctx)) {
        throw JsonError(JsonErrorCode::InternalError, "Failed to dump JSON to stream");
    }
}

void JsonDumper::dump_to_file(const JsonValue& json, const std::filesystem::path& path) {
    std::ofstream file(path);
    if (!file) {
        throw JsonError(JsonErrorCode::SystemError, "Failed to open file: " + path.string());
    }
    dump_to_stream(json, file);
    file.close();
    if (!file) {
        throw JsonError(JsonErrorCode::SystemError, "Failed to write to file: " + path.string());
    }
}

void JsonDumper::set_format(JsonDumpFormat format) {
    format_ = format;
    impl_ = std::make_unique<JsonDumperImpl>(format_, indent_size_, precision_);
}

void JsonDumper::set_indent_size(int size) {
    indent_size_ = size;
    impl_ = std::make_unique<JsonDumperImpl>(format_, indent_size_, precision_);
}

void JsonDumper::set_precision(int precision) {
    precision_ = precision;
    impl_ = std::make_unique<JsonDumperImpl>(format_, indent_size_, precision_);
}

// Convenience functions
std::string json_dump(const JsonValue& json, JsonDumpFormat format) {
    JsonDumper dumper(format);
    return dumper.dump(json);
}

void json_dump_to_file(const JsonValue& json, const std::filesystem::path& path,
                      JsonDumpFormat format) {
    JsonDumper dumper(format);
    dumper.dump_to_file(json, path);
}

} // namespace jasson
