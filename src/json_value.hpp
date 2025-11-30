#ifndef JASSON_JSON_VALUE_HPP
#define JASSON_JSON_VALUE_HPP

#include "json_types.hpp"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <initializer_list>
#include <stdexcept>

namespace jasson {

// Forward declarations
class JsonValue;
class JsonObject;
class JsonArray;
class JsonString;
class JsonInteger;
class JsonReal;
class JsonBoolean;
class JsonNull;

// Smart pointer types
using JsonValuePtr = std::shared_ptr<JsonValue>;
using JsonObjectPtr = std::shared_ptr<JsonObject>;
using JsonArrayPtr = std::shared_ptr<JsonArray>;
using JsonStringPtr = std::shared_ptr<JsonString>;
using JsonIntegerPtr = std::shared_ptr<JsonInteger>;
using JsonRealPtr = std::shared_ptr<JsonReal>;
using JsonBooleanPtr = std::shared_ptr<JsonBoolean>;
using JsonNullPtr = std::shared_ptr<JsonNull>;

// Base JSON value class
class JsonValue {
public:
    explicit JsonValue(JsonType type) : type_(type) {}
    virtual ~JsonValue() = default;
    
    // Type checking
    JsonType type() const { return type_; }
    bool is_object() const { return type_ == JsonType::OBJECT; }
    bool is_array() const { return type_ == JsonType::ARRAY; }
    bool is_string() const { return type_ == JsonType::STRING; }
    bool is_integer() const { return type_ == JsonType::INTEGER; }
    bool is_real() const { return type_ == JsonType::REAL; }
    bool is_number() const { return is_integer() || is_real(); }
    bool is_boolean() const { return type_ == JsonType::BOOLEAN; }
    bool is_null() const { return type_ == JsonType::NULL_VALUE; }
    
    // Safe casting
    JsonObject& as_object();
    const JsonObject& as_object() const;
    JsonArray& as_array();
    const JsonArray& as_array() const;
    JsonString& as_string();
    const JsonString& as_string() const;
    JsonInteger& as_integer();
    const JsonInteger& as_integer() const;
    JsonReal& as_real();
    const JsonReal& as_real() const;
    JsonBoolean& as_boolean();
    const JsonBoolean& as_boolean() const;
    JsonNull& as_null();
    const JsonNull& as_null() const;
    
    // Virtual clone method for copying
    virtual std::unique_ptr<JsonValue> clone() const = 0;
    
    // Virtual equality check
    virtual bool equals(const JsonValue& other) const {
        return type_ == other.type_;
    }
    
    // Virtual comparison for ordering
    virtual int compare(const JsonValue& other) const {
        return static_cast<int>(type_) - static_cast<int>(other.type_);
    }
    
    // Virtual hash function
    virtual size_t hash() const {
        return std::hash<int>{}(static_cast<int>(type_));
    }

private:
    JsonType type_;
};

// JSON object class
class JsonObject : public JsonValue {
public:
    using ValueType = std::unique_ptr<JsonValue>;
    using ContainerType = std::unordered_map<std::string, ValueType>;
    using iterator = ContainerType::iterator;
    using const_iterator = ContainerType::const_iterator;
    
    JsonObject() : JsonValue(JsonType::OBJECT) {}
    
    // Size and capacity
    size_t size() const { return values_.size(); }
    bool empty() const { return values_.empty(); }
    void clear() { values_.clear(); }
    
    // Element access
    JsonValue* get(const std::string& key) {
        auto it = values_.find(key);
        return it != values_.end() ? it->second.get() : nullptr;
    }
    
    const JsonValue* get(const std::string& key) const {
        auto it = values_.find(key);
        return it != values_.end() ? it->second.get() : nullptr;
    }
    
    JsonValue& at(const std::string& key) {
        auto it = values_.find(key);
        if (it == values_.end()) {
            throw std::out_of_range("Key not found: " + key);
        }
        return *it->second;
    }
    
    const JsonValue& at(const std::string& key) const {
        auto it = values_.find(key);
        if (it == values_.end()) {
            throw std::out_of_range("Key not found: " + key);
        }
        return *it->second;
    }
    
    // Modifiers
    void set(const std::string& key, ValueType value) {
        values_[key] = std::move(value);
    }
    
    void set(const std::string& key, JsonValuePtr value) {
        values_[key] = value->clone();
    }
    
    int del(const std::string& key) {
        return values_.erase(key);
    }
    
    // Iterators
    iterator begin() { return values_.begin(); }
    iterator end() { return values_.end(); }
    const_iterator begin() const { return values_.begin(); }
    const_iterator end() const { return values_.end(); }
    
