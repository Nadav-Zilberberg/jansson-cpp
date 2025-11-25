cmake_minimum_required(VERSION 3.10)
project(jansson-cpp VERSION 2.14.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(JANSSON_WITHOUT_TESTS "Build without tests" OFF)
option(JANSSON_BUILD_SHARED_LIBS "Build shared libraries" ON)
option(JANSSON_EXAMPLES "Build examples" ON)
option(JANSSON_INSTALL "Generate install target" ON)

# Source files
set(JANSSON_SOURCES
    jansson-cpp/src/dump.cpp
    jansson-cpp/src/error.cpp
    jansson-cpp/src/hashtable.cpp
    jansson-cpp/src/hashtable_seed.cpp
    jansson-cpp/src/load.cpp
    jansson-cpp/src/memory.cpp
    jansson-cpp/src/pack_unpack.cpp
    jansson-cpp/src/strbuffer.cpp
    jansson-cpp/src/strconv.cpp
    jansson-cpp/src/utf.cpp
    jansson-cpp/src/value.cpp
    jansson-cpp/src/version.cpp
)

# Create library
if(JANSSON_BUILD_SHARED_LIBS)
    add_library(jansson SHARED ${JANSSON_SOURCES})
else()
    add_library(jansson STATIC ${JANSSON_SOURCES})
endif()

# Include directories
target_include_directories(jansson
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/jansson-cpp/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Compile definitions
target_compile_definitions(jansson
    PRIVATE
        JANSSON_BUILDING_DLL
)

# Examples
if(JANSSON_EXAMPLES)
    add_subdirectory(examples)
endif()

# Tests
if(NOT JANSSON_WITHOUT_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

# Install
if(JANSSON_INSTALL)
    install(TARGETS jansson
        EXPORT JanssonTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
    )

    install(DIRECTORY jansson-cpp/include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp"
    )

    install(FILES
        jansson.h
        DESTINATION include
    )
endif()
