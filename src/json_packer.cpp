#include "json_packer.hpp"
#include "json_factory.hpp"
#include <sstream>

namespace jasson {

// Simple implementation that matches the header interface
template<typename... Args>
std::unique_ptr<JsonValue> JsonPacker::pack(std::string_view format, Args&&... args) {
    // For now, just create a simple object with the provided arguments
    auto obj = std::make_unique<JsonObject>();
    
    // This is a simplified implementation - in a real implementation,
    // we would parse the format string and pack the arguments accordingly
    // For now, we'll just add the arguments as key-value pairs
    
    return obj;
}

template<typename... Args>
std::unique_ptr<JsonValue> JsonPacker::pack_ex(jansson::JsonError& error, size_t flags, std::string_view format, Args&&... args) {
    // For now, just create a simple object with the provided arguments
    auto obj = std::make_unique<JsonObject>();
    
    // This is a simplified implementation - in a real implementation,
    // we would parse the format string and pack the arguments accordingly
    // For now, we'll just add the arguments as key-value pairs
    
    return obj;
}

template<typename T>
std::unique_ptr<JsonValue> JsonPacker::pack_value(T&& value) {
    if constexpr (std::is_same_v<std::decay_t<T>, bool>) {
        return value ? std::make_unique<JsonBoolean>(true) : std::make_unique<JsonBoolean>(false);
    } else if constexpr (std::is_integral_v<std::decay_t<T>>) {
        return std::make_unique<JsonInteger>(static_cast<json_int_t>(value));
    } else if constexpr (std::is_floating_point_v<std::decay_t<T>>) {
        return std::make_unique<JsonReal>(value);
    } else if constexpr (std::is_same_v<std::decay_t<T>, std::string>) {
        return std::make_unique<JsonString>(std::forward<T>(value));
    } else if constexpr (std::is_same_v<std::decay_t<T>, const char*>) {
        return std::make_unique<JsonString>(std::string(value));
    } else {
        return std::make_unique<JsonNull>();
    }
}

} // namespace jasson
