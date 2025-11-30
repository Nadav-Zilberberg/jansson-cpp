# Conversion Plan

## Phase 1: Create the unit tests

### Step 1: Set up testing framework
Step 1.1: Choose Google Test (gtest) as the testing framework (already used in cpp_tests)
Step 1.2: Create a new CMakeLists.txt in tests/ directory for unit tests
Step 1.3: Configure CMake to build unit tests alongside the main library
Step 1.4: Set up test discovery and execution in CMake

### Step 2: Create unit tests for core source files
Step 2.1: Create unit test for src/error.cpp
Step 2.2: Create unit test for src/hashtable.cpp
Step 2.3: Create unit test for src/hashtable_seed.cpp
Step 2.4: Create unit test for src/hashtable_simple.cpp
Step 2.5: Create unit test for src/json_dumper.cpp
Step 2.6: Create unit test for src/json_loader.cpp
Step 2.7: Create unit test for src/json_packer.cpp
Step 2.8: Create unit test for src/json_types.cpp
Step 2.9: Create unit test for src/json_unpacker.cpp
Step 2.10: Create unit test for src/memory.cpp
Step 2.11: Create unit test for src/string_converter.cpp
Step 2.12: Create unit test for src/utf.cpp

### Step 3: Create unit tests for header-only functionality
Step 3.1: Create unit test for src/json_value.hpp (JSON value operations)
Step 3.2: Create unit test for src/json_factory.hpp (JSON creation functions)
Step 3.3: Create unit test for src/json_extensions.hpp (extension functions)
Step 3.4: Create unit test for src/json_convenience.hpp (convenience functions)
Step 3.5: Create unit test for src/type_traits.hpp (type trait utilities)

### Step 4: Create integration tests
Step 4.1: Create test for complete JSON parsing and serialization workflow
Step 4.2: Create test for error handling and reporting
Step 4.3: Create test for memory management and cleanup
Step 4.4: Create test for nested JSON structure manipulation
Step 4.5: Create test for type conversion between JSON and C++ types

## Phase 2: Create the example usage folder

### Step 1: Create examples directory structure
Step 1.1: Create examples/ folder in project root
Step 1.2: Create examples/CMakeLists.txt for building examples
Step 1.3: Update main CMakeLists.txt to include examples subdirectory

### Step 2: Create comprehensive example.cpp
Step 2.1: Create example showing basic JSON value creation
Step 2.2: Add example demonstrating JSON parsing from string
Step 2.3: Add example showing JSON serialization to string
Step 2.4: Add example of error handling when parsing invalid JSON
Step 2.5: Add example of accessing nested JSON structures
Step 2.6: Add example of modifying existing JSON values
Step 2.7: Add example of converting JSON to native C++ types
Step 2.8: Add example of creating JSON from native C++ types
Step 2.9: Add example of working with JSON arrays
Step 2.10: Add example of working with JSON objects
Step 2.11: Add example of pretty-printing JSON
Step 2.12: Add example of compact JSON output

### Step 3: Create additional focused examples
Step 3.1: Create simple_parse.cpp (equivalent to the missing C version)
Step 3.2: Create json_array_example.cpp
Step 3.3: Create json_object_example.cpp
Step 3.4: Create error_handling_example.cpp
Step 3.5: Create type_conversion_example.cpp

### Step 4: Document examples
Step 4.1: Create examples/README.md explaining each example
Step 4.2: Add build instructions for examples
Step 4.3: Add usage instructions for each example

## Phase 3: Fix/remove C remnants

### Step 1: Identify and remove obsolete C-related files
Step 1.1: Remove src/jansson.h (C header file)
Step 1.2: Remove android/jansson_config.h (C configuration)
Step 1.3: Remove cpp_tests/src/jasson_config.h (test configuration)
Step 1.4: Remove any remaining .c files in src/ directory

### Step 2: Update CMakeLists.txt to remove C references
Step 2.1: Remove CMAKE_C_COMPILER references
Step 2.2: Remove CMAKE_C_FLAGS references
Step 2.3: Update file(GLOB JANSSON_SRC src/*.c) to use .cpp files
Step 2.4: Remove C-specific compiler flags and settings
Step 2.5: Update simple_parse executable to use .cpp extension

### Step 3: Clean up configuration files
Step 3.1: Remove C-specific configuration from jansson_config.h.cmake
Step 3.2: Update jansson_private_config.h.cmake for C++ only
Step 3.3: Remove C-specific inline macro definitions
Step 3.4: Remove C-style extern "C" guards from headers

### Step 4: Update documentation
Step 4.1: Remove C API references from README.rst
Step 4.2: Update documentation to mention C++ only
Step 4.3: Remove C-specific build instructions
Step 4.4: Update API documentation to show C++ usage

### Step 5: Clean up build system
Step 5.1: Remove C-specific test suites references
Step 5.2: Update CMake minimum version if needed for C++
Step 5.3: Remove C-specific linker flags and options
Step 5.4: Update package configuration files for C++

## Phase 4: Repository validation

### Step 1: Ensure examples compile and run
Step 1.1: Test compilation of all examples
Step 1.2: Run each example and verify output
Step 1.3: Fix any compilation errors in examples
Step 1.4: Ensure examples work with installed library

### Step 2: Run full test suite
Step 2.1: Build and run all unit tests
Step 2.2: Build and run all integration tests
Step 2.3: Verify test coverage is comprehensive
Step 2.4: Fix any failing tests

### Step 3: Generate code coverage reports
Step 3.1: Configure CMake for coverage reporting
Step 3.2: Build library with coverage flags
Step 3.3: Run tests and generate coverage report
Step 3.4: Verify all source files have test coverage

### Step 4: Validate CMake configuration
Step 4.1: Test CMake configuration on different platforms
Step 4.2: Verify both shared and static library builds
Step 4.3: Test installation and package configuration
Step 4.4: Verify CMake targets are properly exported

### Step 5: Final cleanup and validation
Step 5.1: Remove any remaining C-style code patterns
Step 5.2: Ensure consistent C++ style throughout codebase
Step 5.3: Update version information if needed
Step 5.4: Create final validation report
Step 5.5: Tag release as C++ only version
