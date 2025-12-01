#ifndef JASSON_HPP
#define JASSON_HPP

#include "json_value.hpp"
#include "json_types.hpp"
#include "json_factory.hpp"
#include "json_extensions.hpp"
#include "json_convenience.hpp"

namespace jasson {

// Convenience functions for type checking and casting
template<typename T>
bool json_is(const JsonValuePtr& value) {
    return std::dynamic_pointer_cast<T>(value) != nullptr;
}

// Type checking functions
inline bool json_is_object(const JsonValuePtr& value) {
    return value && value->is_object();
}

inline bool json_is_array(const JsonValuePtr& value) {
    return value && value->is_array();
}

inline bool json_is_string(const JsonValuePtr& value) {
    return value && value->is_string();
}

inline bool json_is_integer(const JsonValuePtr& value) {
    return value && value->is_integer();
}

inline bool json_is_real(const JsonValuePtr& value) {
    return value && value->is_real();
}

inline bool json_is_number(const JsonValuePtr& value) {
    return value && value->is_number();
}

inline bool json_is_true(const JsonValuePtr& value) {
    return value && value->is_boolean() && std::static_pointer_cast<JsonBoolean>(value)->value();
}

inline bool json_is_false(const JsonValuePtr& value) {
    return value && value->is_boolean() && !std::static_pointer_cast<JsonBoolean>(value)->value();
}

inline bool json_is_boolean(const JsonValuePtr& value) {
    return value && value->is_boolean();
}

inline bool json_is_null(const JsonValuePtr& value) {
    return value && value->is_null();
}

// Equality functions
inline bool json_equal(const JsonValuePtr& value1, const JsonValuePtr& value2) {
    if (!value1 || !value2) return false;
    return value1->equals(*value2);
}

// Copy functions
inline JsonValuePtr json_copy(const JsonValuePtr& value) {
    return value ? value->clone() : nullptr;
}

inline JsonValuePtr json_deep_copy(const JsonValuePtr& value) {
    return value ? value->deep_clone() : nullptr;
}

} // namespace jasson

#endif // JASSON_HPP
