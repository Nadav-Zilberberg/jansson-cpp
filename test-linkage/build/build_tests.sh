#!/bin/bash

# Build script for Jansson linkage tests
# This script compiles all test files individually

set -e

echo "=== Building Jansson Linkage Tests ==="

# Find all test files
TEST_FILES=$(ls test_*.cpp 2>/dev/null || echo "test_linkage.cpp")

# Compiler settings
CXX=${CXX:-g++}
CXXFLAGS="-std=c++17 -I. -L."
LDFLAGS="-ljansson"

# Build directory
BUILD_DIR="build"
mkdir -p "$BUILD_DIR"

# Compile each test file
for test_file in $TEST_FILES; do
    test_name=$(basename "$test_file" .cpp)
    echo "Building $test_name..."
    
    # Use full path to the test file
    $CXX $CXXFLAGS -o "$BUILD_DIR/$test_name" "./$test_file" $LDFLAGS
    
    echo "✓ Built $test_name"
done

echo "=== Build Complete ==="
echo "Test executables are in the $BUILD_DIR directory"
echo ""
echo "To run all tests:"
echo "  for test in $BUILD_DIR/test_*; do ./\$test; done"
echo ""
echo "To run a specific test:"
echo "  ./$BUILD_DIR/test_name"
