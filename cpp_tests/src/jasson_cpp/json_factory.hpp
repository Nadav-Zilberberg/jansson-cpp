#ifndef JSON_FACTORY_HPP
#define JSON_FACTORY_HPP

#include "jasson.hpp"
#include <string>
#include <memory>

namespace jasson {

// Factory functions for creating JSON values

// String factory
inline json_ptr string(const std::string& value) {
    return std::make_shared<JsonString>(value);
}

// Number factories
inline json_ptr integer(int64_t value) {
    return std::make_shared<JsonNumber>(static_cast<double>(value));
}

inline json_ptr real(double value) {
    return std::make_shared<JsonNumber>(value);
}

// Boolean factories
inline json_ptr boolean(bool value) {
    return std::make_shared<JsonBoolean>(value);
}

inline json_ptr boolean_true() {
    return std::make_shared<JsonBoolean>(true);
}

inline json_ptr boolean_false() {
    return std::make_shared<JsonBoolean>(false);
}

// Null factory
inline json_ptr null() {
    return std::make_shared<JsonNull>();
}

// Object factory
inline json_object_ptr object() {
    return std::make_shared<JsonObject>();
}

// Array factory
inline json_array_ptr array() {
    return std::make_shared<JsonArray>();
}

} // namespace jasson

#endif // JSON_FACTORY_HPP
