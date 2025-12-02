#ifndef JSON_FACTORY_HPP
#define JSON_FACTORY_HPP

#include "json_value.hpp"
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace jasson {

// Forward declaration
using JsonValuePtr = std::shared_ptr<JsonValue>;

// Factory functions for creating JSON values
inline JsonValuePtr jasson_null() {
    return std::make_unique<JsonNull>();
}

inline JsonValuePtr jasson_true() {
    return std::make_unique<JsonBoolean>(true);
}

inline JsonValuePtr jasson_false() {
    return std::make_unique<JsonBoolean>(false);
}

inline JsonValuePtr jasson_boolean(bool value) {
    return std::make_unique<JsonBoolean>(value);
}

inline JsonValuePtr jasson_integer(json_int_t value) {
    return std::make_unique<JsonInteger>(value);
}

inline JsonValuePtr jasson_real(double value) {
    return std::make_unique<JsonReal>(value);
}

inline JsonValuePtr jasson_string(const std::string& value) {
    return std::make_unique<JsonString>(value);
}

inline JsonValuePtr jasson_string(std::string&& value) {
    return std::make_unique<JsonString>(std::move(value));
}

inline JsonValuePtr jasson_array() {
    return std::make_unique<JsonArray>();
}

inline JsonValuePtr jasson_object() {
    return std::make_unique<JsonObject>();
}

// Builder class for fluent JSON construction
class JsonBuilder {
private:
    JsonValuePtr value_;
    
public:
    JsonBuilder() = default;
    
    explicit JsonBuilder(JsonValuePtr value) : value_(std::move(value)) {}
    
    // Array builder
    JsonBuilder& array() {
        value_ = jasson_array();
        return *this;
    }
    
    // Object builder
    JsonBuilder& object() {
        value_ = jasson_object();
        return *this;
    }
    
    // Add to object
    JsonBuilder& add(const std::string& key, JsonValuePtr value) {
        if (!value_ || !value_->is_object()) {
            throw std::runtime_error("Cannot add to non-object");
        }
        value_->as_object()->add(key, std::move(value));
        return *this;
    }
    
    // Convenience overloads for add
    JsonBuilder& add(const std::string& key, const std::string& value) {
        return add(key, jasson_string(value));
    }
    
    JsonBuilder& add(const std::string& key, const char* value) {
        return add(key, jasson_string(value));
    }
    
    JsonBuilder& add(const std::string& key, json_int_t value) {
        return add(key, jasson_integer(value));
    }
    
    JsonBuilder& add(const std::string& key, int value) {
        return add(key, jasson_integer(value));
    }
    
    JsonBuilder& add(const std::string& key, double value) {
        return add(key, jasson_real(value));
    }
    
    JsonBuilder& add(const std::string& key, bool value) {
        return add(key, jasson_boolean(value));
    }
    
    JsonBuilder& add_null(const std::string& key) {
        return add(key, jasson_null());
    }
    
    // Append to array
    JsonBuilder& append(JsonValuePtr value) {
        if (!value_ || !value_->is_array()) {
            throw std::runtime_error("Cannot append to non-array");
        }
        value_->as_array()->append(std::move(value));
        return *this;
    }
    
    // Convenience overloads for append
    JsonBuilder& append(const std::string& value) {
        return append(jasson_string(value));
    }
    
    JsonBuilder& append(const char* value) {
        return append(jasson_string(value));
    }
    
    JsonBuilder& append(json_int_t value) {
        return append(jasson_integer(value));
    }
    
    JsonBuilder& append(int value) {
        return append(jasson_integer(value));
    }
    
    JsonBuilder& append(double value) {
        return append(jasson_real(value));
    }
    
    JsonBuilder& append(bool value) {
        return append(jasson_boolean(value));
    }
    
    // Build the final value
    JsonValuePtr build() {
        return std::move(value_);
    }
    
    // Get current value
    JsonValuePtr value() const {
        return value_;
    }
};

// Convenience function to start building
inline JsonBuilder json_builder() {
    return JsonBuilder();
}

// Literal operators for JSON values
inline JsonValuePtr operator""_json(const char* str, size_t len) {
    return jasson_string(std::string(str, len));
}

inline JsonValuePtr operator""_json(long double value) {
    return jasson_real(static_cast<double>(value));
}

inline JsonValuePtr operator""_json(unsigned long long value) {
    return jasson_integer(static_cast<json_int_t>(value));
}

} // namespace jasson

#endif // JSON_FACTORY_HPP