    // Lookup
    bool contains(const std::string& key) const {
        return values_.find(key) != values_.end();
    }
    
    // Equality
    bool equals(const JsonValue& other) const override {
        if (!JsonValue::equals(other)) return false;
        const auto& obj = static_cast<const JsonObject&>(other);
        if (size() != obj.size()) return false;
        for (const auto& [key, value] : values_) {
            auto it = obj.values_.find(key);
            if (it == obj.values_.end() || !value->equals(*it->second)) {
                return false;
            }
        }
        return true;
    }
    
    // Clone
    std::unique_ptr<JsonValue> clone() const override {
        auto obj = std::make_unique<JsonObject>();
        for (const auto& [key, value] : values_) {
            obj->set(key, value->clone());
        }
        return obj;
    }
    
    // Hash
    size_t hash() const override {
        size_t h = JsonValue::hash();
        for (const auto& [key, value] : values_) {
            h ^= std::hash<std::string>{}(key) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= value->hash() + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }

private:
    ContainerType values_;
};

// JSON array class
class JsonArray : public JsonValue {
public:
    using ValueType = std::unique_ptr<JsonValue>;
    using ContainerType = std::vector<ValueType>;
    using iterator = ContainerType::iterator;
    using const_iterator = ContainerType::const_iterator;
    
    JsonArray() : JsonValue(JsonType::ARRAY) {}
    
    // Size and capacity
    size_t size() const { return values_.size(); }
    bool empty() const { return values_.empty(); }
    void clear() { values_.clear(); }
    void reserve(size_t capacity) { values_.reserve(capacity); }
    
    // Element access
    JsonValue* at(size_t index) {
        if (index >= values_.size()) {
            throw std::out_of_range("Index out of range");
        }
        return values_[index].get();
    }
    
    const JsonValue* at(size_t index) const {
        if (index >= values_.size()) {
            throw std::out_of_range("Index out of range");
        }
        return values_[index].get();
    }
    
    JsonValue& operator[](size_t index) {
        return *values_[index];
    }
    
    const JsonValue& operator[](size_t index) const {
        return *values_[index];
    }
    
    // Modifiers
    void append(ValueType value) {
        values_.push_back(std::move(value));
    }
    
    void append(JsonValuePtr value) {
        values_.push_back(value->clone());
    }
    
    void insert(size_t index, ValueType value) {
        if (index > values_.size()) {
            throw std::out_of_range("Index out of range");
        }
        values_.insert(values_.begin() + index, std::move(value));
    }
    
    void erase(size_t index) {
        if (index >= values_.size()) {
            throw std::out_of_range("Index out of range");
        }
        values_.erase(values_.begin() + index);
    }
    
    // Iterators
    iterator begin() { return values_.begin(); }
    iterator end() { return values_.end(); }
    const_iterator begin() const { return values_.begin(); }
    const_iterator end() const { return values_.end(); }
    
    // Equality
    bool equals(const JsonValue& other) const override {
        if (!JsonValue::equals(other)) return false;
        const auto& arr = static_cast<const JsonArray&>(other);
        if (size() != arr.size()) return false;
        for (size_t i = 0; i < size(); ++i) {
            if (!values_[i]->equals(*arr.values_[i])) {
                return false;
            }
        }
        return true;
    }
    
    // Clone
    std::unique_ptr<JsonValue> clone() const override {
        auto arr = std::make_unique<JsonArray>();
        for (const auto& value : values_) {
            arr->append(value->clone());
        }
        return arr;
    }
    
    // Hash
    size_t hash() const override {
        size_t h = JsonValue::hash();
        for (const auto& value : values_) {
            h ^= value->hash() + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }

private:
    ContainerType values_;
};

// JSON string class
class JsonString : public JsonValue {
public:
    explicit JsonString(const std::string& value) : JsonValue(JsonType::STRING), value_(value) {}
    explicit JsonString(std::string&& value) : JsonValue(JsonType::STRING), value_(std::move(value)) {}
    
    const std::string& value() const { return value_; }
    std::string& value() { return value_; }
    void set_value(const std::string& value) { value_ = value; }
    void set_value(std::string&& value) { value_ = std::move(value); }
    
    // Equality
    bool equals(const JsonValue& other) const override {
        if (!JsonValue::equals(other)) return false;
        const auto& str = static_cast<const JsonString&>(other);
        return value_ == str.value_;
    }
    
    // Comparison
    int compare(const JsonValue& other) const override {
        int base = JsonValue::compare(other);
        if (base != 0) return base;
        const auto& str = static_cast<const JsonString&>(other);
        return value_.compare(str.value_);
    }
    
