cmake_minimum_required(VERSION 3.10)
project(JanssonLinkageTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Link directories for shared library
link_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Basic test executable (without GoogleTest)
add_executable(test_basic test_basic.cpp jansson_cpp.hpp)
target_link_libraries(test_basic jansson)

# Try to find GoogleTest
find_package(GTest QUIET)

if(GTest_FOUND)
    message(STATUS "GoogleTest found, building comprehensive tests")
    # GoogleTest executable
    add_executable(test_linkage test_linkage.cpp jansson_cpp.hpp)
    target_link_libraries(test_linkage jansson GTest::GTest GTest::Main)
    
    # Add tests to CTest
    enable_testing()
    add_test(NAME BasicLinkageTest COMMAND test_basic)
    add_test(NAME GoogleTestLinkage COMMAND test_linkage)
else()
    message(STATUS "GoogleTest not found, building only basic tests")
    # Add only basic test to CTest
    enable_testing()
    add_test(NAME BasicLinkageTest COMMAND test_basic)
endif()

# Custom target to run tests
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose
    DEPENDS test_basic
)

if(GTest_FOUND)
    add_dependencies(run_tests test_linkage)
endif()
