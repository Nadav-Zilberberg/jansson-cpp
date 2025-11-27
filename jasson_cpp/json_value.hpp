#ifndef JASSON_JSON_VALUE_HPP
#define JASSON_JSON_VALUE_HPP

#include "jansson.h"
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <variant>
#include <optional>

namespace jasson {

class JsonValue {
public:
    virtual ~JsonValue() = default;
    
    virtual bool is_object() const { return false; }
    virtual bool is_array() const { return false; }
    virtual bool is_string() const { return false; }
    virtual bool is_integer() const { return false; }
    virtual bool is_real() const { return false; }
    virtual bool is_number() const { return is_integer() || is_real(); }
    virtual bool is_boolean() const { return false; }
    virtual bool is_null() const { return false; }

    virtual size_t size() const { return 0; }
    virtual std::unique_ptr<JsonValue> clone() const = 0;
};

class JsonObject : public JsonValue {
public:
    using ValueType = std::unique_ptr<JsonValue>;
    using Container = std::unordered_map<std::string, ValueType>;
    using iterator = Container::iterator;
    using const_iterator = Container::const_iterator;

    bool is_object() const override { return true; }
    
    size_t size() const override { return values_.size(); }
    
    ValueType& get(const std::string& key) { return values_[key]; }
    const ValueType& get(const std::string& key) const {
        auto it = values_.find(key);
        static ValueType null_value;
        return it != values_.end() ? it->second : null_value;
    }
    
    void set(const std::string& key, ValueType value) {
        values_[key] = std::move(value);
    }
    
    iterator find(const std::string& key) { return values_.find(key); }
    const_iterator find(const std::string& key) const { return values_.find(key); }
    
    iterator begin() { return values_.begin(); }
    iterator end() { return values_.end(); }
    const_iterator begin() const { return values_.begin(); }
    const_iterator end() const { return values_.end(); }
    
    std::unique_ptr<JsonValue> clone() const override {
        auto result = std::make_unique<JsonObject>();
        for (const auto& [key, value] : values_) {
            result->set(key, value->clone());
        }
        return result;
    }

private:
    Container values_;
};

class JsonArray : public JsonValue {
public:
    using ValueType = std::unique_ptr<JsonValue>;
    using Container = std::vector<ValueType>;
    using iterator = Container::iterator;
    using const_iterator = Container::const_iterator;

    bool is_array() const override { return true; }
    
    size_t size() const override { return values_.size(); }
    
    ValueType& at(size_t index) { return values_.at(index); }
    const ValueType& at(size_t index) const { return values_.at(index); }
    
    void append(ValueType value) { values_.push_back(std::move(value)); }
    
    iterator begin() { return values_.begin(); }
    iterator end() { return values_.end(); }
    const_iterator begin() const { return values_.begin(); }
    const_iterator end() const { return values_.end(); }
    
    std::unique_ptr<JsonValue> clone() const override {
        auto result = std::make_unique<JsonArray>();
        for (const auto& value : values_) {
            result->append(value->clone());
        }
        return result;
    }

private:
    Container values_;
};

class JsonString : public JsonValue {
public:
    explicit JsonString(std::string value) : value_(std::move(value)) {}
    
    bool is_string() const override { return true; }
    
    const std::string& value() const { return value_; }
    const char* c_str() const { return value_.c_str(); }
    
    std::unique_ptr<JsonValue> clone() const override {
        return std::make_unique<JsonString>(value_);
    }

private:
    std::string value_;
};

class JsonInteger : public JsonValue {
public:
    explicit JsonInteger(json_int_t value) : value_(value) {}
    
    bool is_integer() const override { return true; }
    
    json_int_t value() const { return value_; }
    
    std::unique_ptr<JsonValue> clone() const override {
        return std::make_unique<JsonInteger>(value_);
    }

private:
    json_int_t value_;
};

class JsonReal : public JsonValue {
public:
    explicit JsonReal(double value) : value_(value) {}
    
    bool is_real() const override { return true; }
    
    double value() const { return value_; }
    
    std::unique_ptr<JsonValue> clone() const override {
        return std::make_unique<JsonReal>(value_);
    }

private:
    double value_;
};

class JsonBoolean : public JsonValue {
public:
    explicit JsonBoolean(bool value) : value_(value) {}
    
    bool is_boolean() const override { return true; }
    
    bool value() const { return value_; }
    
    std::unique_ptr<JsonValue> clone() const override {
        return std::make_unique<JsonBoolean>(value_);
    }

private:
    bool value_;
};

class JsonNull : public JsonValue {
public:
    bool is_null() const override { return true; }
    
    std::unique_ptr<JsonValue> clone() const override {
        return std::make_unique<JsonNull>();
    }
};

} // namespace jasson

#endif // JASSON_JSON_VALUE_HPP
