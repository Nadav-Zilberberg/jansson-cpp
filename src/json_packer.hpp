#ifndef JANSSON_JSON_PACKER_HPP
#define JANSSON_JSON_PACKER_HPP

#include <string>
#include <string_view>
#include <tuple>
#include <memory>
#include <variant>
#include <optional>
#include <vector>
#include "jansson.h"
#include "json_value.hpp"
#include "error.hpp"
#include "type_traits.hpp"

namespace jasson {

class JsonPacker {
public:
    using Value = JsonValue;
    using Object = JsonObject;
    using Array = JsonArray;
    using String = JsonString;
    using Integer = JsonInteger;
    using Real = JsonReal;
    using Boolean = JsonBoolean;
    using Null = JsonNull;

    template<typename... Args>
    static std::unique_ptr<Value> pack(std::string_view format, Args&&... args);

    template<typename... Args>
    static std::unique_ptr<Value> pack_ex(jansson::JsonError& error, size_t flags, std::string_view format, Args&&... args);

    template<typename T>
    static std::unique_ptr<Value> pack_value(T&& value);

private:
    template<typename T>
    static std::unique_ptr<Value> pack_array(T&& container);

    template<typename... Args>
    static std::unique_ptr<Value> pack_object(Args&&... args);

    template<typename T>
    static std::unique_ptr<Value> pack_optional(std::optional<T>&& opt);

    template<typename... Args>
    static std::unique_ptr<Value> pack_variant(std::variant<Args...>&& var);
};

template<typename... Args>
std::unique_ptr<JsonValue> pack(std::string_view format, Args&&... args) {
    return JsonPacker::pack(format, std::forward<Args>(args)...);
}

} // namespace jasson

#endif // JANSSON_JSON_PACKER_HPP
