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

# Create the test executable
add_executable(test_linkage test_linkage.cpp)

# Link against the shared library
target_link_libraries(test_linkage ${JANSSON_LIB})

# Set runtime path to find the shared library
set_target_properties(test_linkage PROPERTIES
    INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}"
    BUILD_WITH_INSTALL_RPATH TRUE
)

# Add a test
enable_testing()
add_test(NAME linkage_test COMMAND test_linkage)
