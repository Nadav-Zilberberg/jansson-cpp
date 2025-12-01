#ifndef JSON_VALUE_HPP
#define JSON_VALUE_HPP

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

namespace jasson {

// Forward declarations
class JsonValue;
class JsonString;
class JsonInteger;
class JsonReal;
class JsonBoolean;
class JsonNull;
class JsonArray;
class JsonObject;

// Type aliases
using JsonValuePtr = std::shared_ptr<JsonValue>;
using json_int_t = long long;

// Base JSON value class
class JsonValue {
public:
    virtual ~JsonValue() = default;
    
    // Type checking
    virtual bool is_string() const { return false; }
    virtual bool is_integer() const { return false; }
    virtual bool is_real() const { return false; }
    virtual bool is_number() const { return false; }
    virtual bool is_boolean() const { return false; }
    virtual bool is_null() const { return false; }
    virtual bool is_array() const { return false; }
    virtual bool is_object() const { return false; }
    
    // Type casting
    virtual JsonString* as_string() { return nullptr; }
    virtual JsonInteger* as_integer() { return nullptr; }
    virtual JsonReal* as_real() { return nullptr; }
    virtual JsonBoolean* as_boolean() { return nullptr; }
    virtual JsonNull* as_null() { return nullptr; }
    virtual JsonArray* as_array() { return nullptr; }
    virtual JsonObject* as_object() { return nullptr; }
    
    // Serialization
    virtual std::string to_string() const = 0;
    virtual std::string to_pretty_string(int indent = 0) const = 0;
    
    // Cloning
    virtual JsonValuePtr clone() const = 0;
    virtual JsonValuePtr deep_clone() const = 0;
    
    // Equality
    virtual bool equals(const JsonValue& other) const = 0;
};

// String type
class JsonString : public JsonValue {
private:
    std::string value_;
    
public:
    explicit JsonString(const std::string& value) : value_(value) {}
    explicit JsonString(std::string&& value) : value_(std::move(value)) {}
    
    bool is_string() const override { return true; }
    JsonString* as_string() override { return this; }
    
    const std::string& value() const { return value_; }
    std::string& value() { return value_; }
    
    std::string to_string() const override {
        std::ostringstream oss;
        oss << std::quoted(value_);
        return oss.str();
    }
    
    std::string to_pretty_string(int indent = 0) const override {
        return to_string();
    }
    
    JsonValuePtr clone() const override {
        return std::make_unique<JsonString>(value_);
    }
    
    JsonValuePtr deep_clone() const override {
        return clone();
    }
    
    bool equals(const JsonValue& other) const override {
        if (!other.is_string()) return false;
        return value_ == static_cast<const JsonString&>(other).value_;
    }
};

// Integer type
class JsonInteger : public JsonValue {
private:
    json_int_t value_;
    
public:
    explicit JsonInteger(json_int_t value) : value_(value) {}
    
    bool is_integer() const override { return true; }
    bool is_number() const override { return true; }
    JsonInteger* as_integer() override { return this; }
    
    json_int_t value() const { return value_; }
    
    std::string to_string() const override {
        return std::to_string(value_);
    }
    
    std::string to_pretty_string(int indent = 0) const override {
        return to_string();
    }
    
    JsonValuePtr clone() const override {
        return std::make_unique<JsonInteger>(value_);
    }
    
    JsonValuePtr deep_clone() const override {
        return clone();
    }
    
    bool equals(const JsonValue& other) const override {
        if (!other.is_integer()) return false;
        return value_ == static_cast<const JsonInteger&>(other).value_;
    }
};

// Real number type
class JsonReal : public JsonValue {
private:
    double value_;
    
public:
    explicit JsonReal(double value) : value_(value) {}
    
    bool is_real() const override { return true; }
    bool is_number() const override { return true; }
    JsonReal* as_real() override { return this; }
    
    double value() const { return value_; }
    
    std::string to_string() const override {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6) << value_;
        std::string result = oss.str();
        // Remove trailing zeros
        result.erase(result.find_last_not_of('0') + 1, std::string::npos);
        result.erase(result.find_last_not_of('.') + 1, std::string::npos);
        return result;
    }
    
    std::string to_pretty_string(int indent = 0) const override {
        return to_string();
    }
    
    JsonValuePtr clone() const override {
        return std::make_unique<JsonReal>(value_);
    }
    
