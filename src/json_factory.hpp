#ifndef JASSON_JSON_FACTORY_HPP
#define JASSON_JSON_FACTORY_HPP

#include "json_value.hpp"
#include <memory>

namespace jasson {

class JsonFactory {
public:
    static std::unique_ptr<JsonObject> create_object() {
        return std::make_unique<JsonObject>();
    }

    static std::unique_ptr<JsonArray> create_array() {
        return std::make_unique<JsonArray>();
    }

    static std::unique_ptr<JsonString> create_string(const std::string& value) {
        return std::make_unique<JsonString>(value);
    }

    static std::unique_ptr<JsonString> create_string(std::string&& value) {
        return std::make_unique<JsonString>(std::move(value));
    }

    static std::unique_ptr<JsonString> create_string(const char* value) {
        return std::make_unique<JsonString>(value);
    }

    static std::unique_ptr<JsonInteger> create_integer(json_int_t value) {
        return std::make_unique<JsonInteger>(value);
    }

    static std::unique_ptr<JsonReal> create_real(double value) {
        return std::make_unique<JsonReal>(value);
    }

    static std::unique_ptr<JsonBoolean> create_true() {
        return std::make_unique<JsonBoolean>(true);
    }

    static std::unique_ptr<JsonBoolean> create_false() {
        return std::make_unique<JsonBoolean>(false);
    }

    static std::unique_ptr<JsonBoolean> create_boolean(bool value) {
        return std::make_unique<JsonBoolean>(value);
    }

    static std::unique_ptr<JsonNull> create_null() {
        return std::make_unique<JsonNull>();
    }
};

} // namespace jasson

#endif // JASSON_JSON_FACTORY_HPP
