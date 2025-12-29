# Jansson Linkage Test

This directory contains comprehensive linkage tests for the Jansson JSON library using GoogleTest. The tests verify that the shared library (`libjansson.so`) can be used independently with only the public header (`jansson.h`).

## Files in this directory:

- `jansson.h` - The public header file from the Jansson library
- `libjansson.so*` - The shared library files
- `test_linkage.cpp` - Original test program (for compatibility)
- `test_string_conversion.cpp` - Tests for string to JSON and JSON to string conversion
- `test_json_builder.cpp` - Tests for JSON builder functionality
- `test_literal_operators.cpp` - Tests for literal operators
- `test_complex_structures.cpp` - Tests for complex JSON structures
- `test_exceptions.cpp` - Tests for exception handling
- `CMakeLists.txt` - CMake configuration for building tests with GoogleTest
- `Makefile` - Alternative build configuration (if CMake is not available)
- `README.md` - This file

## Prerequisites

- CMake 3.14 or higher (recommended)
- C++17 compatible compiler
- Linux/Unix environment (for shared library support)
- GoogleTest (will be downloaded automatically if not available)

## How to run the tests

### Method 1: Using CMake (Recommended)

```bash
# Create a build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build all tests
make

# Run all tests
ctest

# Or run individual tests
./test_string_conversion
./test_json_builder
./test_literal_operators
./test_complex_structures
./test_exceptions
```

### Method 2: Using Make (Alternative)

```bash
# Build all tests
make

# Run all tests
make test

# Or run individual tests
./test_string_conversion
./test_json_builder
./test_literal_operators
./test_complex_structures
./test_exceptions
```

### Method 3: Direct compilation (for individual tests)

```bash
# Compile individual test
g++ -std=c++17 -I. -L. test_string_conversion.cpp -ljansson -lgtest -lgtest_main -pthread -o test_string_conversion

# Set library path and run
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
./test_string_conversion
```

## Test Suites

The tests are organized into the following test suites:

### 1. String Conversion Tests (`test_string_conversion.cpp`)
- String to JSON conversion
- JSON to string conversion
- Pretty printing JSON objects
- Error handling for invalid JSON strings

### 2. JSON Builder Tests (`test_json_builder.cpp`)
- Building JSON objects programmatically
- Building JSON arrays
- Adding properties to JSON objects
- Nested JSON structures

### 3. Literal Operator Tests (`test_literal_operators.cpp`)
- JSON literal operators (if available)
- String literal operators
- Number literal operators

### 4. Complex Structures Tests (`test_complex_structures.cpp`)
- Nested objects and arrays
- Mixed data types
- Large JSON structures
- Deep nesting

### 5. Exception Tests (`test_exceptions.cpp`)
- Invalid JSON parsing
- Type conversion errors
- Memory allocation failures
- Boundary conditions

## Expected output

A successful test run should produce output similar to:

```
[==========] Running 15 tests from 5 test suites.
[----------] Global test environment set-up.
[----------] 3 tests from StringConversionTest
[ RUN      ] StringConversionTest.BasicConversion
[       OK ] StringConversionTest.BasicConversion (0 ms)
[ RUN      ] StringConversionTest.PrettyPrint
[       OK ] StringConversionTest.PrettyPrint (0 ms)
[ RUN      ] StringConversionTest.InvalidJson
[       OK ] StringConversionTest.InvalidJson (0 ms)
[----------] 3 tests from StringConversionTest (0 ms total)
...
[==========] 15 tests from 5 test suites ran. (1 ms total)
[  PASSED  ] 15 tests.
```

## Troubleshooting

### Library not found
If you get an error about the library not being found:
- Make sure `libjansson.so` is in the current directory
- Set `LD_LIBRARY_PATH` to include the current directory: `export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH`

### GoogleTest not found
If GoogleTest is not available on your system:
- CMake will automatically download it (requires internet connection)
- Or install it via your package manager: `sudo apt-get install libgtest-dev` (Ubuntu/Debian)

### Compilation errors
- Ensure you have a C++17 compatible compiler
- Check that `jansson.h` is present in the directory
- Verify GoogleTest is properly installed or downloaded

### Runtime errors
- Verify that the shared library matches the header file version
- Check that all required dependencies are available
- Ensure proper library path configuration

## Purpose

This comprehensive test suite demonstrates that the Jansson library can be used as a shared library dependency with modern C++ testing practices. This is useful for:

- Distribution packaging
- Dynamic loading scenarios
- Verifying ABI compatibility
- Testing library installation
- Ensuring all library features work correctly
- Validating C++ integration

## Test Coverage

The test suites cover all major functionality provided by the Jansson library:
- ✅ String to JSON conversion
- ✅ JSON to string conversion
- ✅ JSON builder functionality
- ✅ Literal operators
- ✅ Complex structures
- ✅ Exception handling
- ✅ Reference counting
- ✅ Memory management
