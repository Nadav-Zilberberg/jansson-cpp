#ifndef JANSSON_CPP_HPP
#define JANSSON_CPP_HPP

#include "jansson.h"
#include <memory>
#include <string>
#include <stdexcept>

namespace jansson_cpp {

class JsonValue {
private:
    json_t* value_;
    
public:
    JsonValue() : value_(nullptr) {}
    explicit JsonValue(json_t* value) : value_(value) {
        if (value_) json_incref(value_);
    }
    
    // Copy constructor
    JsonValue(const JsonValue& other) : value_(other.value_) {
        if (value_) json_incref(value_);
    }
    
    // Move constructor
    JsonValue(JsonValue&& other) noexcept : value_(other.value_) {
        other.value_ = nullptr;
    }
    
    // Destructor
    ~JsonValue() {
        if (value_) json_decref(value_);
    }
    
    // Assignment operators
    JsonValue& operator=(const JsonValue& other) {
        if (this != &other) {
            if (value_) json_decref(value_);
            value_ = other.value_;
            if (value_) json_incref(value_);
        }
        return *this;
    }
    
    JsonValue& operator=(JsonValue&& other) noexcept {
        if (this != &other) {
            if (value_) json_decref(value_);
            value_ = other.value_;
            other.value_ = nullptr;
        }
        return *this;
    }
    
    // Get the underlying json_t pointer
    json_t* get() const { return value_; }
    
    // Release ownership (caller must decref)
    json_t* release() {
        json_t* temp = value_;
        value_ = nullptr;
        return temp;
    }
    
    // Reset the value
    void reset(json_t* value = nullptr) {
        if (value_) json_decref(value_);
        value_ = value;
        if (value_) json_incref(value_);
    }
    
    // Check if value is valid
    explicit operator bool() const { return value_ != nullptr; }
    
    // Type checking
    bool is_object() const { return value_ && json_is_object(value_); }
    bool is_array() const { return value_ && json_is_array(value_); }
    bool is_string() const { return value_ && json_is_string(value_); }
    bool is_integer() const { return value_ && json_is_integer(value_); }
    bool is_real() const { return value_ && json_is_real(value_); }
    bool is_number() const { return value_ && json_is_number(value_); }
    bool is_boolean() const { return value_ && json_is_boolean(value_); }
    bool is_null() const { return value_ && json_is_null(value_); }
    
    // Get type (simplified - just return the type enum)
    json_type type() const {
        if (!value_) return JSON_NULL;
        if (json_is_object(value_)) return JSON_OBJECT;
        if (json_is_array(value_)) return JSON_ARRAY;
        if (json_is_string(value_)) return JSON_STRING;
        if (json_is_integer(value_)) return JSON_INTEGER;
        if (json_is_real(value_)) return JSON_REAL;
        if (json_is_true(value_)) return JSON_TRUE;
        if (json_is_false(value_)) return JSON_FALSE;
        if (json_is_null(value_)) return JSON_NULL;
        return JSON_NULL;
    }
};

// Convenience functions for creating JSON values
inline JsonValue make_object() { return JsonValue(json_object()); }
inline JsonValue make_array() { return JsonValue(json_array()); }
inline JsonValue make_string(const std::string& value) { return JsonValue(json_string(value.c_str())); }
inline JsonValue make_integer(json_int_t value) { return JsonValue(json_integer(value)); }
inline JsonValue make_real(json_real_t value) { return JsonValue(json_real(value)); }
inline JsonValue make_true() { return JsonValue(json_true()); }
inline JsonValue make_false() { return JsonValue(json_false()); }
inline JsonValue make_null() { return JsonValue(json_null()); }

} // namespace jansson_cpp

#endif // JANSSON_CPP_HPP
