#ifndef JSON_CONVENIENCE_HPP
#define JSON_CONVENIENCE_HPP

#include "json_value.hpp"
#include "json_factory.hpp"
#include "json_extensions.hpp"
#include "json_loader.hpp"
#include "json_dumper.hpp"

namespace jasson {

// Convenience functions for common operations

// Create a JSON value from any type
template<typename T>
JsonValuePtr to_json(const T& value);

// Specializations for common types
template<>
inline JsonValuePtr to_json<bool>(const bool& value) {
    return jasson_boolean(value);
}

template<>
inline JsonValuePtr to_json<int>(const int& value) {
    return jasson_integer(value);
}

template<>
inline JsonValuePtr to_json<long>(const long& value) {
    return jasson_integer(value);
}

template<>
inline JsonValuePtr to_json<long long>(const long long& value) {
    return jasson_integer(value);
}

template<>
inline JsonValuePtr to_json<float>(const float& value) {
    return jasson_real(value);
}

template<>
inline JsonValuePtr to_json<double>(const double& value) {
    return jasson_real(value);
}

template<>
inline JsonValuePtr to_json<std::string>(const std::string& value) {
    return jasson_string(value);
}

template<>
inline JsonValuePtr to_json<const char*>(const char* const& value) {
    return jasson_string(value);
}

// Convert JSON value to C++ type
template<typename T>
T from_json(const JsonValuePtr& value);

// Specializations for common types
template<>
inline bool from_json<bool>(const JsonValuePtr& value) {
    if (!value || !value->is_boolean()) {
        throw std::runtime_error("Value is not a boolean");
    }
    return value->as_boolean()->value();
}

template<>
inline int from_json<int>(const JsonValuePtr& value) {
    if (!value || !value->is_integer()) {
        throw std::runtime_error("Value is not an integer");
    }
    return static_cast<int>(value->as_integer()->value());
}

template<>
inline long from_json<long>(const JsonValuePtr& value) {
    if (!value || !value->is_integer()) {
        throw std::runtime_error("Value is not an integer");
    }
    return static_cast<long>(value->as_integer()->value());
}

template<>
inline long long from_json<long long>(const JsonValuePtr& value) {
    if (!value || !value->is_integer()) {
        throw std::runtime_error("Value is not an integer");
    }
    return value->as_integer()->value();
}

template<>
inline float from_json<float>(const JsonValuePtr& value) {
    if (!value || !value->is_number()) {
        throw std::runtime_error("Value is not a number");
    }
    if (value->is_integer()) {
        return static_cast<float>(value->as_integer()->value());
    }
    return static_cast<float>(value->as_real()->value());
}

template<>
inline double from_json<double>(const JsonValuePtr& value) {
    if (!value || !value->is_number()) {
        throw std::runtime_error("Value is not a number");
    }
    if (value->is_integer()) {
        return static_cast<double>(value->as_integer()->value());
    }
    return value->as_real()->value();
}

template<>
inline std::string from_json<std::string>(const JsonValuePtr& value) {
    if (!value || !value->is_string()) {
        throw std::runtime_error("Value is not a string");
    }
    return value->as_string()->value();
}

// Convenience builder functions
inline JsonBuilder object_builder() {
    return json_builder().object();
}

inline JsonBuilder array_builder() {
    return json_builder().array();
}

// Convenience functions for common operations
inline JsonValuePtr clone(const JsonValuePtr& value) {
    return value ? value->clone() : nullptr;
}

inline JsonValuePtr deep_clone(const JsonValuePtr& value) {
    return value ? value->deep_clone() : nullptr;
}

inline bool equals(const JsonValuePtr& a, const JsonValuePtr& b) {
    if (!a && !b) return true;
    if (!a || !b) return false;
    return a->equals(*b);
}

inline std::string to_string(const JsonValuePtr& value) {
    return value ? value->to_string() : "null";
}

inline std::string to_pretty_string(const JsonValuePtr& value) {
    return value ? value->to_pretty_string() : "null";
}

} // namespace jasson

#endif // JSON_CONVENIENCE_HPP
