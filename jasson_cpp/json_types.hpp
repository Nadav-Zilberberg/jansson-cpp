#ifndef JASSON_JSON_TYPES_HPP
#define JASSON_JSON_TYPES_HPP

#include <cstdint>

namespace jasson {

// JSON type enumeration
enum class JsonType {
    NULL_VALUE,
    BOOLEAN,
    INTEGER,
    REAL,
    STRING,
    ARRAY,
    OBJECT
};

// JSON integer type (64-bit signed integer)
using json_int_t = int64_t;

// JSON real type (double precision floating point)
using json_real_t = double;

} // namespace jasson

#endif // JASSON_JSON_TYPES_HPP
