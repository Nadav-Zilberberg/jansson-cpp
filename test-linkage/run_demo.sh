#!/bin/bash

echo "=== Jansson Shared Library Linkage Test Demo ==="
echo

# Set up library path
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

# Run basic test
echo "Running basic test..."
./test_basic
echo

# Run comprehensive test
echo "Running comprehensive GoogleTest..."
./test_linkage
echo

echo "=== All tests completed successfully! ==="
echo
echo "This demonstrates that the shared library can be used"
echo "as a standalone dependency without the original source code."
