#ifndef JASSON_JSON_VALUE_HPP
#define JASSON_JSON_VALUE_HPP

#include <memory>
#include <variant>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

namespace jasson {

enum class JsonType {
    OBJECT,
    ARRAY,
    STRING,
    INTEGER,
    REAL,
    TRUE,
    FALSE,
    NULL_VALUE
};

class JsonValue : public std::enable_shared_from_this<JsonValue> {
public:
    explicit JsonValue(JsonType type) : type_(type) {}
    virtual ~JsonValue() = default;

    JsonType type() const { return type_; }
    
    virtual std::shared_ptr<JsonValue> copy() const = 0;
    virtual std::shared_ptr<JsonValue> deep_copy() const = 0;
    virtual bool equals(const JsonValue& other) const = 0;

    // Type checking
    bool is_object() const { return type_ == JsonType::OBJECT; }
    bool is_array() const { return type_ == JsonType::ARRAY; }
    bool is_string() const { return type_ == JsonType::STRING; }
    bool is_integer() const { return type_ == JsonType::INTEGER; }
    bool is_real() const { return type_ == JsonType::REAL; }
    bool is_number() const { return is_integer() || is_real(); }
    bool is_true() const { return type_ == JsonType::TRUE; }
    bool is_false() const { return type_ == JsonType::FALSE; }
    bool is_boolean() const { return is_true() || is_false(); }
    bool is_null() const { return type_ == JsonType::NULL_VALUE; }

protected:
    JsonType& mutable_type() { return type_; }

private:
    JsonType type_;
};

using JsonValuePtr = std::shared_ptr<JsonValue>;

} // namespace jasson

#endif // JASSON_JSON_VALUE_HPP
