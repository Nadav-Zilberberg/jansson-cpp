# Jansson Shared Library Linkage Tests

This directory contains tests that verify the shared library (`libjansson.so`) can be used as a standalone library without the original source code.

## Files in this directory:

- `libjansson.so` - The shared library built from the original source
- `jansson.h` - Minimal C header file with the public API
- `jansson_cpp.hpp` - C++ wrapper providing RAII and modern C++ interface
- `test_basic.cpp` - Basic test without GoogleTest dependency
- `test_linkage.cpp` - Comprehensive tests using GoogleTest
- `CMakeLists.txt` - Build configuration

## Prerequisites

- CMake 3.10 or higher
- C++17 compatible compiler
- GoogleTest (GTest) library

## Building and Running Tests

### Method 1: Using CMake

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the tests
make

# Run all tests
ctest --verbose

# Or run individual tests
./test_basic
./test_linkage
```

### Method 2: Manual compilation (basic test only)

```bash
# Compile the basic test
g++ -std=c++17 -I. -L. test_basic.cpp -ljansson -o test_basic

# Run the test
LD_LIBRARY_PATH=. ./test_basic
```

### Method 3: Using the provided make target

```bash
# From the build directory
make run_tests
```

## Test Output

The tests will output:
- JSON objects being created and manipulated
- Success/failure status for each test
- Detailed error messages if any tests fail
- Reference counting behavior verification
- Type checking validation

## Troubleshooting

### Library not found error
If you get an error about the library not being found, make sure to set the library path:
```bash
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
```

### GoogleTest not found
If CMake cannot find GoogleTest, you may need to install it:
```bash
# On Ubuntu/Debian
sudo apt-get install libgtest-dev

# On macOS with Homebrew
brew install googletest
```

### Compilation errors
Ensure you have a C++17 compatible compiler and that all dependencies are properly installed.

## What these tests verify

1. **Basic functionality**: Creation of all JSON types (object, array, string, number, boolean, null)
2. **Reference counting**: Proper increment/decrement of reference counts
3. **Memory management**: No memory leaks through RAII
4. **Type safety**: Correct type identification and checking
5. **Move semantics**: Proper handling of C++11 move semantics
6. **Copy semantics**: Safe copying of JSON values

These tests simulate how an external developer would use the jansson library as a shared library dependency in their project.
