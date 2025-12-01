# Conversion Plan for C Tests to Modern C++ Tests

## Overview
This plan outlines the conversion of existing C test code in the test directory to modern C++ tests in a new tests_cpp directory. The conversion will focus on quality code using modern C++ practices.

## Modern C++ Practices to Apply
- Replace raw structs with encapsulated classes using constructors, destructors, and RAII
- Replace raw pointers with smart pointers and references where appropriate
- Use std::unique_ptr, std::shared_ptr, and std::weak_ptr instead of manual memory allocation
- Use STL containers (std::vector, std::string) instead of custom implementations
- Use enum class instead of plain enums
- Use exceptions for error handling instead of error codes where appropriate
- Apply C++17 features (auto, range-based loops, smart iterators, string views, move semantics, constexpr)
- Use templates and type-safe abstractions instead of generic void pointers
- Ensure const-correctness and thread-safety where needed

## Phase 1: Infrastructure Setup

### Step 1: Create tests_cpp directory structure
- Create tests_cpp directory in root
- Create subdirectories for different test categories (api, encoding, invalid, etc.)
- Create CMakeLists.txt or Makefile for building tests
- Set up test framework (Google Test or Catch2)

### Step 2: Create base test fixtures
- Create base test class with common functionality
- Create test utilities for JSON operations
- Set up memory leak detection
- Create test data generators

## Phase 2: Convert Core API Tests

### Step 3: Convert test_equal.c
- Create test_equal.cpp with test fixture
- Replace C-style assertions with C++ test framework assertions
- Use RAII for JSON value management
- Test all equality scenarios

### Step 4: Convert test_copy.c
- Create test_copy.cpp with test fixture
- Use smart pointers for JSON value management
- Test deep copy functionality
- Verify reference counting behavior

### Step 5: Convert test_deep_copy.c
- Create test_deep_copy.cpp
- Test recursive copying of complex JSON structures
- Verify memory management with RAII
- Test edge cases (circular references, etc.)

## Phase 3: Convert Type-Specific Tests

### Step 6: Convert test_string.c
- Create test_string.cpp
- Test string creation, manipulation, and destruction
- Verify UTF-8 handling
- Test string comparison and searching

### Step 7: Convert test_number.c
- Create test_number.cpp
- Test integer and floating-point number handling
- Verify precision and range limits
- Test number-to-string conversions

### Step 8: Convert test_array.c
- Create test_array.cpp
- Test array creation and manipulation
- Verify index-based operations
- Test array iteration with range-based loops

### Step 9: Convert test_object.c
- Create test_object.cpp
- Test object creation and key-value operations
- Verify iteration over object members
- Test key existence and retrieval

## Phase 4: Convert Advanced Feature Tests

### Step 10: Convert test_load.c
- Create test_load.cpp
- Test JSON loading from strings and files
- Verify error handling for malformed JSON
- Use std::filesystem for file operations

### Step 11: Convert test_dump.c
- Create test_dump.cpp
- Test JSON serialization to strings and files
- Verify formatting options
- Test pretty-printing functionality

### Step 12: Convert test_pack.c
- Create test_pack.cpp
- Test variable argument packing
- Verify type safety with templates
- Test error handling for invalid arguments

### Step 13: Convert test_unpack.c
- Create test_unpack.cpp
- Test unpacking JSON to native types
- Verify type checking and conversion
- Test error handling for type mismatches

## Phase 5: Convert Error Handling Tests

### Step 14: Convert test_error.c
- Create test_error.cpp
- Test error code propagation
- Verify error message formatting
- Test error recovery scenarios

### Step 15: Convert test_loadb.c
- Create test_loadb.cpp
- Test loading with byte buffers
- Verify buffer size handling
- Test memory allocation failures

## Phase 6: Convert Memory Management Tests

### Step 16: Convert test_memory_funcs.c
- Create test_memory_funcs.cpp
- Test custom memory allocators
- Verify memory leak detection
- Test allocation failure handling

### Step 17: Convert test_sprintf.c
- Create test_sprintf.cpp
- Test string formatting functions
- Verify buffer management
- Test format string validation

## Phase 7: Convert Unicode Tests

### Step 18: Convert encoding-flags tests
- Create test_encoding_flags.cpp
- Test UTF-8 validation
- Verify Unicode normalization
- Test encoding conversion errors

### Step 19: Convert invalid-unicode tests
- Create test_invalid_unicode.cpp
- Test handling of invalid UTF-8 sequences
- Verify error reporting for Unicode errors
- Test recovery from invalid input

## Phase 8: Convert Validation Tests

### Step 20: Convert valid tests
- Create test_valid.cpp
- Test validation of valid JSON documents
- Verify acceptance of edge cases
- Test performance with large documents

### Step 21: Convert invalid tests
- Create test_invalid.cpp
- Test rejection of invalid JSON
- Verify appropriate error messages
- Test error position reporting

## Phase 9: Integration and Performance

### Step 22: Create integration tests
- Test interaction between different components
- Verify thread safety of operations
- Test concurrent access patterns

### Step 23: Create performance tests
- Benchmark JSON parsing and serialization
- Compare performance with C version
- Test memory usage patterns

### Step 24: Create stress tests
- Test with very large JSON documents
- Verify handling of deeply nested structures
- Test resource exhaustion scenarios

## Phase 10: Final Review and Cleanup

### Step 25: Code review
- Review all converted tests for consistency
- Verify proper use of modern C++ features
- Check for memory leaks and resource management

### Step 26: Documentation
- Document test coverage
- Create usage examples
- Update build instructions

### Step 27: CI/CD Integration
- Set up automated test execution
- Configure code coverage reporting
- Set up performance regression testing

## Files to Skip
- Tests for deprecated features
- Tests for internal implementation details
- Duplicate test coverage
- Platform-specific tests that are no longer relevant

## Quality Assurance
- Each converted test must pass before moving to the next
- Use static analysis tools to check for issues
- Verify no memory leaks with Valgrind or similar
- Ensure thread safety where applicable
- Maintain or improve test coverage

## Timeline
- Phase 1: 1-2 days
- Phase 2: 2-3 days
- Phase 3: 2-3 days
- Phase 4: 3-4 days
- Phase 5: 1-2 days
- Phase 6: 1-2 days
- Phase 7: 2-3 days
- Phase 8: 2-3 days
- Phase 9: 3-4 days
- Phase 10: 2-3 days

Total estimated time: 3-4 weeks
