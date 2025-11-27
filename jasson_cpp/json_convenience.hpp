#ifndef JASSON_JSON_CONVENIENCE_HPP
#define JASSON_JSON_CONVENIENCE_HPP

#include "json_value.hpp"
#include "json_types.hpp"
#include "json_extensions.hpp"
#include <initializer_list>
#include <utility>
#include <stdexcept>

namespace jasson {

// Convenience functions for creating JSON values - return unique_ptr to work with existing API
inline std::unique_ptr<JsonObject> jasson_object() {
    return std::make_unique<JsonObject>();
}

inline std::unique_ptr<JsonArray> jasson_array() {
    return std::make_unique<JsonArray>();
}

inline std::unique_ptr<JsonString> jasson_string(const std::string& value) {
    return std::make_unique<JsonString>(value);
}

inline std::unique_ptr<JsonString> jasson_string(const char* value) {
    return std::make_unique<JsonString>(std::string(value));
}

inline std::unique_ptr<JsonInteger> jasson_integer(json_int_t value) {
    return std::make_unique<JsonInteger>(value);
}

inline std::unique_ptr<JsonReal> jasson_real(double value) {
    return std::make_unique<JsonReal>(value);
}

inline std::unique_ptr<JsonBoolean> jasson_true() {
    return std::make_unique<JsonBoolean>(true);
}

inline std::unique_ptr<JsonBoolean> jasson_false() {
    return std::make_unique<JsonBoolean>(false);
}

inline std::unique_ptr<JsonNull> jasson_null() {
    return std::make_unique<JsonNull>();
}

// Also provide shared_ptr versions for the convenience API
inline JsonObjectPtr json_object() {
    return std::make_shared<JsonObject>();
}

inline JsonArrayPtr json_array() {
    return std::make_shared<JsonArray>();
}

inline JsonStringPtr json_string(const std::string& value) {
    return std::make_shared<JsonString>(value);
}

inline JsonStringPtr json_string(const char* value) {
    return std::make_shared<JsonString>(std::string(value));
}

inline JsonIntegerPtr json_integer(json_int_t value) {
    return std::make_shared<JsonInteger>(value);
}

inline JsonRealPtr json_real(double value) {
    return std::make_shared<JsonReal>(value);
}

inline JsonBooleanPtr json_true() {
    return std::make_shared<JsonBoolean>(true);
}

inline JsonBooleanPtr json_false() {
    return std::make_shared<JsonBoolean>(false);
}

inline JsonNullPtr json_null() {
    return std::make_shared<JsonNull>();
}

// Fluent API for JSON construction
class JsonBuilder {
public:
    JsonBuilder() = default;
    
    // Object construction
    JsonBuilder& object() {
        current_ = json_object();
        return *this;
    }
    
    JsonBuilder& array() {
        current_ = json_array();
        return *this;
    }
    
    // Add key-value pair to object
    JsonBuilder& add(const std::string& key, JsonValuePtr value) {
        if (!current_ || !current_->is_object()) {
            throw std::runtime_error("Current value is not an object");
        }
        // Convert shared_ptr to unique_ptr for the set method
        std::static_pointer_cast<JsonObject>(current_)->set(key, value->clone());
        return *this;
    }
    
    // Add key-value pair with convenience constructors
    JsonBuilder& add(const std::string& key, const std::string& value) {
        return add(key, json_string(value));
    }
    
    JsonBuilder& add(const std::string& key, const char* value) {
        return add(key, json_string(value));
    }
    
    JsonBuilder& add(const std::string& key, json_int_t value) {
        return add(key, json_integer(value));
    }
    
    JsonBuilder& add(const std::string& key, double value) {
        return add(key, json_real(value));
    }
    
    JsonBuilder& add(const std::string& key, bool value) {
        return add(key, value ? json_true() : json_false());
    }
    
    // Append to array
    JsonBuilder& append(JsonValuePtr value) {
        if (!current_ || !current_->is_array()) {
            throw std::runtime_error("Current value is not an array");
        }
        // Convert shared_ptr to unique_ptr for the append method
        std::static_pointer_cast<JsonArray>(current_)->append(value->clone());
        return *this;
    }
    
    // Append with convenience constructors
    JsonBuilder& append(const std::string& value) {
        return append(json_string(value));
    }
    
    JsonBuilder& append(const char* value) {
        return append(json_string(value));
    }
    
