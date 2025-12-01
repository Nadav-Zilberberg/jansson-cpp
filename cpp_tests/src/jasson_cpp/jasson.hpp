#ifndef JASSON_HPP
#define JASSON_HPP

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <iostream>
#include <sstream>

namespace jasson {

// Forward declarations
class JsonValue;
class JsonObject;
class JsonArray;
class JsonString;
class JsonNumber;
class JsonBoolean;
class JsonNull;

// Smart pointer types
using json_ptr = std::shared_ptr<JsonValue>;
using json_object_ptr = std::shared_ptr<JsonObject>;
using json_array_ptr = std::shared_ptr<JsonArray>;
using json_string_ptr = std::shared_ptr<JsonString>;
using json_number_ptr = std::shared_ptr<JsonNumber>;
using json_boolean_ptr = std::shared_ptr<JsonBoolean>;
using json_null_ptr = std::shared_ptr<JsonNull>;

// Base JSON value class
class JsonValue {
public:
    virtual ~JsonValue() = default;
    virtual std::string to_string() const = 0;
    virtual std::string type_name() const = 0;
    
    // Type checking
    virtual bool is_object() const { return false; }
    virtual bool is_array() const { return false; }
    virtual bool is_string() const { return false; }
    virtual bool is_number() const { return false; }
    virtual bool is_boolean() const { return false; }
    virtual bool is_null() const { return false; }
    
    // Value access (for primitive types)
    virtual std::string value() const { 
        throw std::runtime_error("value() not implemented for this type"); 
    }
};

// JSON object class
class JsonObject : public JsonValue {
private:
    std::map<std::string, json_ptr> members;
    
public:
    JsonObject() = default;
    
    std::string to_string() const override {
        std::ostringstream oss;
        oss << "{";
        bool first = true;
        for (const auto& [key, value] : members) {
            if (!first) oss << ",";
            oss << "\"" << key << "\":" << value->to_string();
            first = false;
        }
        oss << "}";
        return oss.str();
    }
    
    std::string type_name() const override { return "object"; }
    bool is_object() const override { return true; }
    
    void add(const std::string& key, json_ptr value) {
        members[key] = value;
    }
    
    json_ptr get(const std::string& key) const {
        auto it = members.find(key);
        return (it != members.end()) ? it->second : nullptr;
    }
    
    size_t size() const { return members.size(); }
};

// JSON array class
class JsonArray : public JsonValue {
private:
    std::vector<json_ptr> elements;
    
public:
    JsonArray() = default;
    
    std::string to_string() const override {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < elements.size(); ++i) {
            if (i > 0) oss << ",";
            oss << elements[i]->to_string();
        }
        oss << "]";
        return oss.str();
    }
    
    std::string type_name() const override { return "array"; }
    bool is_array() const override { return true; }
    
    void append(json_ptr value) {
        elements.push_back(value);
    }
    
    size_t size() const { return elements.size(); }
};

// JSON string class
class JsonString : public JsonValue {
private:
    std::string str_value;
    
public:
    explicit JsonString(const std::string& value) : str_value(value) {}
    
    std::string to_string() const override {
        return "\"" + str_value + "\"";
    }
    
    std::string type_name() const override { return "string"; }
    bool is_string() const override { return true; }
    
    std::string value() const override { return str_value; }
};

// JSON number class
class JsonNumber : public JsonValue {
private:
    double num_value;
    
public:
    explicit JsonNumber(double value) : num_value(value) {}
    
    std::string to_string() const override {
        std::ostringstream oss;
        oss << num_value;
        return oss.str();
    }
    
    std::string type_name() const override { return "number"; }
    bool is_number() const override { return true; }
    
    std::string value() const override {
        std::ostringstream oss;
        oss << num_value;
        return oss.str();
    }
};

// JSON boolean class
class JsonBoolean : public JsonValue {
private:
    bool bool_value;
    
public:
    explicit JsonBoolean(bool value) : bool_value(value) {}
    
    std::string to_string() const override {
        return bool_value ? "true" : "false";
    }
    
    std::string type_name() const override { return "boolean"; }
    bool is_boolean() const override { return true; }
    
    std::string value() const override {
        return bool_value ? "true" : "false";
    }
};

// JSON null class
class JsonNull : public JsonValue {
public:
    JsonNull() = default;
    
