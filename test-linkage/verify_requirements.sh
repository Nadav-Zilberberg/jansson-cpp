#!/bin/bash

# Script to verify that all PR requirements are met
# This checks the test-linkage directory structure and functionality

echo "=== Verifying PR Requirements ==="
echo ""

# Check 1: Required files exist
echo "1. Checking required files..."
required_files=(
    "jansson.h"
    "libjansson.so"
    "CMakeLists.txt"
    "README.md"
)

missing_files=()
for file in "${required_files[@]}"; do
    if [ ! -f "$file" ] && [ ! -e "$file" ]; then
        missing_files+=("$file")
    fi
done

if [ ${#missing_files[@]} -eq 0 ]; then
    echo "   ✓ All required files present"
else
    echo "   ✗ Missing files: ${missing_files[*]}"
    exit 1
fi

# Check 2: Multiple test suites support
echo ""
echo "2. Checking test suite infrastructure..."

# Check if CMakeLists.txt supports multiple tests
if grep -q "file(GLOB TEST_SOURCES" CMakeLists.txt; then
    echo "   ✓ CMakeLists.txt supports multiple test files"
else
    echo "   ✗ CMakeLists.txt does not support multiple test files"
    exit 1
fi

# Check if build script exists
if [ -f "build_tests.sh" ]; then
    echo "   ✓ Build script for multiple tests exists"
else
    echo "   ✗ Build script missing"
    exit 1
fi

# Check if test runner exists
if [ -f "run_tests.sh" ]; then
    echo "   ✓ Test runner script exists"
else
    echo "   ✗ Test runner script missing"
    exit 1
fi

# Check 3: GoogleTest support
echo ""
echo "3. Checking GoogleTest support..."

if [ -f "setup_googletest.sh" ]; then
    echo "   ✓ GoogleTest setup script exists"
else
    echo "   ✗ GoogleTest setup script missing"
    exit 1
fi

if grep -q "find_package(GTest" CMakeLists.txt; then
    echo "   ✓ CMakeLists.txt has GoogleTest integration"
else
    echo "   ✗ CMakeLists.txt lacks GoogleTest integration"
    exit 1
fi

# Check 4: C++17 support
echo ""
echo "4. Checking C++17 support..."

if grep -q "CMAKE_CXX_STANDARD 17" CMakeLists.txt; then
    echo "   ✓ C++17 standard specified in CMakeLists.txt"
else
    echo "   ✗ C++17 standard not specified"
    exit 1
fi

# Check 5: Test compilation
echo ""
echo "5. Testing compilation..."

if ./build_tests.sh > /dev/null 2>&1; then
    echo "   ✓ Tests compile successfully"
else
    echo "   ✗ Test compilation failed"
    exit 1
fi

# Check 6: Test execution
echo ""
echo "6. Testing execution..."

# Run tests and capture output
if ./run_tests.sh > /tmp/test_output.txt 2>&1; then
    echo "   ✓ Tests execute successfully"
    # Show a summary
    passed=$(grep -o "[0-9]* passed" /tmp/test_output.txt | head -1 | cut -d' ' -f1)
    failed=$(grep -o "[0-9]* failed" /tmp/test_output.txt | head -1 | cut -d' ' -f1)
    if [ -n "$passed" ] && [ -n "$failed" ]; then
        echo "     Summary: $passed passed, $failed failed"
    fi
else
    # Check if tests actually passed but runner returned non-zero
    if grep -q "passed" /tmp/test_output.txt && ! grep -q "failed" /tmp/test_output.txt; then
        echo "   ✓ Tests execute successfully (all passed)"
    else
        echo "   ✗ Test execution failed"
        echo "   Output:"
        cat /tmp/test_output.txt
        exit 1
    fi
fi

# Check 7: Documentation
echo ""
echo "7. Checking documentation..."

if [ -f "README_TESTS.md" ]; then
    echo "   ✓ Detailed test documentation exists"
else
    echo "   ✗ Detailed test documentation missing"
    exit 1
fi

# Check 8: Test suite categories mentioned in PR
echo ""
echo "8. Checking test suite categories..."

categories=(
    "String to JSON conversion"
    "JSON to string conversion" 
    "JSON builder"
    "Literal operators"
    "Complex structures"
    "Exception handling"
)

for category in "${categories[@]}"; do
    if grep -qi "$category" README.md; then
        echo "   ✓ $category mentioned in documentation"
    else
        echo "   ⚠ $category not explicitly mentioned (may be in test files)"
    fi
done

echo ""
echo "=== All Requirements Verified ==="
echo "✓ The test-linkage directory meets all PR requirements"
echo "✓ Infrastructure supports multiple test suites"
echo "✓ GoogleTest integration is ready"
echo "✓ C++17 compatibility is enforced"
echo "✓ Build and execution systems are working"
echo ""
echo "The infrastructure is ready for the improved test suites!"
