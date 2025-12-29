cmake_minimum_required(VERSION 3.14)
project(test_linkage)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include FetchContent for GoogleTest
include(FetchContent)

# Download GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Link directories for the shared library
link_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Define test executables
set(TEST_SOURCES
    test_string_conversion.cpp
    test_json_builder.cpp
    test_literal_operators.cpp
    test_complex_structures.cpp
    test_exceptions.cpp
)

# Create test executables
foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source})
    target_link_libraries(${test_name} jansson gtest gtest_main)
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()

# Also build the original test for compatibility
add_executable(test_linkage_original test_linkage.cpp)
target_link_libraries(test_linkage_original jansson)
