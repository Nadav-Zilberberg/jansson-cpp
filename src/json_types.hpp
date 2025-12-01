#ifndef JSON_TYPES_HPP
#define JSON_TYPES_HPP

#include <cstdint>
#include <string>

namespace jasson {

// JSON type enumeration
enum class JsonType {
    NULL_TYPE,
    BOOLEAN,
    NUMBER,
    STRING,
    ARRAY,
    OBJECT
};

// Type traits for JSON values
template<typename T>
struct JsonTypeTraits {
    static constexpr JsonType type = JsonType::NULL_TYPE;
};

template<>
struct JsonTypeTraits<bool> {
    static constexpr JsonType type = JsonType::BOOLEAN;
};

template<>
struct JsonTypeTraits<int> {
    static constexpr JsonType type = JsonType::NUMBER;
};

template<>
struct JsonTypeTraits<long> {
    static constexpr JsonType type = JsonType::NUMBER;
};

template<>
struct JsonTypeTraits<long long> {
    static constexpr JsonType type = JsonType::NUMBER;
};

template<>
struct JsonTypeTraits<float> {
    static constexpr JsonType type = JsonType::NUMBER;
};

template<>
struct JsonTypeTraits<double> {
    static constexpr JsonType type = JsonType::NUMBER;
};

template<>
struct JsonTypeTraits<std::string> {
    static constexpr JsonType type = JsonType::STRING;
};

template<>
struct JsonTypeTraits<const char*> {
    static constexpr JsonType type = JsonType::STRING;
};

} // namespace jasson

#endif // JSON_TYPES_HPP