    JsonBuilder& append(json_int_t value) {
        return append(json_integer(value));
    }
    
    JsonBuilder& append(double value) {
        return append(json_real(value));
    }
    
    JsonBuilder& append(bool value) {
        return append(value ? json_true() : json_false());
    }
    
    // Range-based constructors
    template<typename Container>
    JsonBuilder& from_container(const Container& container) {
        array();
        for (const auto& item : container) {
            append(item);
        }
        return *this;
    }
    
    // Initializer list support for objects
    JsonBuilder& from_pairs(std::initializer_list<std::pair<std::string, JsonValuePtr>> pairs) {
        object();
        for (const auto& [key, value] : pairs) {
            add(key, value);
        }
        return *this;
    }
    
    // Initializer list support for arrays
    JsonBuilder& from_values(std::initializer_list<JsonValuePtr> values) {
        array();
        for (const auto& value : values) {
            append(value);
        }
        return *this;
    }
    
    // Get the result
    JsonValuePtr build() {
        return current_;
    }
    
    // Implicit conversion
    operator JsonValuePtr() {
        return current_;
    }
    
private:
    JsonValuePtr current_;
};

// Convenience builder function
inline JsonBuilder json_builder() {
    return JsonBuilder();
}

// Operator overloading for intuitive syntax
inline JsonStringPtr operator""_json(const char* str, size_t len) {
    return json_string(std::string(str, len));
}

inline JsonIntegerPtr operator""_json(unsigned long long value) {
    return json_integer(static_cast<json_int_t>(value));
}

inline JsonRealPtr operator""_json(long double value) {
    return json_real(static_cast<double>(value));
}

// Modern iterator support
class JsonIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = JsonValuePtr;
    using difference_type = std::ptrdiff_t;
    using pointer = JsonValuePtr*;
    using reference = JsonValuePtr&;
    
    JsonIterator() = default;
    
    // Array iterator
    explicit JsonIterator(const JsonArrayPtr& array) 
        : array_(array), is_array_(true), array_index_(0) {}
    
    // Object iterator
    explicit JsonIterator(const JsonObjectPtr& object)
        : object_(object), is_array_(false), object_it_(object->begin()) {}
    
    reference operator*() {
        if (is_array_) {
            current_value_ = array_->at(array_index_)->clone();
        } else {
            current_value_ = object_it_->second->clone();
        }
        return current_value_;
    }
    
    pointer operator->() {
        if (is_array_) {
            current_value_ = array_->at(array_index_)->clone();
        } else {
            current_value_ = object_it_->second->clone();
        }
        return &current_value_;
    }
    
    JsonIterator& operator++() {
        if (is_array_) {
            ++array_index_;
        } else {
            ++object_it_;
        }
        return *this;
    }
    
    JsonIterator operator++(int) {
        JsonIterator tmp = *this;
        ++(*this);
        return tmp;
    }
    
    bool operator==(const JsonIterator& other) const {
        if (is_array_ != other.is_array_) return false;
        if (is_array_) {
            return array_index_ == other.array_index_;
        } else {
            return object_it_ == other.object_it_;
        }
    }
    
    bool operator!=(const JsonIterator& other) const {
        return !(*this == other);
    }
    
    // Get key for object iteration
    const std::string& key() const {
        if (is_array_) {
            throw std::runtime_error("Cannot get key from array iterator");
        }
        return object_it_->first;
    }
    
private:
    JsonArrayPtr array_;
    JsonObjectPtr object_;
    bool is_array_ = false;
    size_t array_index_ = 0;
    std::unordered_map<std::string, std::unique_ptr<JsonValue>>::const_iterator object_it_;
    mutable JsonValuePtr current_value_;
};

// Range-based iteration support
inline JsonIterator json_begin(const JsonValuePtr& value) {
    if (value->is_array()) {
        return JsonIterator(std::static_pointer_cast<JsonArray>(value));
    } else if (value->is_object()) {
        return JsonIterator(std::static_pointer_cast<JsonObject>(value));
    } else {
        throw std::runtime_error("Value is not iterable");
    }
}

inline JsonIterator json_end(const JsonValuePtr& value) {
    return JsonIterator(); // Default constructor creates end iterator
}

} // namespace jasson

#endif // JASSON_JSON_CONVENIENCE_HPP
