#!/bin/bash

# Install dependencies
apt-get update
apt-get install -y cmake g++ make

# Create build directory
mkdir -p build
cd build

# Configure with tests and examples enabled
cmake .. \
    -DJANSSON_WITHOUT_TESTS=OFF \
    -DJANSSON_BUILD_SHARED_LIBS=ON \
    -DJANSSON_EXAMPLES=ON \
    -DCMAKE_BUILD_TYPE=Debug

# Build
make -j$(nproc)

# Run tests
make test

# Alternative test command using CTest
ctest --verbose
