#ifndef JASSON_JSON_TYPES_HPP
#define JASSON_JSON_TYPES_HPP

#include "json_value.hpp"
#include "json_config.hpp"
#include <memory>

namespace jasson {

// Forward declarations
class JsonObject;
class JsonArray;
class JsonString;
class JsonNumber;
class JsonBoolean;
class JsonNull;

using JsonValuePtr = std::shared_ptr<JsonValue>;
using JsonObjectPtr = std::shared_ptr<JsonObject>;
using JsonArrayPtr = std::shared_ptr<JsonArray>;
using JsonStringPtr = std::shared_ptr<JsonString>;
using JsonNumberPtr = std::shared_ptr<JsonNumber>;
using JsonIntegerPtr = std::shared_ptr<JsonInteger>;
using JsonRealPtr = std::shared_ptr<JsonReal>;
using JsonBooleanPtr = std::shared_ptr<JsonBoolean>;
using JsonNullPtr = std::shared_ptr<JsonNull>;

// Casting functions
template<typename T>
std::shared_ptr<T> json_cast(const JsonValuePtr& value) {
    return std::dynamic_pointer_cast<T>(value);
}

template<typename T>
bool json_isa(const JsonValuePtr& value) {
    return json_cast<T>(value) != nullptr;
}

} // namespace jasson

#endif // JASSON_JSON_TYPES_HPP
