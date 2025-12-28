#!/bin/bash

# Test runner for Jansson linkage tests
# This script runs all tests and provides a summary

echo "=== Jansson Linkage Test Suite ==="
echo ""

# Find all test executables
BUILD_DIR="build"
TEST_EXECUTABLES=$(find "$BUILD_DIR" -name "test_*" -type f -executable 2>/dev/null || echo "")

if [ -z "$TEST_EXECUTABLES" ]; then
    echo "ERROR: No test executables found in $BUILD_DIR"
    echo "Please run ./build_tests.sh first to build the tests."
    exit 1
fi

# Run each test and collect results
total_tests=0
passed_tests=0
failed_tests=0

for test_exe in $TEST_EXECUTABLES; do
    test_name=$(basename "$test_exe")
    echo "Running $test_name..."
    
    if $test_exe; then
        passed_tests=$((passed_tests + 1))
        echo "✓ $test_name PASSED"
    else
        failed_tests=$((failed_tests + 1))
        echo "✗ $test_name FAILED"
    fi
    
    total_tests=$((total_tests + 1))
    echo ""
done

# Summary
echo "=== Test Summary ==="
echo "Total tests: $total_tests"
echo "Passed: $passed_tests"
echo "Failed: $failed_tests"

if [ $failed_tests -eq 0 ]; then
    echo "All tests passed! ✓"
    exit 0
else
    echo "Some tests failed! ✗"
    exit 1
fi
