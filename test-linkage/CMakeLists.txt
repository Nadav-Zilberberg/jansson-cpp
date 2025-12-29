cmake_minimum_required(VERSION 3.10)
project(jansson_test_linkage VERSION 2.14.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Add debug flags for Debug build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Find the main jansson library
find_library(JANSSON_LIBRARY 
    NAMES jansson
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../src
    NO_DEFAULT_PATH
)

if(NOT JANSSON_LIBRARY)
    # If not found, try to build it from source
    message(STATUS "Building jansson library from source...")
    
    # Add the source directory
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../src jansson_build)
    set(JANSSON_LIBRARY jansson)
endif()

# Create shared library with both C and C++ interfaces
add_library(jansson_test_linkage SHARED
    jansson_cpp.cpp
)

# Link with the main jansson library
target_link_libraries(jansson_test_linkage ${JANSSON_LIBRARY})

# Set properties for the shared library
set_target_properties(jansson_test_linkage PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 2
    PUBLIC_HEADER jansson.h
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)

# Export all symbols for testing purposes
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(jansson_test_linkage PRIVATE -fvisibility=default)
endif()

# Create test executable
add_executable(test_linkage
    test_linkage.cpp
)

# Link test with our shared library
target_link_libraries(test_linkage jansson_test_linkage)

# Set runtime path for the test executable
if(UNIX AND NOT APPLE)
    set_target_properties(test_linkage PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
elseif(APPLE)
    set_target_properties(test_linkage PROPERTIES
        INSTALL_RPATH "@loader_path"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
endif()

# Add tests
enable_testing()
add_test(NAME test_linkage COMMAND test_linkage)

# Custom target to run tests
add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS test_linkage
)

# Installation rules
install(TARGETS jansson_test_linkage test_linkage
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include
)

# Print configuration summary
message(STATUS "Configuration:")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "  C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "  C flags: ${CMAKE_C_FLAGS}")
message(STATUS "  C++ flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "  Jansson library: ${JANSSON_LIBRARY}")
