#include <cassert>
#include <cerrno>
#include <cmath>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <string>
#include <locale>
#include <algorithm>

namespace jansson {
namespace strconv {

namespace {
    char get_decimal_point() {
        std::ostringstream oss;
        oss.imbue(std::locale());
        oss << std::fixed << std::setprecision(1) << 1.0;
        std::string result = oss.str();
        return result[1]; // Decimal point is at index 1
    }

    void to_locale(std::string& str) {
        char point = get_decimal_point();
        if (point == '.') {
            return;
        }

        size_t pos = str.find('.');
        if (pos != std::string::npos) {
            str[pos] = point;
        }
    }

    void from_locale(std::string& str) {
        char point = get_decimal_point();
        if (point == '.') {
            return;
        }

        size_t pos = str.find(point);
        if (pos != std::string::npos) {
            str[pos] = '.';
        }
    }
}

int jsonp_strtod(const std::string& str, double& out) {
    std::string temp = str;
    to_locale(temp);

    errno = 0;
    char* end = nullptr;
    double value = std::strtod(temp.c_str(), &end);
    
    if (end != temp.c_str() + temp.length()) {
        return -1;
    }

    if ((value == HUGE_VAL || value == -HUGE_VAL) && errno == ERANGE) {
        return -1;
    }

    out = value;
    return 0;
}

#if DTOA_ENABLED
extern "C" {
    char *dtoa_r(double dd, int mode, int ndigits, int *decpt, int *sign, char **rve,
                 char *buf, size_t blen);
}

int jsonp_dtostr(char* buffer, size_t size, double value, int precision) {
    char digits[25];
    char *digits_end;
    int mode = precision == 0 ? 0 : 2;
    int decpt, sign, exp_len, exp = 0, use_exp = 0;
    int digits_len, vdigits_start, vdigits_end;
    char *p;

    if (dtoa_r(value, mode, precision, &decpt, &sign, &digits_end, digits, 25) == nullptr) {
        return -1;
    }

    digits_len = digits_end - digits;
    if (decpt <= -4 || decpt > 16) {
        use_exp = 1;
        exp = decpt - 1;
        decpt = 1;
    }

    vdigits_start = decpt <= 0 ? decpt - 1 : 0;
    vdigits_end = digits_len;
    if (!use_exp) {
        vdigits_end = std::max(vdigits_end, decpt + 1);
    } else {
        vdigits_end = std::max(vdigits_end, decpt);
    }

    if (3 + (vdigits_end - vdigits_start) + (use_exp ? 5 : 0) > size) {
        return -1;
    }

    p = buffer;
    if (sign == 1) {
        *p++ = '-';
    }

    if (decpt <= 0) {
        std::fill(p, p + (decpt - vdigits_start), '0');
        p += decpt - vdigits_start;
        *p++ = '.';
        std::fill(p, p + (0 - decpt), '0');
        p += 0 - decpt;
    } else {
        std::fill(p, p + (0 - vdigits_start), '0');
        p += 0 - vdigits_start;
    }

    if (0 < decpt && decpt <= digits_len) {
        std::copy(digits, digits + decpt, p);
        p += decpt;
        *p++ = '.';
        std::copy(digits + decpt, digits + digits_len, p);
        p += digits_len - decpt;
    } else {
        std::copy(digits, digits + digits_len, p);
        p += digits_len;
    }

    if (digits_len < decpt) {
        std::fill(p, p + (decpt - digits_len), '0');
        p += decpt - digits_len;
        *p++ = '.';
        std::fill(p, p + (vdigits_end - decpt), '0');
        p += vdigits_end - decpt;
    } else {
        std::fill(p, p + (vdigits_end - digits_len), '0');
        p += vdigits_end - digits_len;
    }

    if (p[-1] == '.')
        p--;

    if (use_exp) {
        *p++ = 'e';
        int exp_len = std::sprintf(p, "%d", exp);
        p += exp_len;
    }
    *p = '\0';

    return static_cast<int>(p - buffer);
}
#else
int jsonp_dtostr(char* buffer, size_t size, double value, int precision) {
    if (precision == 0)
        precision = 17;

    std::ostringstream oss;
    oss.imbue(std::locale());
    oss << std::setprecision(precision) << value;
    std::string result = oss.str();
    
    from_locale(result);

    if (result.find('.') == std::string::npos && result.find('e') == std::string::npos) {
        result += ".0";
    }

    size_t start = result.find('e');
    if (start != std::string::npos) {
        start++;
        if (result[start] == '-') {
            start++;
        }
        
        size_t end = start + 1;
        while (end < result.length() && result[end] == '0') {
            end++;
        }
        
        if (end != start) {
            result.erase(start, end - start);
        }
    }

    if (result.length() >= size) {
        return -1;
    }

    std::copy(result.begin(), result.end(), buffer);
    buffer[result.length()] = '\0';
    
    return static_cast<int>(result.length());
}
#endif

} // namespace strconv
} // namespace jansson
