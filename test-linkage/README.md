# Jansson Linkage Test

This directory contains a linkage test for the Jansson JSON library. The test verifies that the shared library (`libjansson.so`) can be used independently with only the public header (`jansson.h`).

## Files in this directory:

- `jansson.h` - The public header file from the Jansson library
- `libjansson.so*` - The shared library files
- `test_linkage.cpp` - Test program that exercises the library functionality
- `CMakeLists.txt` - CMake configuration for building the test
- `README.md` - This file

## Prerequisites

- CMake 3.10 or higher
- C++17 compatible compiler
- Linux/Unix environment (for shared library support)

## How to run the tests

### Method 1: Using CMake (Recommended)

```bash
# Create a build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the test
make

# Run the test
./test_linkage

# Or run via CTest
ctest
```

### Method 2: Direct compilation

```bash
# Compile directly with g++
g++ -std=c++17 -I. -L. test_linkage.cpp -ljansson -o test_linkage

# Set library path and run
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
./test_linkage
```

## What the tests cover

The linkage test verifies the following functionality:

1. **Basic JSON creation**: Creating null, boolean, integer, real, and string values
2. **Array operations**: Creating arrays, appending elements, retrieving elements
3. **Object operations**: Creating objects, setting properties, retrieving properties
4. **Serialization**: Converting JSON to string format and parsing it back
5. **Equality**: Comparing JSON structures
6. **Reference counting**: Managing object lifetimes

## Expected output

A successful test run should produce output similar to:

```
Running Jansson Linkage Tests...
=================================
Running test: basic_json_creation... PASSED
Running test: json_array_operations... PASSED
Running test: json_object_operations... PASSED
Running test: json_serialization... PASSED
Running test: json_equality... PASSED
Running test: reference_counting... PASSED
=================================
Test Results: 6 passed, 0 failed
```

## Troubleshooting

### Library not found
If you get an error about the library not being found:
- Make sure `libjansson.so` is in the current directory
- Set `LD_LIBRARY_PATH` to include the current directory: `export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH`

### Compilation errors
- Ensure you have a C++17 compatible compiler
- Check that `jansson.h` is present in the directory

### Runtime errors
- Verify that the shared library matches the header file version
- Check that all required dependencies are available

## Purpose

This test demonstrates that the Jansson library can be used as a shared library dependency without requiring the source code or static linking. This is useful for:

- Distribution packaging
- Dynamic loading scenarios
- Verifying ABI compatibility
- Testing library installation

## Recent Improvements

The test-linkage folder has been significantly enhanced with modern C++ features:

### New Features
- **Modern C++ Interface**: Full C++17 support with RAII and smart pointers
- **Exception Handling**: Custom exception hierarchy for better error reporting
- **JSON Builder**: Fluent interface for constructing JSON objects
- **Literal Operators**: Convenient JSON creation using user-defined literals
- **Type Safety**: Strong typing with proper inheritance hierarchy
- **Memory Safety**: Automatic resource management prevents memory leaks

### Backward Compatibility
All existing C code continues to work without modification. The C interface remains fully functional and compatible with the original jansson library.

### Build Instructions
```bash
./final_build.sh
```

This will compile the shared library and run all tests automatically.

### Usage Examples
See IMPROVEMENTS.md for detailed usage examples and documentation.