    std::string to_string() const override {
        return "null";
    }
    
    std::string type_name() const override { return "null"; }
    bool is_null() const override { return true; }
};

// Factory functions
inline json_ptr jasson_string(const std::string& value) {
    return std::make_shared<JsonString>(value);
}

inline json_ptr jasson_integer(int64_t value) {
    return std::make_shared<JsonNumber>(static_cast<double>(value));
}

inline json_ptr jasson_real(double value) {
    return std::make_shared<JsonNumber>(value);
}

inline json_ptr jasson_true() {
    return std::make_shared<JsonBoolean>(true);
}

inline json_ptr jasson_false() {
    return std::make_shared<JsonBoolean>(false);
}

inline json_ptr jasson_null() {
    return std::make_shared<JsonNull>();
}

inline json_object_ptr jasson_object() {
    return std::make_shared<JsonObject>();
}

inline json_array_ptr jasson_array() {
    return std::make_shared<JsonArray>();
}

// Builder class for fluent JSON construction
class json_builder {
private:
    json_ptr current_value;
    
public:
    json_builder() = default;
    
    json_builder& object() {
        current_value = jasson_object();
        return *this;
    }
    
    json_builder& array() {
        current_value = jasson_array();
        return *this;
    }
    
    json_builder& add(const std::string& key, json_ptr value) {
        if (current_value && current_value->is_object()) {
            std::static_pointer_cast<JsonObject>(current_value)->add(key, value);
        }
        return *this;
    }
    
    // String overloads - must come before bool to avoid implicit conversion issues
    json_builder& add(const std::string& key, const char* value) {
        return add(key, jasson_string(std::string(value)));
    }
    
    json_builder& add(const std::string& key, const std::string& value) {
        return add(key, jasson_string(value));
    }
    
    // Explicit int64_t overload to avoid ambiguity
    json_builder& add(const std::string& key, int64_t value) {
        return add(key, jasson_integer(value));
    }
    
    json_builder& add(const std::string& key, int value) {
        return add(key, jasson_integer(static_cast<int64_t>(value)));
    }
    
    json_builder& add(const std::string& key, long long value) {
        return add(key, jasson_integer(static_cast<int64_t>(value)));
    }
    
    json_builder& add(const std::string& key, double value) {
        return add(key, jasson_real(value));
    }
    
    json_builder& add(const std::string& key, bool value) {
        return add(key, value ? jasson_true() : jasson_false());
    }
    
    json_builder& append(json_ptr value) {
        if (current_value && current_value->is_array()) {
            std::static_pointer_cast<JsonArray>(current_value)->append(value);
        }
        return *this;
    }
    
    // String overloads - must come before bool to avoid implicit conversion issues
    json_builder& append(const char* value) {
        return append(jasson_string(std::string(value)));
    }
    
    json_builder& append(const std::string& value) {
        return append(jasson_string(value));
    }
    
    // Explicit int64_t overload to avoid ambiguity
    json_builder& append(int64_t value) {
        return append(jasson_integer(value));
    }
    
    json_builder& append(int value) {
        return append(jasson_integer(static_cast<int64_t>(value)));
    }
    
    json_builder& append(long long value) {
        return append(jasson_integer(static_cast<int64_t>(value)));
    }
    
    json_builder& append(double value) {
        return append(jasson_real(value));
    }
    
    json_builder& append(bool value) {
        return append(value ? jasson_true() : jasson_false());
    }
    
    template<typename Container>
    json_builder& from_container(const Container& container) {
        current_value = jasson_array();
        auto arr = std::static_pointer_cast<JsonArray>(current_value);
        for (const auto& item : container) {
            arr->append(jasson_string(item));
        }
        return *this;
    }
    
    json_ptr build() {
        return current_value;
    }
};

// Literal operators for JSON values
inline json_ptr operator""_json(const char* str, size_t len) {
    return std::make_shared<JsonString>(std::string(str, len));
}

inline json_ptr operator""_json(unsigned long long value) {
    return std::make_shared<JsonNumber>(static_cast<double>(value));
}

inline json_ptr operator""_json(long double value) {
    return std::make_shared<JsonNumber>(static_cast<double>(value));
}

} // namespace jasson

#endif // JASSON_HPP
