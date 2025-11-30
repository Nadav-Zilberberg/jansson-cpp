# JSON Library Examples

This directory contains example programs demonstrating the usage of the modern C++ JSON library.

## Building the Examples

The examples are built automatically when you build the main project if `JANSSON_EXAMPLES` is set to `ON` in CMake.

To build manually:
```bash
cd examples
mkdir build
cd build
cmake ..
make
```

## Running the Examples

After building, all example executables will be in the `build/` directory.

### Available Examples

1. **example** - Comprehensive demonstration of all major features
2. **simple_parse** - Basic JSON parsing example for beginners
3. **json_array_example** - Working with JSON arrays
4. **json_object_example** - Working with JSON objects
5. **error_handling_example** - Proper error handling techniques
6. **type_conversion_example** - Converting between JSON and C++ types
7. **performance_example** - Performance optimization techniques
8. **advanced_features_example** - Advanced library features

## Example Usage

```bash
./build/example
./build/simple_parse
./build/json_array_example
```

Each example is self-contained and includes comments explaining the code.
