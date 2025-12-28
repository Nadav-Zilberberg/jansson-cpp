#!/bin/bash

# Script to set up GoogleTest integration
# This script can be run when GoogleTest becomes available

set -e

echo "=== Setting up GoogleTest Integration ==="

# Check if GoogleTest is available
if ! pkg-config --exists gtest; then
    echo "GoogleTest not found via pkg-config"
    echo "Checking for manual installation..."
    
    # Check common locations
    GTEST_LOCATIONS=(
        "/usr/include/gtest"
        "/usr/local/include/gtest"
        "/opt/gtest/include/gtest"
    )
    
    gtest_found=false
    for location in "${GTEST_LOCATIONS[@]}"; do
        if [ -d "$location" ]; then
            echo "Found GoogleTest at: $location"
            gtest_found=true
            break
        fi
    done
    
    if [ "$gtest_found" = false ]; then
        echo "ERROR: GoogleTest not found. Please install GoogleTest first."
        echo ""
        echo "To install GoogleTest:"
        echo "  Ubuntu/Debian: sudo apt-get install libgtest-dev"
        echo "  CentOS/RHEL:   sudo yum install gtest-devel"
        echo "  From source:   See https://github.com/google/googletest"
        exit 1
    fi
fi

# Update CMakeLists.txt to use GoogleTest
echo "Updating CMakeLists.txt for GoogleTest..."

cat > CMakeLists.txt << 'GTEST_CMAKE'
cmake_minimum_required(VERSION 3.10)
project(JanssonLinkageTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the shared library
find_library(JANSSON_LIB jansson HINTS ${CMAKE_CURRENT_SOURCE_DIR})

if(NOT JANSSON_LIB)
    message(FATAL_ERROR "Could not find libjansson.so")
endif()

# Include the header
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Find GoogleTest
find_package(GTest REQUIRED)

# Find all test files
file(GLOB TEST_SOURCES "test_*.cpp")

# Create a single test executable with all tests
add_executable(jansson_tests ${TEST_SOURCES})

# Link against the shared library and GoogleTest
target_link_libraries(jansson_tests ${JANSSON_LIB} GTest::GTest GTest::Main)

# Set runtime path to find the shared library
set_target_properties(jansson_tests PROPERTIES
    INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}"
    BUILD_WITH_INSTALL_RPATH TRUE
)

# Add test
add_test(NAME jansson_tests COMMAND jansson_tests)

# Enable CTest for better test discovery
enable_testing()
GTEST_CMAKE

echo "✓ CMakeLists.txt updated for GoogleTest"
echo ""
echo "To build with GoogleTest:"
echo "  mkdir -p build && cd build"
echo "  cmake .. && make"
echo "  ./jansson_tests"
