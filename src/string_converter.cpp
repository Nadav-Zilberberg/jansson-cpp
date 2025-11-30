#include "string_converter.hpp"
#include <cerrno>
#include <cstdlib>
#include <algorithm>
#include <cctype>

namespace jansson {

std::locale StringConverter::get_classic_locale() {
    return std::locale::classic();
}

bool StringConverter::is_valid_number_format(const std::string& str) {
    if (str.empty()) return false;
    
    size_t start = 0;
    if (str[0] == '+' || str[0] == '-') start = 1;
    
    if (start >= str.length()) return false;
    
    bool has_digit = false;
    bool has_dot = false;
    bool has_exp = false;
    
    for (size_t i = start; i < str.length(); ++i) {
        char c = str[i];
        
        if (std::isdigit(c)) {
            has_digit = true;
        } else if (c == '.' && !has_dot && !has_exp) {
            has_dot = true;
        } else if ((c == 'e' || c == 'E') && !has_exp && has_digit) {
            has_exp = true;
            has_digit = false; // Need digits after exponent
            if (i + 1 < str.length() && (str[i + 1] == '+' || str[i + 1] == '-')) {
                i++; // Skip exponent sign
            }
        } else {
            return false;
        }
    }
    
    return has_digit;
}

std::string StringConverter::normalize_exponent(const std::string& str) {
    size_t exp_pos = str.find_first_of("eE");
    if (exp_pos == std::string::npos) {
        return str;
    }
    
    std::string result = str;
    size_t exp_start = exp_pos + 1;
    
    // Handle exponent sign
    if (exp_start < result.length() && (result[exp_start] == '+' || result[exp_start] == '-')) {
        exp_start++;
    }
    
    // Find first non-zero digit in exponent
    size_t first_non_zero = exp_start;
    while (first_non_zero < result.length() && result[first_non_zero] == '0') {
        first_non_zero++;
    }
    
    // If all zeros, keep one zero
    if (first_non_zero >= result.length()) {
        first_non_zero = exp_start;
    }
    
    // Remove leading zeros from exponent
    if (first_non_zero > exp_start) {
        result.erase(exp_start, first_non_zero - exp_start);
    }
    
    return result;
}

std::string StringConverter::ensure_decimal_or_exponent(const std::string& str) {
    if (str.find('.') != std::string::npos || str.find_first_of("eE") != std::string::npos) {
        return str;
    }
    
    // Add ".0" to distinguish from integer
    return str + ".0";
}

std::optional<double> StringConverter::string_to_double(const std::string& str) {
    if (!is_valid_number_format(str)) {
        return std::nullopt;
    }
    
    try {
        // Use classic locale for consistent parsing
        std::istringstream iss(str);
        iss.imbue(get_classic_locale());
        
        double value;
        iss >> value;
        
        // Check for parsing errors
        if (iss.fail() || !iss.eof()) {
            return std::nullopt;
        }
        
        // Check for overflow/underflow
        if (value == std::numeric_limits<double>::infinity() || 
            value == -std::numeric_limits<double>::infinity()) {
            return std::nullopt;
        }
        
        return value;
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

std::string StringConverter::double_to_string(double value, int precision) {
    if (precision == 0) {
        precision = 17; // Default precision for maximum accuracy
    }
    
    std::ostringstream oss;
    oss.imbue(get_classic_locale());
    
    // Set precision and formatting
    oss << std::setprecision(precision);
    
    // Use fixed notation for small values, scientific for large/small values
    if (value != 0.0 && (std::abs(value) < 1e-4 || std::abs(value) >= 1e16)) {
        oss << std::scientific;
    }
    
    oss << value;
    
    std::string result = oss.str();
    
    // Normalize the result
    result = normalize_exponent(result);
    result = ensure_decimal_or_exponent(result);
    
    return result;
}

std::optional<long long> StringConverter::string_to_integer(const std::string& str) {
    if (str.empty()) {
        return std::nullopt;
    }
    
    try {
        size_t pos;
        long long value = std::stoll(str, &pos);
        
        // Check if entire string was consumed
        if (pos != str.length()) {
            return std::nullopt;
        }
        
        return value;
    } catch (const std::invalid_argument&) {
        return std::nullopt;
    } catch (const std::out_of_range&) {
        return std::nullopt;
    }
}

std::string StringConverter::integer_to_string(long long value) {
    return std::to_string(value);
}

std::optional<unsigned long long> StringConverter::string_to_unsigned_integer(const std::string& str) {
    if (str.empty()) {
        return std::nullopt;
    }
    
    // Check for negative sign - not allowed for unsigned
    if (str[0] == '-') {
        return std::nullopt;
    }
    
    try {
        size_t pos;
        unsigned long long value = std::stoull(str, &pos);
        
        // Check if entire string was consumed
        if (pos != str.length()) {
            return std::nullopt;
        }
        
        return value;
    } catch (const std::invalid_argument&) {
        return std::nullopt;
    } catch (const std::out_of_range&) {
        return std::nullopt;
    }
}

std::string StringConverter::unsigned_integer_to_string(unsigned long long value) {
    return std::to_string(value);
}

// Convenience functions
std::optional<double> json_strtod(const std::string& str) {
    return StringConverter::string_to_double(str);
}

std::string json_dtostr(double value, int precision) {
    return StringConverter::double_to_string(value, precision);
}

} // namespace jansson
