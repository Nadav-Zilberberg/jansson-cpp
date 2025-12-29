# Jansson Linkage Tests

This directory contains comprehensive linkage tests for the Jansson JSON library using GoogleTest framework.

## Files in this directory

- `jansson.h` - The Jansson library header file
- `libjansson.so*` - The shared library files
- `CMakeLists.txt` - CMake configuration for building the tests
- `README.md` - This documentation file
- `test_*.cpp` - Individual test suites:
  - `test_linkage.cpp` - Basic linkage and functionality tests
  - `test_string_conversion.cpp` - String to JSON and JSON to string conversion tests
  - `test_json_builder.cpp` - JSON object and array building tests
  - `test_literals_and_exceptions.cpp` - JSON literal parsing and error handling tests

## Building and Running Tests

### Prerequisites
- CMake 3.28 or higher
- C++17 compatible compiler
- GoogleTest (will be fetched automatically by CMake)

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

### Running Tests

```bash
# Run all tests
./jansson_linkage_tests

# Run specific test suite
./jansson_linkage_tests --gtest_filter=StringConversionTest.*

# Run with verbose output
./jansson_linkage_tests --gtest_output=xml:test_results.xml
```

### Using CTest

```bash
# Run all tests through CTest
ctest

# Run with verbose output
ctest -V
```

## Test Coverage

The tests cover the following Jansson library features:

1. **Basic JSON Types**
   - Null values
   - Boolean values (true/false)
   - Integer numbers
   - Real numbers
   - Strings

2. **JSON Objects**
   - Object creation and destruction
   - Property setting and getting
   - Nested objects

3. **JSON Arrays**
   - Array creation and destruction
   - Element appending and retrieval
   - Array size management

4. **String Conversion**
   - Parsing JSON from strings
   - Serializing JSON to strings
   - Compact and pretty-printed formats

5. **Complex Structures**
   - Nested objects and arrays
   - Mixed data types
   - Real-world JSON examples

6. **Error Handling**
   - Invalid JSON parsing
   - Error reporting
   - Graceful error recovery

7. **Literals and Escaping**
   - String escape sequences
   - Array literals
   - Object literals

## Example Usage

```cpp
#include <gtest/gtest.h>
extern "C" {
#include "jansson.h"
}

TEST(MyTest, BasicJson) {
    // Create a JSON object
    json_t* obj = json_object();
    ASSERT_NE(obj, nullptr);
    
    // Add properties
    json_object_set(obj, "name", json_string("test"));
    json_object_set(obj, "value", json_integer(42));
    
    // Serialize to string
    char* json_str = json_dumps(obj, JSON_COMPACT);
    EXPECT_STREQ(json_str, "{\"name\":\"test\",\"value\":42}");
    
    // Cleanup
    free(json_str);
    json_decref(obj);
}
```

## Integration with Main Project

These tests are designed to verify that the Jansson library can be properly linked and used from C++ code. They serve as both integration tests and examples of how to use the library in modern C++ applications.

The tests follow modern C++ best practices:
- Use of RAII principles (though the C library requires manual memory management)
- Proper error handling
- Comprehensive test coverage
- Clear and descriptive test names
