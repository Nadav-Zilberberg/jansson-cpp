cmake_minimum_required(VERSION 3.10)
project(JanssonCLinkageTests C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find the shared library
find_library(JANSSON_LIBRARY
    NAMES jansson
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}
    NO_DEFAULT_PATH
)

if(NOT JANSSON_LIBRARY)
    message(FATAL_ERROR "Could not find libjansson.so")
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Test executables
add_executable(test_basic test_basic.c)
add_executable(test_advanced test_advanced.c)
add_executable(test_error_handling test_error_handling.c)

# Link libraries
target_link_libraries(test_basic ${JANSSON_LIBRARY})
target_link_libraries(test_advanced ${JANSSON_LIBRARY})
target_link_libraries(test_error_handling ${JANSSON_LIBRARY})

# Add tests
enable_testing()
add_test(NAME test_basic COMMAND test_basic)
add_test(NAME test_advanced COMMAND test_advanced)
add_test(NAME test_error_handling COMMAND test_error_handling)

# Custom target to run all tests
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose
    DEPENDS test_basic test_advanced test_error_handling
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running all C linkage tests"
)