    JsonValuePtr deep_clone() const override {
        return clone();
    }
    
    bool equals(const JsonValue& other) const override {
        if (!other.is_real()) return false;
        return std::abs(value_ - static_cast<const JsonReal&>(other).value_) < 1e-9;
    }
};

// Boolean type
class JsonBoolean : public JsonValue {
private:
    bool value_;
    
public:
    explicit JsonBoolean(bool value) : value_(value) {}
    
    bool is_boolean() const override { return true; }
    JsonBoolean* as_boolean() override { return this; }
    
    bool value() const { return value_; }
    
    std::string to_string() const override {
        return value_ ? "true" : "false";
    }
    
    std::string to_pretty_string(int indent = 0) const override {
        return to_string();
    }
    
    JsonValuePtr clone() const override {
        return std::make_unique<JsonBoolean>(value_);
    }
    
    JsonValuePtr deep_clone() const override {
        return clone();
    }
    
    bool equals(const JsonValue& other) const override {
        if (!other.is_boolean()) return false;
        return value_ == static_cast<const JsonBoolean&>(other).value_;
    }
};

// Null type
class JsonNull : public JsonValue {
public:
    JsonNull() = default;
    
    bool is_null() const override { return true; }
    JsonNull* as_null() override { return this; }
    
    std::string to_string() const override {
        return "null";
    }
    
    std::string to_pretty_string(int indent = 0) const override {
        return to_string();
    }
    
    JsonValuePtr clone() const override {
        return std::make_unique<JsonNull>();
    }
    
    JsonValuePtr deep_clone() const override {
        return clone();
    }
    
    bool equals(const JsonValue& other) const override {
        return other.is_null();
    }
};

// Array type
class JsonArray : public JsonValue {
private:
    std::vector<JsonValuePtr> values_;
    
public:
    JsonArray() = default;
    
    bool is_array() const override { return true; }
    JsonArray* as_array() override { return this; }
    
    // Size and access
    size_t size() const { return values_.size(); }
    bool empty() const { return values_.empty(); }
    
    JsonValuePtr at(size_t index) const {
        if (index >= values_.size()) {
            throw std::out_of_range("Index out of range");
        }
        return values_[index];
    }
    
    JsonValuePtr operator[](size_t index) const {
        return at(index);
    }
    
    // Modification
    void append(JsonValuePtr value) {
        values_.push_back(value->clone());
    }
    
    void append_value(JsonValuePtr value) {
        values_.push_back(value->clone());
    }
    
    void insert(size_t index, JsonValuePtr value) {
        if (index > values_.size()) {
            throw std::out_of_range("Index out of range");
        }
        values_.insert(values_.begin() + index, value->clone());
    }
    
    void erase(size_t index) {
        if (index >= values_.size()) {
            throw std::out_of_range("Index out of range");
        }
        values_.erase(values_.begin() + index);
    }
    
    void clear() {
        values_.clear();
    }
    
    // Iterators
    using iterator = std::vector<JsonValuePtr>::iterator;
    using const_iterator = std::vector<JsonValuePtr>::const_iterator;
    
    iterator begin() { return values_.begin(); }
    iterator end() { return values_.end(); }
    const_iterator begin() const { return values_.begin(); }
    const_iterator end() const { return values_.end(); }
    
    std::string to_string() const override {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < values_.size(); ++i) {
            if (i > 0) oss << ",";
            oss << values_[i]->to_string();
        }
        oss << "]";
        return oss.str();
    }
    
    std::string to_pretty_string(int indent = 0) const override {
        std::ostringstream oss;
        oss << "[";
        if (!values_.empty()) {
            oss << "\n";
            for (size_t i = 0; i < values_.size(); ++i) {
                oss << std::string(indent + 2, ' ');
                oss << values_[i]->to_pretty_string(indent + 2);
                if (i < values_.size() - 1) oss << ",";
                oss << "\n";
            }
            oss << std::string(indent, ' ') << "]";
        } else {
            oss << "]";
        }
        return oss.str();
    }
    
    JsonValuePtr clone() const override {
        auto result = std::make_unique<JsonArray>();
        for (const auto& value : values_) {
            result->append(value);
        }
        return result;
    }
    
    JsonValuePtr deep_clone() const override {
        auto result = std::make_unique<JsonArray>();
        for (const auto& value : values_) {
            result->append(value->deep_clone());
        }
        return result;
    }
    
    bool equals(const JsonValue& other) const override {
        if (!other.is_array()) return false;
        const auto& other_array = static_cast<const JsonArray&>(other);
        if (size() != other_array.size()) return false;
        for (size_t i = 0; i < size(); ++i) {
            if (!values_[i]->equals(*other_array.values_[i])) return false;
        }
        return true;
    }
};

