#ifndef JANSSON_STRCONV_WRAPPER_HPP
#define JANSSON_STRCONV_WRAPPER_HPP

#include "string_converter.hpp"
#include <cstring>

// C++ wrapper providing compatibility with original C interface
// These functions can be used as drop-in replacements for jsonp_strtod and jsonp_dtostr

namespace jansson {
namespace compat {

// Wrapper for jsonp_strtod functionality
// Converts string buffer to double, returns true on success, false on failure
inline bool jsonp_strtod_wrapper(const char* str, double* out) {
    if (!str || !out) return false;
    
    auto result = StringConverter::string_to_double(std::string(str));
    if (result.has_value()) {
        *out = result.value();
        return true;
    }
    return false;
}

// Wrapper for jsonp_dtostr functionality
// Converts double to string with specified precision
// Returns the number of characters written, or -1 on error
inline int jsonp_dtostr_wrapper(char* buffer, size_t size, double value, int precision) {
    if (!buffer || size == 0) return -1;
    
    std::string result = StringConverter::double_to_string(value, precision);
    
    // Check if result fits in buffer
    if (result.length() >= size) {
        return -1; // Buffer too small
    }
    
    // Copy result to buffer
    std::strcpy(buffer, result.c_str());
    return static_cast<int>(result.length());
}

// Convenience function using std::string
inline std::string jsonp_dtostr_string(double value, int precision = 0) {
    return StringConverter::double_to_string(value, precision);
}

} // namespace compat
} // namespace jansson

#endif // JANSSON_STRCONV_WRAPPER_HPP
