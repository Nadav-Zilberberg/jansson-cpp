# C++ JSON Library Demo

This demo showcases all features of the modern C++ JSON library.

## Compilation Instructions

To compile the demo program, use the following g++ command:

```bash
g++ -std=c++20 -I../src json_demo.cpp -o json_demo
```

## Running the Demo

After successful compilation, run the demo with:

```bash
./json_demo
```

## Features Demonstrated

The demo program demonstrates the following features:

1. **Basic JSON Types**
   - Strings
   - Numbers (integers and real numbers)
   - Booleans (true/false)
   - Null values

2. **JSON Objects**
   - Creating objects with key-value pairs
   - Nested objects
   - Object manipulation

3. **JSON Arrays**
   - Creating arrays
   - Mixed-type arrays
   - Array manipulation

4. **Fluent Builder API**
   - Chain method calls for building JSON structures
   - Object and array builders
   - Type-safe construction

5. **Literal Operators**
   - String literals: `"text"_json`
   - Integer literals: `42_json`
   - Real number literals: `3.14_json`

6. **Convenience Builders**
   - `object_builder()` for creating objects
   - `array_builder()` for creating arrays
   - Type-safe fluent interface

7. **Type Checking**
   - Runtime type checking methods
   - Safe type introspection

8. **Complex Structures**
   - Nested objects and arrays
   - Real-world JSON examples

## Library Design

The library follows modern C++ practices:

- **RAII**: Automatic memory management
- **Smart Pointers**: No manual memory management
- **Exceptions**: Safe error handling
- **Namespaces**: Organized code structure
- **C++20 Features**: Modern language features

## Error Handling

The library uses exceptions for error handling. All operations are designed to be exception-safe.

## Dependencies

- C++20 compatible compiler
- Standard library only (no external dependencies)
