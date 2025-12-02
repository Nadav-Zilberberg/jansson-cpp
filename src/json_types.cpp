#include "json_value.hpp"
#include <memory>
#include <algorithm>

namespace jasson {

// JsonObject implementation
std::shared_ptr<JsonValue> JsonObject::copy() const {
    auto result = std::make_shared<JsonObject>();
    for (const auto& [key, value] : values_) {
        result->set(key, value->copy());
    }
    return result;
}

std::shared_ptr<JsonValue> JsonObject::deep_copy() const {
    auto result = std::make_shared<JsonObject>();
    for (const auto& [key, value] : values_) {
        result->set(key, value->deep_copy());
    }
    return result;
}

bool JsonObject::equals(const JsonValue& other) const {
    if (!other.is_object()) return false;
    
    const auto& obj = static_cast<const JsonObject&>(other);
    if (size() != obj.size()) return false;
    
    for (const auto& [key, value] : values_) {
        auto other_value = obj.get(key);
        if (!other_value || !value->equals(*other_value)) {
            return false;
        }
    }
    return true;
}

// JsonArray implementation
std::shared_ptr<JsonValue> JsonArray::copy() const {
    auto result = std::make_shared<JsonArray>();
    for (const auto& value : values_) {
        result->append(value->copy());
    }
    return result;
}

std::shared_ptr<JsonValue> JsonArray::deep_copy() const {
    auto result = std::make_shared<JsonArray>();
    for (const auto& value : values_) {
        result->append(value->deep_copy());
    }
    return result;
}

bool JsonArray::equals(const JsonValue& other) const {
    if (!other.is_array()) return false;
    
    const auto& arr = static_cast<const JsonArray&>(other);
    if (size() != arr.size()) return false;
    
    for (size_t i = 0; i < size(); ++i) {
        if (!values_[i]->equals(*arr.values_[i])) {
            return false;
        }
    }
    return true;
}

// JsonString implementation
std::shared_ptr<JsonValue> JsonString::copy() const {
    return std::make_shared<JsonString>(value_);
}

std::shared_ptr<JsonValue> JsonString::deep_copy() const {
    return copy(); // Strings are immutable, so deep copy is same as shallow copy
}

bool JsonString::equals(const JsonValue& other) const {
    if (!other.is_string()) return false;
    return value_ == static_cast<const JsonString&>(other).value_;
}

// JsonNumber implementation
std::shared_ptr<JsonValue> JsonNumber::copy() const {
    if (is_integer()) {
        return std::make_shared<JsonNumber>(int_value_);
    } else {
        return std::make_shared<JsonNumber>(real_value_);
    }
}

std::shared_ptr<JsonValue> JsonNumber::deep_copy() const {
    return copy(); // Numbers are immutable, so deep copy is same as shallow copy
}

bool JsonNumber::equals(const JsonValue& other) const {
    if (!other.is_number()) return false;
    
    const auto& num = static_cast<const JsonNumber&>(other);
    if (is_integer() && num.is_integer()) {
        return int_value_ == num.int_value_;
    } else if (is_real() && num.is_real()) {
        return real_value_ == num.real_value_;
    } else if (is_integer() && num.is_real()) {
        return static_cast<double>(int_value_) == num.real_value_;
    } else if (is_real() && num.is_integer()) {
        return real_value_ == static_cast<double>(num.int_value_);
    }
    return false;
}

// JsonBoolean implementation
std::shared_ptr<JsonValue> JsonBoolean::copy() const {
    return std::make_shared<JsonBoolean>(value_);
}

std::shared_ptr<JsonValue> JsonBoolean::deep_copy() const {
    return copy(); // Booleans are immutable, so deep copy is same as shallow copy
}

bool JsonBoolean::equals(const JsonValue& other) const {
    if (!other.is_boolean()) return false;
    return value_ == static_cast<const JsonBoolean&>(other).value_;
}

// JsonNull implementation
std::shared_ptr<JsonValue> JsonNull::copy() const {
    return std::make_shared<JsonNull>();
}

std::shared_ptr<JsonValue> JsonNull::deep_copy() const {
    return copy(); // Null is immutable, so deep copy is same as shallow copy
}

bool JsonNull::equals(const JsonValue& other) const {
    return other.is_null();
}

} // namespace jasson
