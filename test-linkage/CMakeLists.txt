cmake_minimum_required(VERSION 3.10)
project(JanssonLinkageTests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Function to create a test executable
function(create_test test_name)
    add_executable(${test_name} ${test_name}.cpp jansson-cpp.cpp)
    
    # No need to link against jansson library since we're using pure C++ implementation
endfunction()

# Create test executables
create_test(test_basic_types)
create_test(test_arrays)
create_test(test_objects)
create_test(test_comprehensive)

# Add custom target to run all tests
add_custom_target(run_tests
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_basic_types
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_arrays
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_objects
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_comprehensive
    DEPENDS test_basic_types test_arrays test_objects test_comprehensive
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running all linkage tests"
)

# Add custom target to clean up
add_custom_target(clean_tests
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Cleaning test build artifacts"
)