// Object type
class JsonObject : public JsonValue {
private:
    std::map<std::string, JsonValuePtr> values_;
    
public:
    JsonObject() = default;
    
    bool is_object() const override { return true; }
    JsonObject* as_object() override { return this; }
    
    // Size and access
    size_t size() const { return values_.size(); }
    bool empty() const { return values_.empty(); }
    
    bool has_key(const std::string& key) const {
        return values_.find(key) != values_.end();
    }
    
    JsonValuePtr get(const std::string& key) const {
        auto it = values_.find(key);
        if (it == values_.end()) {
            throw std::out_of_range("Key not found: " + key);
        }
        return it->second;
    }
    
    JsonValuePtr operator[](const std::string& key) const {
        return get(key);
    }
    
    // Modification
    void set(const std::string& key, JsonValuePtr value) {
        values_[key] = value->clone();
    }
    
    void remove(const std::string& key) {
        values_.erase(key);
    }
    
    void clear() {
        values_.clear();
    }
    
    // Add a key-value pair to the object
    void add(const std::string& key, JsonValuePtr value) {
        values_[key] = value->clone();
    }
    
    // Convenience overloads for common types
    void add(const std::string& key, const std::string& value) {
        values_[key] = std::make_unique<JsonString>(value);
    }
    
    void add(const std::string& key, const char* value) {
        values_[key] = std::make_unique<JsonString>(value);
    }
    
    void add(const std::string& key, json_int_t value) {
        values_[key] = std::make_unique<JsonInteger>(value);
    }
    
    void add(const std::string& key, double value) {
        values_[key] = std::make_unique<JsonReal>(value);
    }
    
    void add(const std::string& key, bool value) {
        values_[key] = std::make_unique<JsonBoolean>(value);
    }
    
    void add_null(const std::string& key) {
        values_[key] = std::make_unique<JsonNull>();
    }
    
    // Iterators
    using iterator = std::map<std::string, JsonValuePtr>::iterator;
    using const_iterator = std::map<std::string, JsonValuePtr>::const_iterator;
    
    iterator begin() { return values_.begin(); }
    iterator end() { return values_.end(); }
    const_iterator begin() const { return values_.begin(); }
    const_iterator end() const { return values_.end(); }
    
    std::string to_string() const override {
        std::ostringstream oss;
        oss << "{";
        bool first = true;
        for (const auto& [key, value] : values_) {
            if (!first) oss << ",";
            first = false;
            oss << std::quoted(key) << ":" << value->to_string();
        }
        oss << "}";
        return oss.str();
    }
    
    std::string to_pretty_string(int indent = 0) const override {
        std::ostringstream oss;
        oss << "{";
        if (!values_.empty()) {
            oss << "\n";
            bool first = true;
            for (const auto& [key, value] : values_) {
                if (!first) oss << ",\n";
                first = false;
                oss << std::string(indent + 2, ' ');
                oss << std::quoted(key) << ": ";
                oss << value->to_pretty_string(indent + 2);
            }
            oss << "\n" << std::string(indent, ' ') << "}";
        } else {
            oss << "}";
        }
        return oss.str();
    }
    
    JsonValuePtr clone() const override {
        auto result = std::make_unique<JsonObject>();
        for (const auto& [key, value] : values_) {
            result->set(key, value);
        }
        return result;
    }
    
    JsonValuePtr deep_clone() const override {
        auto result = std::make_unique<JsonObject>();
        for (const auto& [key, value] : values_) {
            result->set(key, value->deep_clone());
        }
        return result;
    }
    
    bool equals(const JsonValue& other) const override {
        if (!other.is_object()) return false;
        const auto& other_object = static_cast<const JsonObject&>(other);
        if (size() != other_object.size()) return false;
        for (const auto& [key, value] : values_) {
            if (!other_object.has_key(key)) return false;
            if (!value->equals(*other_object.get(key))) return false;
        }
        return true;
    }
};

} // namespace jasson

#endif // JSON_VALUE_HPP
