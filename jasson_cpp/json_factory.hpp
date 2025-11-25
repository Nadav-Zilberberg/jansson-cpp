#ifndef JASSON_JSON_FACTORY_HPP
#define JASSON_JSON_FACTORY_HPP

#include "json_types.hpp"
#include <string>

namespace jasson {

// Factory functions for creating JSON values
inline JsonObjectPtr json_object() {
    return std::make_shared<JsonObject>();
}

inline JsonArrayPtr json_array() {
    return std::make_shared<JsonArray>();
}

inline JsonStringPtr json_string(const std::string& value) {
    return std::make_shared<JsonString>(value);
}

inline JsonNumberPtr json_integer(json_int_t value) {
    return std::make_shared<JsonNumber>(value);
}

inline JsonNumberPtr json_real(double value) {
    return std::make_shared<JsonNumber>(value);
}

inline JsonBooleanPtr json_true() {
    return std::make_shared<JsonBoolean>(true);
}

inline JsonBooleanPtr json_false() {
    return std::make_shared<JsonBoolean>(false);
}

inline JsonBooleanPtr json_boolean(bool value) {
    return std::make_shared<JsonBoolean>(value);
}

inline JsonNullPtr json_null() {
    return std::make_shared<JsonNull>();
}

} // namespace jasson

#endif // JASSON_JSON_FACTORY_HPP
