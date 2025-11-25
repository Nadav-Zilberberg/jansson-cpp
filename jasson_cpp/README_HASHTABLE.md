# Modern C++ HashTable Implementation

This directory contains a modern C++ implementation of the Jansson hashtable that replaces the original C implementation while maintaining API compatibility.

## Files Created

### Core Implementation
- `hashtable_simple.hpp` - Main HashTable class header with modern C++ features
- `hashtable_simple.cpp` - Implementation of the HashTable class
- `hashtable_seed.hpp` - Hash seed management for consistent hashing
- `hashtable_seed.cpp` - Implementation of hash seed initialization

### Compatibility Layer
- `hashtable_wrapper.h` - C API wrapper header
- `hashtable_wrapper.cpp` - C API wrapper implementation that allows C code to use the C++ hashtable

### Additional Files
- `hashtable.hpp` - Template-based HashTable implementation (more advanced, not used in current implementation)
- `hashtable.cpp` - Template implementation file
- `hashtable_impl.hpp` - Template implementation details
- `hashtable_compat.h/cpp` - Alternative compatibility layer

## Key Features

### Modern C++ Practices Applied
1. **RAII** - Automatic resource management through destructors
2. **Smart Pointers** - Not needed here as we manage json_t* references
3. **STL Containers** - Uses `std::unordered_map` as underlying implementation
4. **Thread Safety** - Uses `std::shared_mutex` for concurrent access
5. **Move Semantics** - Efficient transfer of resources
6. **Exception Safety** - Strong exception guarantees
7. **Type Safety** - Strong typing with templates and modern C++ features

### API Compatibility
- Maintains compatibility with existing C code through wrapper functions
- Supports both C-style string operations and C++ std::string operations
- Preserves the original jansson reference counting behavior

### Performance Features
- Thread-safe operations with shared/exclusive locking
- Efficient hash function using the original lookup3 algorithm
- Reserve capacity for better performance with known sizes
- Move semantics for efficient resource transfer

## Usage Examples

### C++ Usage
```cpp
#include "jasson_cpp/hashtable_simple.hpp"

jansson::HashTable table;
table.set("key", json_value);
json_t* retrieved = table.get("key");
table.del("key");
```

### C Usage (through wrapper)
```c
#include "jasson_cpp/hashtable_wrapper.h"

hashtable_t* table = hashtable_cpp_create();
hashtable_cpp_set(table, "key", 3, json_value);
json_t* retrieved = hashtable_cpp_get(table, "key", 3);
hashtable_cpp_destroy(table);
```

## Testing

Comprehensive tests have been implemented:
- Basic operations (insert, retrieve, delete)
- C API compatibility
- Iterator functionality
- Thread safety
- Move semantics
- Edge cases

All tests pass successfully, demonstrating the robustness of the implementation.

## Migration Path

The implementation provides a gradual migration path:
1. C code can continue using the wrapper API
2. New C++ code can use the native C++ API
3. Eventually, all code can migrate to the C++ API

## Thread Safety

The implementation is thread-safe for concurrent operations:
- Multiple threads can read simultaneously (shared lock)
- Write operations require exclusive access (exclusive lock)
- Reference counting is properly synchronized

## Memory Management

- Automatic cleanup through RAII
- Proper reference counting for json_t objects
- No memory leaks in error conditions
- Exception-safe operations
