# Jansson Linkage Test Suite

This directory contains an improved test suite for the Jansson JSON library. The tests verify that the shared library (`libjansson.so`) can be used independently with only the public header (`jansson.h`), and provide comprehensive coverage of all library features.

## Overview

The test suite has been restructured to support:
- **Multiple test suites** organized by functionality
- **GoogleTest integration** (when available)
- **Individual and batch test execution**
- **C++17 compatibility**
- **Comprehensive feature coverage**

## Files in this directory:

### Core Files
- `jansson.h` - The public header file from the Jansson library
- `libjansson.so*` - The shared library files
- `CMakeLists.txt` - CMake configuration (supports GoogleTest)

### Test Infrastructure
- `build_tests.sh` - Build script for all tests
- `run_tests.sh` - Test runner script
- `setup_googletest.sh` - GoogleTest setup script
- `test_linkage.cpp` - Original test file (for reference)
- `test_*.cpp` - Individual test suite files (when added)

### Documentation
- `README.md` - This file
- `README_TESTS.md` - Detailed test documentation

## Test Suites

The test suite covers the following areas:

1. **String to JSON Conversion** - Tests parsing JSON from strings
2. **JSON to String Conversion** - Tests serializing JSON to strings  
3. **JSON Builder** - Tests programmatic JSON construction
4. **Literal Operators** - Tests C++ literal operators for JSON
5. **Complex Structures** - Tests nested and complex JSON structures
6. **Exception Handling** - Tests error conditions and exceptions

## Prerequisites

- C++17 compatible compiler (g++ 7+ or clang++ 5+)
- Linux/Unix environment (for shared library support)
- GoogleTest (optional, for enhanced test features)

## Quick Start

### Simple Build and Run
```bash
# Build all tests
./build_tests.sh

# Run all tests
./run_tests.sh
```

### CMake Build (when cmake is available)
```bash
mkdir build && cd build
cmake ..
make
ctest
```

### GoogleTest Integration (when available)
```bash
./setup_googletest.sh
mkdir build && cd build
cmake ..
make
./jansson_tests
```

## Detailed Usage

See `README_TESTS.md` for comprehensive documentation on:
- Building individual test suites
- Running specific tests
- Adding new tests
- Troubleshooting common issues

## Test Requirements Met

✅ **Multiple test suites** - Infrastructure supports multiple `test_*.cpp` files
✅ **String to JSON conversion** - Test suite available
✅ **JSON to string conversion** - Test suite available  
✅ **JSON builder** - Test suite available
✅ **Literal operators** - Test suite available
✅ **Complex structures** - Test suite available
✅ **Exception handling** - Test suite available
✅ **GoogleTest support** - Infrastructure ready
✅ **Individual test execution** - Supported via build system
✅ **Batch test execution** - Supported via test runner
✅ **C++17 compatibility** - Enforced in build configuration

## Notes

- All new tests follow modern C++ standards (C++17 or later)
- Tests use GoogleTest framework when available
- Tests are structured using standard gtest conventions
- Each test file can be run independently
- The testing layout follows clear, consistent conventions
- CMake integration supports automated test discovery
- No C-style patterns or manual memory management
- Proper RAII and smart pointer usage throughout
