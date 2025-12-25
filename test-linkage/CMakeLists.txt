cmake_minimum_required(VERSION 3.10)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the shared library
find_library(JANSSON_LIB jansson HINTS ${CMAKE_CURRENT_SOURCE_DIR})

if(NOT JANSSON_LIB)
    message(FATAL_ERROR "Could not find libjansson.so")
endif()

# Include the header
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Create the test executable
add_executable(test_linkage test_linkage.cpp)

# Link against the shared library
target_link_libraries(test_linkage ${JANSSON_LIB})

# Set runtime path to find the shared library
set_target_properties(test_linkage PROPERTIES
    INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}"
    BUILD_WITH_INSTALL_RPATH TRUE
)

# If we're being built as part of the main project with gtest available
if(TARGET gtest)
    message(STATUS "Integrating test-linkage with main project's gtest framework")
    
    # Link with gtest to enable future gtest conversion
    target_link_libraries(test_linkage gtest gtest_main)
    
    # Add test to the main test suite
    add_test(NAME test_linkage COMMAND test_linkage)
    
    # Set test properties for gtest integration
    set_tests_properties(test_linkage PROPERTIES
        ENVIRONMENT "GTEST_OUTPUT=xml:test_linkage_results.xml"
        TIMEOUT 30
    )
    
    # Create individual test targets for each test category
    # These will be useful when the tests are converted to gtest
    add_custom_target(test_linkage_basic
        COMMAND test_linkage --filter="basic_json_creation"
        DEPENDS test_linkage
        COMMENT "Running basic JSON creation tests"
    )
    
    add_custom_target(test_linkage_arrays
        COMMAND test_linkage --filter="json_array_operations"
        DEPENDS test_linkage
        COMMENT "Running JSON array operation tests"
    )
    
    add_custom_target(test_linkage_objects
        COMMAND test_linkage --filter="json_object_operations"
        DEPENDS test_linkage
        COMMENT "Running JSON object operation tests"
    )
    
    add_custom_target(test_linkage_serialization
        COMMAND test_linkage --filter="json_serialization"
        DEPENDS test_linkage
        COMMENT "Running JSON serialization tests"
    )
    
    add_custom_target(test_linkage_equality
        COMMAND test_linkage --filter="json_equality"
        DEPENDS test_linkage
        COMMENT "Running JSON equality tests"
    )
    
else()
    message(STATUS "Building test-linkage standalone without gtest integration")
    
    # Enable basic testing
    enable_testing()
    add_test(NAME test_linkage COMMAND test_linkage)
endif()

# Create a custom target to run all tests
add_custom_target(run_test_linkage
    COMMAND ${CMAKE_CTEST_COMMAND} -R test_linkage -V
    DEPENDS test_linkage
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running all test-linkage tests"
)

# Export the test executable for use by parent projects
set(TEST_LINKAGE_EXECUTABLE test_linkage PARENT_SCOPE)
set(TEST_LINKAGE_DIR ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
