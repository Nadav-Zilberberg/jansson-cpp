# C to C++ Conversion Plan for Jansson Library

## Overview
This plan outlines the conversion of the Jansson C library to modern C++ in a new `jasson-cpp` directory. The conversion will focus on maintaining API compatibility while implementing modern C++ practices including RAII, smart pointers, STL containers, and type safety.

## Modern C++ Practices to Apply
- Replace raw structs with encapsulated classes using constructors, destructors, and RAII
- Replace raw pointers with smart pointers and references where appropriate
- Use `std::unique_ptr`, `std::shared_ptr`, and `std::weak_ptr` instead of manual memory allocation
- Use `std::vector`, `std::string`, `std::optional`, `std::variant`, `std::filesystem`, and other STL utilities
- Use `enum class` instead of plain enums
- Use exceptions for error handling instead of error codes where appropriate
- Apply modern C++17 features (auto, range-based loops, smart iterators, string views, move semantics, constexpr)
- Use templates and type-safe abstractions instead of generic void pointers
- Ensure code is const-correct and thread-safe where needed

## Files to Skip
- `src/dtoa.c` - Third-party implementation for double-to-string conversion
- `src/lookup3.h` - Third-party hash function implementation
- `src/lookup3.c` - Not present (header-only implementation)

## Conversion Phases

### Phase 1: Convert Core Infrastructure (No Internal Dependencies)

#### Step 1.1: Convert error handling (`error.c`)
- Convert `json_error_t` struct to a class with proper encapsulation
- Convert `enum json_error_code` to `enum class JsonErrorCode`
- Replace error formatting functions with exception-based error handling
- Create custom exception hierarchy for different error types

#### Step 1.2: Convert memory management (`memory.c`)
- Replace custom allocation functions with RAII-based memory management
- Create memory resource classes for custom allocators
- Implement smart pointer support for JSON values
- Remove global allocation function pointers in favor of dependency injection

#### Step 1.3: Convert string buffer (`strbuffer.c`, `strbuffer.h`)
- Replace `strbuffer_t` struct with `StringBuffer` class
- Use `std::string` internally instead of manual buffer management
- Implement move semantics for efficient string handling
- Add proper RAII for automatic memory management

#### Step 1.4: Convert UTF-8 utilities (`utf.c`, `utf.h`)
- Convert UTF-8 validation functions to use `std::string_view`
- Create `Utf8String` class for validated UTF-8 strings
- Use `std::optional` for functions that may fail
- Implement Unicode code point handling with proper type safety

### Phase 2: Convert Core Data Structures

#### Step 2.1: Convert hashtable (`hashtable.c`, `hashtable.h`)
- Replace `hashtable_t` struct with `HashTable` template class
- Use `std::unordered_map` as the underlying implementation
- Implement proper iterators compatible with STL
- Add thread-safety with appropriate synchronization primitives
- Use `std::string` for keys instead of raw char pointers

#### Step 2.2: Convert value types (`value.c`)
- Convert `json_t` base class to abstract base class `JsonValue`
- Create concrete classes for each JSON type:
  - `JsonObject` (inherits from `JsonValue`)
  - `JsonArray` (inherits from `JsonValue`)
  - `JsonString` (inherits from `JsonValue`)
  - `JsonNumber` (inherits from `JsonValue`)
  - `JsonBoolean` (inherits from `JsonValue`)
  - `JsonNull` (inherits from `JsonValue`)
- Implement reference counting with `std::shared_ptr`
- Use `std::variant` for storing different value types
- Add proper copy/move semantics and const-correctness

### Phase 3: Convert String Conversion Utilities

#### Step 3.1: Convert string conversion (`strconv.c`)
- Replace C-style string conversion with `std::to_string` and `std::stod`
- Create `StringConverter` class for locale-independent conversions
- Use `std::optional` for conversion functions that may fail
- Implement proper error handling with exceptions

### Phase 4: Convert Serialization/Deserialization

#### Step 4.1: Convert JSON loading (`load.c`)
- Create `JsonLoader` class with configurable options
- Use `std::istream` and `std::ifstream` for input handling
- Implement `std::filesystem::path` support for file loading
- Replace callback-based loading with modern iterator-based approach
- Use exceptions for error reporting instead of `json_error_t`

#### Step 4.2: Convert JSON dumping (`dump.c`)
- Create `JsonDumper` class with configurable formatting options
- Use `std::ostream` and `std::ofstream` for output handling
- Implement proper indentation and formatting controls
- Add support for custom output formats through inheritance
- Use `std::string` for string output instead of manual buffer management

### Phase 5: Convert Advanced Features

#### Step 5.1: Convert pack/unpack functionality (`pack_unpack.c`)
- Create `JsonPacker` and `JsonUnpacker` classes
- Use variadic templates instead of C-style varargs
- Implement type-safe packing/unpacking with template metaprogramming
- Use `std::tuple` and `std::apply` for structured data
- Add compile-time format string validation where possible

### Phase 6: Create Modern C++ API

#### Step 6.1: Design modern C++ API layer
- Create fluent API for JSON construction
- Implement operator overloading for intuitive syntax
- Add range-based constructors for containers
- Implement modern iterator support
- Create builder pattern for complex JSON structures

#### Step 6.2: Add convenience functions
- Create `json::parse()` and `json::stringify()` functions
- Implement `std::to_json()` and `std::from_json()` for custom types
- Add support for C++20 concepts where applicable
- Create type traits for JSON compatibility

### Phase 7: Testing and Validation

#### Step 7.1: Create comprehensive test suite
- Write unit tests for each converted component
- Ensure API compatibility with existing C API
- Test exception safety and RAII behavior
- Validate thread safety of concurrent operations
- Benchmark performance against original C implementation

#### Step 7.2: Documentation and examples
- Create modern C++ documentation with examples
- Write migration guide from C to C++ API
- Document best practices for modern C++ usage
- Create example applications demonstrating new features

## Implementation Notes

1. **Backward Compatibility**: Maintain a C-compatible API layer during transition
2. **Performance**: Benchmark critical paths to ensure no performance regression
3. **Memory Management**: Use smart pointers consistently to eliminate memory leaks
4. **Error Handling**: Replace error codes with exceptions for better error propagation
5. **Thread Safety**: Add appropriate synchronization for concurrent access
6. **Type Safety**: Use strong typing and templates to prevent runtime errors
7. **Modern Features**: Leverage C++17 features while maintaining C++14 compatibility where needed

## Success Criteria

- All core functionality preserved and tested
- Memory leaks eliminated through RAII
- Thread-safe operations where applicable
- Modern C++ idioms consistently applied
- Performance comparable to or better than original C implementation
- Comprehensive documentation and examples provided
