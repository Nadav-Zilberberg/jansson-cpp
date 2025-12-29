cmake_minimum_required(VERSION 3.28)
project(JanssonLinkageTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GoogleTest setup
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# Find the shared library
find_library(JANSSON_LIB jansson HINTS ${CMAKE_CURRENT_SOURCE_DIR})

if(NOT JANSSON_LIB)
    message(FATAL_ERROR "Could not find libjansson.so")
endif()

# Include the header
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Find all test files
file(GLOB TEST_SOURCES "*.cpp")

# Create test executable
add_executable(jansson_linkage_tests ${TEST_SOURCES})

# Link against the shared library and GoogleTest
target_link_libraries(jansson_linkage_tests 
    ${JANSSON_LIB}
    GTest::gtest
    GTest::gtest_main
)

# Set runtime path to find the shared library
set_target_properties(jansson_linkage_tests PROPERTIES
    INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}"
    BUILD_WITH_INSTALL_RPATH TRUE
)

# Add tests
enable_testing()
include(GoogleTest)
gtest_discover_tests(jansson_linkage_tests)
