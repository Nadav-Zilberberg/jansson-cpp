# Overview
This repository is a modern C++ JSON library that was converted from an older C implementation. While most of the core functionality has already been migrated to use idiomatic C++ (classes, RAII, smart pointers, namespaces, exceptions, etc.), the codebase still contains legacy elements left over from the original C version. These include unused or partially-used .c files, C-style scripts, outdated documentation, and build configuration entries that may still reference C-based implementations.

# **Conversion Plan**

---

## **Phase 1: Create the Unit Tests**

### Step 1: Set up the testing environment

* Configure CMake to build unit tests (using GoogleTest already present in the project).
* Ensure `tests/` directory has its own CMakeLists.txt.
* Enable automatic test discovery.

---

### Step 2: Create unit test for `src/error.cpp`

### Step 3: Create unit test for `src/hashtable.cpp`

### Step 4: Create unit test for `src/hashtable_seed.cpp`

### Step 5: Create unit test for `src/hashtable_simple.cpp`

### Step 6: Create unit test for `src/json_dumper.cpp`

### Step 7: Create unit test for `src/json_loader.cpp`

### Step 8: Create unit test for `src/json_packer.cpp`

### Step 9: Create unit test for `src/json_types.cpp`

### Step 10: Create unit test for `src/json_unpacker.cpp`

### Step 11: Create unit test for `src/memory.cpp`

### Step 12: Create unit test for `src/string_converter.cpp`

### Step 13: Create unit test for `src/utf.cpp`

---

### Step 14: Create unit test for `include/json/json_value.hpp`

### Step 15: Create unit test for `include/json/json_factory.hpp`

### Step 16: Create unit test for `include/json/json_extensions.hpp`

### Step 17: Create unit test for `include/json/json_convenience.hpp`

### Step 18: Create unit test for `include/json/type_traits.hpp`

---

### Step 19: Create integration test for full parsing + serialization workflow

### Step 20: Create integration test for error handling

### Step 21: Create integration test for nested structure manipulation

### Step 22: Create integration test for JSON ↔ C++ type conversion

### Step 23: Create integration test for memory handling behavior

---

## **Phase 2: Create the Example Usage Folder**

### Step 1: Create `examples/` directory and its CMake setup

* Add `examples/CMakeLists.txt`
* Include examples in main CMakeLists.txt

### Step 2: Create `example.cpp` covering all major features

* JSON value creation
* Parsing from string
* Serialization
* Error handling
* Nested structures
* Modification of JSON values
* JSON arrays and objects
* Conversion to/from C++ types
* Pretty-printing and compact printing

### Step 3: Create additional focused examples

* `simple_parse.cpp`
* `json_array_example.cpp`
* `json_object_example.cpp`
* `error_handling_example.cpp`
* `type_conversion_example.cpp`

### Step 4: Document examples

* Write `examples/README.md`
* Add build/run instructions
* Add explanation for each example file

---

## **Phase 3: Fix/Remove C Remnants**

### Step 1: Remove obsolete C-related files

* Remove any remaining `.c` files
* Remove old C headers such as `jansson.h`
* Remove unused configuration headers

### Step 2: Update CMake files to remove all C references

* Remove any references to C flags or C compiler
* Replace glob patterns targeting `.c` with `.cpp`
* Ensure all executables use `.cpp`

### Step 3: Clean configuration files

* Remove C-style macro definitions
* Update config files to C++-only semantics
* Remove `extern "C"` guards where unnecessary

### Step 4: Update documentation

* Remove references to C API
* Update build instructions to C++ only
* Update all examples in docs to modern C++ style

### Step 5: Clean up build system

* Remove C-only tests or scripts
* Standardize CMake minimum version
* Remove C-specific linker flags
* Fix package config files for C++

---

## **Phase 4: Repository Validation**

### Step 1: Ensure examples compile and run correctly

### Step 2: Build and run full test suite

### Step 3: Generate code coverage report and confirm full coverage

### Step 4: Validate CMake configuration on multiple platforms

### Step 5: Final cleanup and style consistency check
