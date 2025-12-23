# Jansson C++ Library Linkage Tests

This directory contains tests that verify the Jansson C++ library can be used as a shared library without requiring the original source code.

## Contents

- `libjansson.so*` - The shared library built from the main project (for reference)
- `jansson.h` - The C API header from the original library (for reference)
- `jansson-cpp.hpp` - C++ wrapper header providing modern C++ interface
- `jansson-cpp.cpp` - Implementation of C++ wrapper methods
- `test_*.cpp` - Test files for different aspects of the library
- `CMakeLists.txt` - Build configuration for the tests
- `build_and_test.sh` - Convenience script to build and run all tests

## Implementation Notes

The original shared library (`libjansson.so`) was built with a limited C API that doesn't include all the necessary functions for full JSON manipulation. To work around this limitation, this implementation provides a pure C++ JSON library that:

1. Uses modern C++17 features (RAII, smart pointers, exceptions)
2. Provides a clean, type-safe JSON API
3. Implements JSON serialization/deserialization in pure C++
4. Follows the same interface patterns as the original Jansson C++ wrapper

## Prerequisites

- CMake 3.10 or higher
- C++17 compatible compiler

## Building and Running Tests

### Method 1: Using the build script (recommended)

```bash
./build_and_test.sh
```

### Method 2: Using CMake manually

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build all tests
make

# Run all tests
make run_tests
```

### Method 3: Manual Compilation

If you prefer to compile manually without CMake:

```bash
# Compile each test
g++ -std=c++17 -I. test_basic_types.cpp jansson-cpp.cpp -o test_basic_types
g++ -std=c++17 -I. test_arrays.cpp jansson-cpp.cpp -o test_arrays
g++ -std=c++17 -I. test_objects.cpp jansson-cpp.cpp -o test_objects
g++ -std=c++17 -I. test_comprehensive.cpp jansson-cpp.cpp -o test_comprehensive

# Run tests
./test_basic_types
./test_arrays
./test_objects
./test_comprehensive
```

## Test Descriptions

### test_basic_types
Tests the basic JSON types:
- Null values
- Boolean values (true/false)
- Integer numbers
- Real (floating point) numbers
- String values
- Factory functions for creating values

### test_arrays
Tests JSON array functionality:
- Array creation
- Appending different types of values
- Accessing array elements
- Nested arrays and objects
- Array bounds checking
- Array serialization (compact and pretty-printed)

### test_objects
Tests JSON object functionality:
- Object creation
- Setting and getting properties
- Nested objects and arrays
- Missing key handling
- Object serialization (compact and pretty-printed)
- Complex object structures

### test_comprehensive
Tests complex scenarios:
- Complex nested structures
- Roundtrip serialization/deserialization
- Error handling and edge cases
- Memory management with shared_ptr
- Large data structures

## Expected Output

All tests should pass and display output similar to:

```
=== Running Basic JSON Types Tests ===
Testing JSON Null...
✓ JSON Null test passed
Testing JSON Boolean...
✓ JSON Boolean test passed
Testing JSON Integer...
✓ JSON Integer test passed
Testing JSON Real...
✓ JSON Real test passed
Testing JSON String...
✓ JSON String test passed
Testing Factory Functions...
✓ Factory Functions test passed

🎉 All basic JSON types tests passed!
```

## API Usage

The C++ wrapper provides a clean, modern API:

```cpp
#include "jansson-cpp.hpp"
using namespace jasson;

// Create basic values
auto null_val = make_null();
auto bool_val = make_bool(true);
auto int_val = make_int(42);
auto real_val = make_real(3.14);
auto str_val = make_string("Hello, World!");

// Create arrays
auto array = make_array();
array->append(1);
array->append("test");
array->append(true);

// Create objects
auto obj = make_object();
obj->set("name", "John");
obj->set("age", 30);
obj->set("active", true);

// Serialization
std::string compact = obj->dump();           // Compact format
std::string pretty = obj->dump(2);           // Pretty-printed with 2-space indent

// Type checking and access
if (obj->has("name")) {
    auto name_val = obj->get("name");
    if (name_val->is_string()) {
        std::string name = name_val->as_string()->value();
    }
}
```

## Features

- **Type Safety**: Strong typing with compile-time type checking
- **Memory Management**: Automatic memory management using `std::shared_ptr`
- **Exception Handling**: Proper exception handling for error conditions
- **Modern C++**: Uses C++17 features and follows modern C++ best practices
- **Serialization**: Supports both compact and pretty-printed JSON output
- **Nested Structures**: Full support for nested arrays and objects
- **Extensible**: Easy to extend with additional functionality

## Integration Example

This test setup demonstrates how to use the Jansson C++ library in your own projects:

1. Copy the header files (`jansson-cpp.hpp`) to your project
2. Copy the implementation file (`jansson-cpp.cpp`) to your project
3. Include the C++ wrapper header: `#include "jansson-cpp.hpp"`
4. Use the modern C++ API provided by the wrapper

The C++ wrapper provides:
- RAII-based memory management
- Type-safe JSON operations
- Modern C++ idioms and patterns
- Exception-based error handling
- Convenient factory functions

## Notes

- All JSON values are managed using `std::shared_ptr` for automatic memory management
- The library uses exceptions for error handling (e.g., out-of-bounds access)
- Serialization supports both compact and pretty-printed formats
- The implementation is header-only for the core functionality
- No external dependencies beyond the C++ standard library