    // Clone
    std::unique_ptr<JsonValue> clone() const override {
        return std::make_unique<JsonString>(value_);
    }
    
    // Hash
    size_t hash() const override {
        return JsonValue::hash() ^ std::hash<std::string>{}(value_);
    }

private:
    std::string value_;
};

// JSON integer class
class JsonInteger : public JsonValue {
public:
    explicit JsonInteger(json_int_t value) : JsonValue(JsonType::INTEGER), value_(value) {}
    
    json_int_t value() const { return value_; }
    void set_value(json_int_t value) { value_ = value; }
    
    // Equality
    bool equals(const JsonValue& other) const override {
        if (!JsonValue::equals(other)) return false;
        const auto& int_val = static_cast<const JsonInteger&>(other);
        return value_ == int_val.value_;
    }
    
    // Comparison
    int compare(const JsonValue& other) const override {
        int base = JsonValue::compare(other);
        if (base != 0) return base;
        const auto& int_val = static_cast<const JsonInteger&>(other);
        return value_ < int_val.value_ ? -1 : (value_ > int_val.value_ ? 1 : 0);
    }
    
    // Clone
    std::unique_ptr<JsonValue> clone() const override {
        return std::make_unique<JsonInteger>(value_);
    }
    
    // Hash
    size_t hash() const override {
        return JsonValue::hash() ^ std::hash<json_int_t>{}(value_);
    }

private:
    json_int_t value_;
};

// JSON real class
class JsonReal : public JsonValue {
public:
    explicit JsonReal(double value) : JsonValue(JsonType::REAL), value_(value) {}
    
    double value() const { return value_; }
    void set_value(double value) { value_ = value; }
    
    // Equality
    bool equals(const JsonValue& other) const override {
        if (!JsonValue::equals(other)) return false;
        const auto& real = static_cast<const JsonReal&>(other);
        return value_ == real.value_;
    }
    
    // Comparison
    int compare(const JsonValue& other) const override {
        int base = JsonValue::compare(other);
        if (base != 0) return base;
        const auto& real = static_cast<const JsonReal&>(other);
        return value_ < real.value_ ? -1 : (value_ > real.value_ ? 1 : 0);
    }
    
    // Clone
    std::unique_ptr<JsonValue> clone() const override {
        return std::make_unique<JsonReal>(value_);
    }
    
    // Hash
    size_t hash() const override {
        return JsonValue::hash() ^ std::hash<double>{}(value_);
    }

private:
    double value_;
};

// JSON boolean class
class JsonBoolean : public JsonValue {
public:
    explicit JsonBoolean(bool value) : JsonValue(JsonType::BOOLEAN), value_(value) {}
    
    bool value() const { return value_; }
    void set_value(bool value) { value_ = value; }
    
    // Equality
    bool equals(const JsonValue& other) const override {
        if (!JsonValue::equals(other)) return false;
        const auto& bool_val = static_cast<const JsonBoolean&>(other);
        return value_ == bool_val.value_;
    }
    
    // Comparison
    int compare(const JsonValue& other) const override {
        int base = JsonValue::compare(other);
        if (base != 0) return base;
        const auto& bool_val = static_cast<const JsonBoolean&>(other);
        return value_ == bool_val.value_ ? 0 : (value_ ? 1 : -1);
    }
    
    // Clone
    std::unique_ptr<JsonValue> clone() const override {
        return std::make_unique<JsonBoolean>(value_);
    }
    
    // Hash
    size_t hash() const override {
        return JsonValue::hash() ^ std::hash<bool>{}(value_);
    }

private:
    bool value_;
};

// JSON null class
class JsonNull : public JsonValue {
public:
    JsonNull() : JsonValue(JsonType::NULL_VALUE) {}
    
    // Equality
    bool equals(const JsonValue& other) const override {
        return JsonValue::equals(other);
    }
    
    // Clone
    std::unique_ptr<JsonValue> clone() const override {
        return std::make_unique<JsonNull>();
    }
};

// Comparison operators
inline bool operator==(const JsonValue& a, const JsonValue& b) {
    return a.equals(b);
}

inline bool operator!=(const JsonValue& a, const JsonValue& b) {
    return !a.equals(b);
}

inline bool operator<(const JsonValue& a, const JsonValue& b) {
    return a.compare(b) < 0;
}

inline bool operator<=(const JsonValue& a, const JsonValue& b) {
    return a.compare(b) <= 0;
}

inline bool operator>(const JsonValue& a, const JsonValue& b) {
    return a.compare(b) > 0;
}

inline bool operator>=(const JsonValue& a, const JsonValue& b) {
    return a.compare(b) >= 0;
}

} // namespace jasson

#endif // JASSON_JSON_VALUE_HPP
