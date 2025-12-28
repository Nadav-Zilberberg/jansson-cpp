#!/bin/bash

echo "Building and running Jansson C Linkage Tests..."
echo "=============================================="

# Compile the test
echo "Compiling test_linkage.c..."
gcc -std=c99 -I. -L. test_linkage.c -ljansson -o test_linkage_c

if [ $? -ne 0 ]; then
    echo "ERROR: Compilation failed!"
    exit 1
fi

# Set library path and run the test
echo "Running tests..."
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
./test_linkage_c

# Capture the result
TEST_RESULT=$?

# Clean up
rm -f test_linkage_c

if [ $TEST_RESULT -eq 0 ]; then
    echo ""
    echo "SUCCESS: All tests passed!"
    exit 0
else
    echo ""
    echo "FAILURE: Some tests failed!"
    exit 1
fi
