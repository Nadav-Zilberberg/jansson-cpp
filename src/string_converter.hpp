#ifndef JANSSON_STRING_CONVERTER_HPP
#define JANSSON_STRING_CONVERTER_HPP

#include <string>
#include <optional>
#include <stdexcept>
#include <locale>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <limits>

namespace jansson {

class StringConverter {
public:
    // Convert string to double with locale-independent parsing
    static std::optional<double> string_to_double(const std::string& str);
    
    // Convert double to string with locale-independent formatting
    static std::string double_to_string(double value, int precision = 0);
    
    // Convert string to integer
    static std::optional<long long> string_to_integer(const std::string& str);
    
    // Convert integer to string
    static std::string integer_to_string(long long value);
    
    // Convert string to unsigned integer
    static std::optional<unsigned long long> string_to_unsigned_integer(const std::string& str);
    
    // Convert unsigned integer to string
    static std::string unsigned_integer_to_string(unsigned long long value);

private:
    // Use classic locale for consistent parsing/formatting
    static std::locale get_classic_locale();
    
    // Check if string represents a valid number format
    static bool is_valid_number_format(const std::string& str);
    
    // Remove leading zeros from exponent part
    static std::string normalize_exponent(const std::string& str);
    
    // Ensure decimal point or exponent in double string representation
    static std::string ensure_decimal_or_exponent(const std::string& str);
};

// Convenience functions for direct usage
std::optional<double> json_strtod(const std::string& str);
std::string json_dtostr(double value, int precision = 0);

} // namespace jansson

#endif // JANSSON_STRING_CONVERTER_HPP
