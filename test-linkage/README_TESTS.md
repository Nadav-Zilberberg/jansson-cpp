# Jansson Linkage Test Suite

This directory contains the improved test suite for the Jansson JSON library linkage testing.

## Overview

The test suite has been restructured to support:
- Multiple test files organized by functionality
- GoogleTest integration (when available)
- Individual and batch test execution
- Proper C++17 compatibility

## Directory Structure

```
test-linkage/
├── jansson.h              # Jansson header file
├── libjansson.so*         # Shared library files
├── CMakeLists.txt         # CMake configuration (supports GoogleTest)
├── build_tests.sh         # Build script for all tests
├── run_tests.sh           # Test runner script
├── setup_googletest.sh    # GoogleTest setup script
├── test_linkage.cpp       # Original test file (for reference)
├── test_*.cpp             # Individual test suite files
└── README_TESTS.md        # This file
```

## Test Suites

The test suite is divided into the following categories:

1. **String to JSON Conversion** - Tests parsing JSON from strings
2. **JSON to String Conversion** - Tests serializing JSON to strings
3. **JSON Builder** - Tests programmatic JSON construction
4. **Literal Operators** - Tests C++ literal operators for JSON
5. **Complex Structures** - Tests nested and complex JSON structures
6. **Exception Handling** - Tests error conditions and exceptions

## Building Tests

### Simple Build (Current)
```bash
./build_tests.sh
```

### CMake Build (When cmake is available)
```bash
mkdir build && cd build
cmake ..
make
```

### GoogleTest Build (When GoogleTest is available)
```bash
./setup_googletest.sh
mkdir build && cd build
cmake ..
make
```

## Running Tests

### Run All Tests
```bash
./run_tests.sh
```

### Run Individual Tests
```bash
./build/test_string_to_json
./build/test_json_to_string
./build/test_json_builder
# etc.
```

### Run with CMake/CTest
```bash
cd build
ctest
# or
ctest -V  # verbose output
```

## Requirements

- C++17 compatible compiler (g++ 7+ or clang++ 5+)
- Jansson shared library (libjansson.so)
- GoogleTest (optional, for enhanced test features)

## Adding New Tests

1. Create a new test file following the naming convention: `test_<feature>.cpp`
2. Include the Jansson header: `#include "jansson.h"`
3. Use the existing test structure as a template
4. Run `./build_tests.sh` to compile
5. Run `./run_tests.sh` to execute all tests

## Troubleshooting

### Library Not Found
If you get "cannot find -ljansson" errors, ensure libjansson.so is in the current directory.

### Runtime Library Issues
If tests fail to run with library errors, try:
```bash
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
./run_tests.sh
```

### GoogleTest Issues
If GoogleTest setup fails, the build system will fall back to the simple test approach.

## Notes

- All tests use C++17 features
- Tests are designed to be independent and can run separately
- The shared library must be in the same directory as the test executables
- Test output is designed to be parseable by automated tools
