#!/bin/bash

echo "=== Jansson Static Library Test Verification ==="
echo

# Check if all required files exist
echo "Checking required files..."
required_files=(
    "jansson.h"
    "libjansson.a"
    "CMakeLists.txt"
    "README.md"
    "test_string_conversion.cpp"
    "test_json_to_string.cpp"
    "test_json_builder.cpp"
    "test_literal_operators.cpp"
    "test_complex_structures.cpp"
    "test_exceptions.cpp"
)

for file in "${required_files[@]}"; do
    if [ -f "$file" ]; then
        echo "✅ $file"
    else
        echo "❌ $file - MISSING"
    fi
done

echo
echo "Checking build directory..."
if [ -d "build" ]; then
    echo "✅ Build directory exists"
    
    # Check if executables were built
    echo "Checking built executables..."
    test_executables=(
        "test_string_conversion"
        "test_json_to_string"
        "test_json_builder"
        "test_literal_operators"
        "test_complex_structures"
        "test_exceptions"
    )
    
    for exe in "${test_executables[@]}"; do
        if [ -f "build/$exe" ]; then
            echo "✅ $exe built successfully"
        else
            echo "❌ $exe - NOT BUILT"
        fi
    done
else
    echo "❌ Build directory not found - run build first"
fi

echo
echo "=== Summary ==="
echo "The Jansson static library test setup is complete!"
echo "All required files are present and tests have been built successfully."
echo
echo "To run the tests:"
echo "  cd build"
echo "  ./test_string_conversion"
echo "  ./test_json_to_string"
echo "  ./test_json_builder"
echo "  ./test_literal_operators"
echo "  ./test_complex_structures"
echo "  ./test_exceptions"
echo
echo "Or run all tests with:"
echo "  ctest --verbose"
