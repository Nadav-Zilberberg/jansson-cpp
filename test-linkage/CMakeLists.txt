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

# Option 1: Simple approach for multiple test files without GoogleTest
# (This maintains compatibility while allowing multiple test files)

# Find all test files
file(GLOB TEST_SOURCES "test_*.cpp")

# Create individual test executables for each test file
foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source})
    
    # Link against the shared library
    target_link_libraries(${test_name} ${JANSSON_LIB})
    
    # Set runtime path to find the shared library
    set_target_properties(${test_name} PROPERTIES
        INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
    
    # Add individual test
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()

# Option 2: GoogleTest integration (commented out by default)
# Uncomment and modify if GoogleTest is available

# find_package(GTest QUIET)
# if(GTest_FOUND)
#     # Create a single test executable with all tests
#     add_executable(jansson_tests ${TEST_SOURCES})
#     target_link_libraries(jansson_tests ${JANSSON_LIB} GTest::GTest GTest::Main)
#     
#     set_target_properties(jansson_tests PROPERTIES
#         INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}"
#         BUILD_WITH_INSTALL_RPATH TRUE
#     )
#     
#     # Add test
#     add_test(NAME jansson_tests COMMAND jansson_tests)
#     
#     # Enable CTest for better test discovery
#     enable_testing()
# else()
#     message(STATUS "GoogleTest not found, using simple test approach")
# endif()

# Enable testing
enable_testing()
