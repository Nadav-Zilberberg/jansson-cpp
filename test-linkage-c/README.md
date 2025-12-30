# Jansson C Linkage Test

This directory contains a C linkage test for the Jansson JSON library. The test verifies that the shared library (`libjansson.so`) can be used independently with only the public header (`jansson.h`) from a C application.

## Files in this directory:

- `jansson.h` - The public header file from the Jansson library
- `libjansson.so*` - The shared library files
- `test_linkage.c` - Test program written in C that exercises the library functionality
- `CMakeLists.txt` - CMake configuration for building the test (if CMake is available)
- `run_tests.sh` - Convenience script to build and run the tests
- `README.md` - This file

## Prerequisites

- C99 compatible compiler (gcc, clang, etc.)
- Linux/Unix environment (for shared library support)

## How to run the tests

### Method 1: Using the convenience script (Recommended)

```bash
# Make the script executable and run it
chmod +x run_tests.sh
./run_tests.sh
```

### Method 2: Direct compilation

```bash
# Compile directly with gcc
gcc -std=c99 -I. -L. test_linkage.c -ljansson -o test_linkage_c

# Set library path and run
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
./test_linkage_c
```

### Method 3: Using CMake (if available)

```bash
# Create a build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the test
make

# Run the test
./test_linkage_c

# Or run via CTest
ctest
```

## What the tests cover

The C linkage test verifies the following functionality:

1. **Basic JSON creation**: Creating null, boolean, integer, real, and string values
2. **Array operations**: Creating arrays, appending elements, retrieving elements
3. **Object operations**: Creating objects, setting properties, retrieving properties
4. **Serialization**: Converting JSON to string format
5. **Deserialization**: Converting String to JSON 
6. **Equality**: Comparing JSON structures

## Expected output

A successful test run should produce output similar to:

```
Running Jansson C Linkage Tests...
=================================
Running test: basic_json_creation
  Testing basic JSON creation...
  PASSED
Running test: json_array_operations
  Testing JSON array operations...
  PASSED
Running test: json_object_operations
  Testing JSON object operations...
  PASSED
Running test: json_serialization
  Testing JSON serialization...
    Serialized JSON: {"data":["test",42],"name":"example"}
  Deserialization test passed
  PASSED
Running test: json_equality
  Testing JSON equality...
  PASSED
=================================
Test Results: 5 passed, 0 failed
```

## Troubleshooting

### Library not found
If you get an error about the library not being found:
- Make sure `libjansson.so` is in the current directory
- Set `LD_LIBRARY_PATH` to include the current directory: `export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH`

### Compilation errors
- Ensure you have a C99 compatible compiler
- Check that `jansson.h` is present in the directory

### Runtime errors
- Verify that the shared library matches the header file version
- Check that all required dependencies are available

## Purpose

This test demonstrates that the Jansson library can be used as a shared library dependency from C code without requiring the source code or static linking. This is useful for:

- C applications that need JSON processing
- Distribution packaging
- Dynamic loading scenarios
- Verifying ABI compatibility
- Testing library installation from C programs

## Differences from C++ version

This C version provides the same functionality as the C++ test but:
- Uses pure C syntax and standard library
- No C++ features like classes, references, or STL containers
- Uses `printf` instead of `iostream`
- Uses function pointers instead of C++ objects
- Follows C naming conventions and memory management patterns
