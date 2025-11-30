#ifndef JSON_CONVENIENCE_HPP
#define JSON_CONVENIENCE_HPP

#include "jasson.hpp"
#include "json_factory.hpp"
#include <string>
#include <vector>
#include <map>

namespace jasson {

// Convenience builder classes for fluent JSON construction

class ObjectBuilder {
private:
    json_object_ptr obj;
    
public:
    ObjectBuilder() : obj(object()) {}
    
    ObjectBuilder& add(const std::string& key, json_ptr value) {
        obj->add(key, value);
        return *this;
    }
    
    // String overloads - must come before bool to avoid implicit conversion issues
    ObjectBuilder& add(const std::string& key, const char* value) {
        obj->add(key, string(std::string(value)));
        return *this;
    }
    
    ObjectBuilder& add(const std::string& key, const std::string& value) {
        obj->add(key, string(value));
        return *this;
    }
    
    ObjectBuilder& add(const std::string& key, int value) {
        obj->add(key, integer(static_cast<int64_t>(value)));
        return *this;
    }
    
    ObjectBuilder& add(const std::string& key, long long value) {
        obj->add(key, integer(static_cast<int64_t>(value)));
        return *this;
    }
    
    ObjectBuilder& add(const std::string& key, double value) {
        obj->add(key, real(value));
        return *this;
    }
    
    ObjectBuilder& add(const std::string& key, bool value) {
        obj->add(key, boolean(value));
        return *this;
    }
    
    ObjectBuilder& add_null(const std::string& key) {
        obj->add(key, null());
        return *this;
    }
    
    json_object_ptr build() {
        return obj;
    }
    
    json_ptr as_value() {
        return obj;
    }
};

class ArrayBuilder {
private:
    json_array_ptr arr;
    
public:
    ArrayBuilder() : arr(array()) {}
    
    ArrayBuilder& append(json_ptr value) {
        arr->append(value);
        return *this;
    }
    
    // String overloads - must come before bool to avoid implicit conversion issues
    ArrayBuilder& append(const char* value) {
        arr->append(string(std::string(value)));
        return *this;
    }
    
    ArrayBuilder& append(const std::string& value) {
        arr->append(string(value));
        return *this;
    }
    
    ArrayBuilder& append(int value) {
        arr->append(integer(static_cast<int64_t>(value)));
        return *this;
    }
    
    ArrayBuilder& append(long long value) {
        arr->append(integer(static_cast<int64_t>(value)));
        return *this;
    }
    
    ArrayBuilder& append(double value) {
        arr->append(real(value));
        return *this;
    }
    
    ArrayBuilder& append(bool value) {
        arr->append(boolean(value));
        return *this;
    }
    
    ArrayBuilder& append_null() {
        arr->append(null());
        return *this;
    }
    
    json_array_ptr build() {
        return arr;
    }
    
    json_ptr as_value() {
        return arr;
    }
};

// Convenience functions
inline ObjectBuilder object_builder() {
    return ObjectBuilder();
}

inline ArrayBuilder array_builder() {
    return ArrayBuilder();
}

// Helper function to create a simple object with key-value pairs
template<typename... Args>
json_object_ptr make_object(Args... args) {
    auto obj = object();
    // This is a simplified version - in a real implementation,
    // we'd use variadic templates more effectively
    return obj;
}

// Helper function to create a simple array with values
template<typename... Args>
json_array_ptr make_array(Args... args) {
    auto arr = array();
    // This is a simplified version - in a real implementation,
    // we'd use variadic templates more effectively
    return arr;
}

} // namespace jasson

#endif // JSON_CONVENIENCE_HPP
