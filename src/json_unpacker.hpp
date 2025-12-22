#ifndef JANSSON_JSON_UNPACKER_HPP
#define JANSSON_JSON_UNPACKER_HPP

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

class JsonUnpacker {
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
    static bool unpack(const Value& root, std::string_view format, Args&&... args);

    template<typename... Args>
    static bool unpack_ex(const Value& root, jansson::JsonError& error, size_t flags,
                         std::string_view format, Args&&... args);

private:
    template<typename T>
    static bool unpack_value(const Value& json, T& target);

    template<typename T>
    static bool unpack_array(const Value& json, std::vector<T>& target);

    template<typename... Args>
    static bool unpack_object(const Value& json, Args&&... args);
};

template<typename... Args>
bool unpack(const JsonValue& root, std::string_view format, Args&&... args) {
    return JsonUnpacker::unpack(root, format, std::forward<Args>(args)...);
}

} // namespace jasson

#endif // JANSSON_JSON_UNPACKER_HPP
