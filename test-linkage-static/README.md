# Jansson Static Library Linkage Tests

This directory contains tests for the Jansson static library (`libjansson.a`).

## Building and Running Tests

1. Make sure you have built the static library:
   ```bash
   cd /Nadav-Zilberberg__jansson-cpp
   mkdir -p build && cd build
   cmake .. && make
   ```

2. Build the tests:
   ```bash
   cd /Nadav-Zilberberg__jansson-cpp/test-linkage-static
   mkdir -p build && cd build
   cmake .. && make
   ```

3. Run all tests:
   ```bash
   ctest --verbose
   ```

4. Run individual tests:
   ```bash
   ./test_string_conversion
   ./test_json_to_string
   ./test_json_builder
   ./test_literal_operators
   ./test_complex_structures
   ./test_exceptions
   ```

## Test Coverage

The tests cover the following Jansson library features:

- **String to JSON conversion**: Testing parsing JSON from strings
- **JSON to string conversion**: Testing serialization of JSON objects to strings
- **JSON Builder**: Testing programmatic creation of JSON objects
- **Literal Operators**: Testing C++ literal operators for JSON creation
- **Complex Structures**: Testing nested JSON objects and arrays
- **Exceptions**: Testing error handling and exception throwing

## Files

- `libjansson.a` - The static library built from the main project
- `jansson.h` - The main header file for the Jansson library
- `CMakeLists.txt` - CMake configuration for building the tests
- `test_*.cpp` - Individual test files for each feature area
