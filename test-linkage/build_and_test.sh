#!/bin/bash

# Build and test script for Jansson linkage tests

set -e  # Exit on any error

echo "=== Jansson Linkage Tests Build and Run Script ==="
echo

# Clean up any previous build
if [ -d "build" ]; then
    echo "Cleaning previous build directory..."
    rm -rf build
fi

# Create build directory
echo "Creating build directory..."
mkdir build
cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake ..

# Build tests
echo "Building tests..."
make -j$(nproc)

# Run tests
echo
echo "=== Running Basic Types Test ==="
./test_basic_types

echo
echo "=== Running Arrays Test ==="
./test_arrays

echo
echo "=== Running Objects Test ==="
./test_objects

echo
echo "=== Running Comprehensive Test ==="
./test_comprehensive

echo
echo "🎉 All tests completed successfully!"
echo
echo "=== Test Summary ==="
echo "✓ Basic JSON types (null, boolean, integer, real, string)"
echo "✓ JSON arrays with mixed types and nesting"
echo "✓ JSON objects with properties and nested structures"
echo "✓ Complex structures with roundtrip serialization"
echo "✓ Error handling and memory management"
echo
echo "The Jansson C++ library linkage is working correctly!"
