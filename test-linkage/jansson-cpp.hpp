#ifndef JANSSON_CPP_HPP
#define JANSSON_CPP_HPP

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

// Forward declaration for C json_t type
struct json_t;

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
    
    virtual const JsonString* as_string() const { return nullptr; }
    virtual const JsonInteger* as_integer() const { return nullptr; }
    virtual const JsonReal* as_real() const { return nullptr; }
    virtual const JsonBoolean* as_boolean() const { return nullptr; }
    virtual const JsonNull* as_null() const { return nullptr; }
    virtual const JsonArray* as_array() const { return nullptr; }
    virtual const JsonObject* as_object() const { return nullptr; }
    
    // Serialization
    virtual std::string dump(int indent = -1) const = 0;
    virtual json_t* to_c_json() const = 0;
    
    // String representation
    virtual std::string to_string() const = 0;
};

// JSON String
class JsonString : public JsonValue {
private:
    std::string value_;
    
public:
    explicit JsonString(const std::string& value) : value_(value) {}
    explicit JsonString(const char* value) : value_(value ? value : "") {}
    
    bool is_string() const override { return true; }
    JsonString* as_string() override { return this; }
    const JsonString* as_string() const override { return this; }
    
    const std::string& value() const { return value_; }
    void set_value(const std::string& value) { value_ = value; }
    
    std::string dump(int indent = -1) const override;
    json_t* to_c_json() const override;
    std::string to_string() const override { return value_; }
};

// JSON Integer
class JsonInteger : public JsonValue {
private:
    json_int_t value_;
    
public:
    explicit JsonInteger(json_int_t value) : value_(value) {}
    
    bool is_integer() const override { return true; }
    bool is_number() const override { return true; }
    JsonInteger* as_integer() override { return this; }
    const JsonInteger* as_integer() const override { return this; }
    
    json_int_t value() const { return value_; }
    void set_value(json_int_t value) { value_ = value; }
    
    std::string dump(int indent = -1) const override;
    json_t* to_c_json() const override;
    std::string to_string() const override { return std::to_string(value_); }
};

// JSON Real
class JsonReal : public JsonValue {
private:
    double value_;
    
public:
    explicit JsonReal(double value) : value_(value) {}
    
    bool is_real() const override { return true; }
    bool is_number() const override { return true; }
    JsonReal* as_real() override { return this; }
    const JsonReal* as_real() const override { return this; }
    
    double value() const { return value_; }
    void set_value(double value) { value_ = value; }
    
    std::string dump(int indent = -1) const override;
    json_t* to_c_json() const override;
    std::string to_string() const override { return std::to_string(value_); }
};

// JSON Boolean
class JsonBoolean : public JsonValue {
private:
    bool value_;
    
public:
    explicit JsonBoolean(bool value) : value_(value) {}
    
    bool is_boolean() const override { return true; }
    JsonBoolean* as_boolean() override { return this; }
    const JsonBoolean* as_boolean() const override { return this; }
    
    bool value() const { return value_; }
    void set_value(bool value) { value_ = value; }
    
    std::string dump(int indent = -1) const override;
    json_t* to_c_json() const override;
    std::string to_string() const override { return value_ ? "true" : "false"; }
};

// JSON Null
class JsonNull : public JsonValue {
public:
    JsonNull() = default;
    
    bool is_null() const override { return true; }
    JsonNull* as_null() override { return this; }
    const JsonNull* as_null() const override { return this; }
    
    std::string dump(int indent = -1) const override;
    json_t* to_c_json() const override;
    std::string to_string() const override { return "null"; }
};

// JSON Array
class JsonArray : public JsonValue {
private:
    std::vector<JsonValuePtr> elements_;
    
public:
    JsonArray() = default;
    
    bool is_array() const override { return true; }
    JsonArray* as_array() override { return this; }
    const JsonArray* as_array() const override { return this; }
    
    void append(JsonValuePtr value) { elements_.push_back(value); }
    void append(const std::string& value) { append(std::make_shared<JsonString>(value)); }
    void append(json_int_t value) { append(std::make_shared<JsonInteger>(value)); }
    void append(double value) { append(std::make_shared<JsonReal>(value)); }
    void append(bool value) { append(std::make_shared<JsonBoolean>(value)); }
    void append_null() { append(std::make_shared<JsonNull>()); }
    
    size_t size() const { return elements_.size(); }
    JsonValuePtr at(size_t index) const { 
        if (index >= elements_.size()) {
            throw std::out_of_range("Array index out of range");
        }
        return elements_[index]; 
    }
    
    std::string dump(int indent = -1) const override;
    json_t* to_c_json() const override;
    std::string to_string() const override { return dump(); }
};

// JSON Object
class JsonObject : public JsonValue {
private:
    std::map<std::string, JsonValuePtr> members_;
    
public:
    JsonObject() = default;
    
    bool is_object() const override { return true; }
    JsonObject* as_object() override { return this; }
    const JsonObject* as_object() const override { return this; }
    
    void set(const std::string& key, JsonValuePtr value) { members_[key] = value; }
    void set(const std::string& key, const std::string& value) { set(key, std::make_shared<JsonString>(value)); }
    void set(const std::string& key, json_int_t value) { set(key, std::make_shared<JsonInteger>(value)); }
    void set(const std::string& key, double value) { set(key, std::make_shared<JsonReal>(value)); }
    void set(const std::string& key, bool value) { set(key, std::make_shared<JsonBoolean>(value)); }
    void set_null(const std::string& key) { set(key, std::make_shared<JsonNull>()); }
    
    bool has(const std::string& key) const { return members_.find(key) != members_.end(); }
    JsonValuePtr get(const std::string& key) const {
        auto it = members_.find(key);
        if (it == members_.end()) {
            throw std::out_of_range("Object member not found: " + key);
        }
        return it->second;
    }
    
    size_t size() const { return members_.size(); }
    
    std::string dump(int indent = -1) const override;
    json_t* to_c_json() const override;
    std::string to_string() const override { return dump(); }
};

// Factory functions
inline JsonValuePtr make_null() { return std::make_shared<JsonNull>(); }
inline JsonValuePtr make_bool(bool value) { return std::make_shared<JsonBoolean>(value); }
inline JsonValuePtr make_int(json_int_t value) { return std::make_shared<JsonInteger>(value); }
inline JsonValuePtr make_real(double value) { return std::make_shared<JsonReal>(value); }
inline JsonValuePtr make_string(const std::string& value) { return std::make_shared<JsonString>(value); }
inline JsonValuePtr make_array() { return std::make_shared<JsonArray>(); }
inline JsonValuePtr make_object() { return std::make_shared<JsonObject>(); }

} // namespace jasson

#endif // JANSSON_CPP_HPP
