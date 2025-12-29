cmake_minimum_required(VERSION 3.10)
project(jansson_static_linkage_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find GoogleTest - try different approaches
find_package(GTest QUIET)

if(NOT GTest_FOUND)
    # Fallback to manual configuration
    set(GTEST_INCLUDE_DIRS /usr/src/gtest/include)
    set(GTEST_LIBRARIES /usr/src/gtest/lib/libgtest.a)
    set(GTEST_MAIN_LIBRARIES /usr/src/gtest/lib/libgtest_main.a)
    set(GTEST_BOTH_LIBRARIES ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
if(GTest_FOUND)
    include_directories(${GTEST_INCLUDE_DIRS})
endif()

# Static library path
set(JANSSON_STATIC_LIB ${CMAKE_CURRENT_SOURCE_DIR}/libjansson.a)

# Test executables
set(TEST_SOURCES
    test_string_conversion.cpp
    test_json_to_string.cpp
    test_json_builder.cpp
    test_literal_operators.cpp
    test_complex_structures.cpp
    test_exceptions.cpp
)

# Create test executables
foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source})
    
    if(GTest_FOUND)
        target_link_libraries(${test_name} ${JANSSON_STATIC_LIB} ${GTEST_BOTH_LIBRARIES} pthread)
    else()
        target_link_libraries(${test_name} ${JANSSON_STATIC_LIB} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
    endif()
    
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()

# Add a custom target to run all tests
add_custom_target(run_static_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose
)
