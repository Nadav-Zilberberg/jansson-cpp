# StringConverter - Modern C++ String Conversion for Jansson

## Overview

The `StringConverter` class provides modern C++ string conversion functionality as a replacement for the original C-style string conversion functions in `strconv.c`. It implements locale-independent number parsing and formatting with safe error handling using `std::optional`.

## Key Features

### Modern C++ Practices Applied
- **RAII**: Automatic resource management with `std::string`
- **Smart Pointers**: Not needed for this use case (no dynamic allocation)
- **STL Containers**: Uses `std::string` and `std::optional`
- **Type Safety**: Strong typing prevents common C errors
- **Exception Safety**: Uses `std::optional` instead of exceptions for error handling
- **Locale Independence**: Uses classic locale for consistent parsing/formatting

### API Design

#### Core StringConverter Class
```cpp
class StringConverter {
public:
    // Convert string to double with locale-independent parsing
    static std::optional<double> string_to_double(const std::string& str);
    
    // Convert double to string with locale-independent formatting
    static std::string double_to_string(double value, int precision = 0);
    
    // Integer conversions
    static std::optional<long long> string_to_integer(const std::string& str);
    static std::string integer_to_string(long long value);
    
    // Unsigned integer conversions
    static std::optional<unsigned long long> string_to_unsigned_integer(const std::string& str);
    static std::string unsigned_integer_to_string(unsigned long long value);
};
```

#### Convenience Functions
```cpp
std::optional<double> json_strtod(const std::string& str);
std::string json_dtostr(double value, int precision = 0);
```

#### C Compatibility Wrapper
```cpp
namespace compat {
    bool jsonp_strtod_wrapper(const char* str, double* out);
    int jsonp_dtostr_wrapper(char* buffer, size_t size, double value, int precision);
    std::string jsonp_dtostr_string(double value, int precision = 0);
}
```

## Implementation Details

### String to Double Conversion
- Validates number format before conversion
- Uses `std::istringstream` with classic locale
- Handles scientific notation (e.g., `1.23e10`)
- Returns `std::nullopt` for invalid input
- Checks for overflow/underflow conditions

### Double to String Conversion
- Uses `std::ostringstream` with classic locale
- Automatic precision selection for optimal accuracy
- Scientific notation for very large/small values
- Ensures decimal point or exponent for clarity
- Normalizes exponent format (removes leading zeros)

### Error Handling
- Uses `std::optional` for safe optional return values
- No exceptions thrown - graceful error handling
- Boolean return values for C compatibility functions
- Buffer size checking for C-style string functions

### Locale Independence
- Uses `std::locale::classic()` for consistent behavior
- Rejects comma separators (only accepts dot)
- Consistent output format regardless of system locale

## Usage Examples

### Modern C++ Approach
```cpp
#include "jasson_cpp/string_converter.hpp"

// String to double
auto result = jansson::StringConverter::string_to_double("123.45");
if (result.has_value()) {
    double value = result.value();
    // Use value...
}

// Double to string
std::string str = jansson::StringConverter::double_to_string(123.45);
```

### Convenience Functions
```cpp
auto result = jansson::json_strtod("1.23e10");
std::string str = jansson::json_dtostr(1.23e10);
```

### C Compatibility
```cpp
#include "jasson_cpp/strconv_wrapper.hpp"

// C-style interface
double value;
bool success = jansson::compat::jsonp_strtod_wrapper("123.45", &value);

char buffer[256];
int len = jansson::compat::jsonp_dtostr_wrapper(buffer, sizeof(buffer), 123.45, 0);
```

## Benefits Over Original C Implementation

1. **Memory Safety**: No manual memory management
2. **Type Safety**: Strong typing prevents buffer overflows
3. **Error Handling**: Graceful error handling with `std::optional`
4. **Locale Independence**: Consistent behavior across platforms
5. **Modern C++**: Uses RAII, STL containers, and modern patterns
6. **Backward Compatibility**: Wrapper functions for gradual migration
7. **Performance**: Efficient implementation using standard library

## Testing

The implementation includes comprehensive tests covering:
- Basic conversions (string ↔ double, string ↔ integer)
- Edge cases (zero, infinity, very large/small numbers)
- Error conditions (invalid input, overflow, underflow)
- JSON compatibility (scientific notation, various formats)
- Round-trip conversion consistency
- Locale independence
- C compatibility wrapper functionality

## Integration

The StringConverter is integrated into the main Jansson-C++ library through:
- Inclusion in `jasson.hpp` main header
- Compatibility wrapper for existing code
- Drop-in replacement for original `strconv.c` functionality

This implementation fulfills the requirements specified in the PR description for Phase 3 Step 1: converting string conversion functionality to modern C++ while maintaining API compatibility.
