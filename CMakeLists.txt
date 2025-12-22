cmake_minimum_required(VERSION 3.10)
project(jansson VERSION 2.14.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(JANSSON_BUILD_SHARED_LIBS "Build shared libraries" ON)
option(JANSSON_BUILD_STATIC_LIBS "Build static libraries" ON)
option(JANSSON_BUILD_DOCS "Build documentation" OFF)
option(JANSSON_BUILD_TESTS "Build tests" ON)
option(JANSSON_EXAMPLES "Build examples" OFF)
option(JANSSON_WITHOUT_TESTS "Build without tests" OFF)
option(USE_DTOA "Use dtoa" OFF)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/private_include)

# Configure files
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/jansson_config.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/include/jansson_config.h
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/jansson_private_config.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/private_include/jansson_private_config.h
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/jansson.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/jansson.pc
    @ONLY
)

# Add the lib sources - use C++ files instead of C files
file(GLOB JANSSON_SRC src/*.cpp)
if (NOT USE_DTOA)
   list(FILTER JANSSON_SRC EXCLUDE REGEX ".*dtoa\\.cpp$")
endif()

# Private headers (C++)
set(JANSSON_HDR_PRIVATE
   ${CMAKE_CURRENT_SOURCE_DIR}/src/hashtable.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/memory.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/strbuffer.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/utf.hpp
   ${CMAKE_CURRENT_BINARY_DIR}/private_include/jansson_private_config.h)

# Public headers (C interface)
set(JANSSON_HDR_PUBLIC
   ${CMAKE_CURRENT_BINARY_DIR}/include/jansson_config.h
   ${CMAKE_CURRENT_SOURCE_DIR}/src/jansson.h)

# Create a .def file for Windows DLL exports
if(WIN32)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/jansson.def "LIBRARY jansson\nEXPORTS\n")
    # Add function exports here based on jansson.h
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/jansson.def 
"json_array
json_array_append
json_array_append_new
json_array_clear
json_array_extend
json_array_get
json_array_insert
json_array_insert_new
json_array_remove
json_array_set
json_array_set_new
json_array_size
json_copy
json_deep_copy
json_delete
json_dumpb
json_dumpf
json_dumpfd
json_dumps
json_dump_file
json_equal
json_error_code
json_false
json_false()
json_integer
json_integer_set
json_integer_value
json_is_array
json_is_false
json_is_integer
json_is_null
json_is_number
json_is_object
json_is_real
json_is_string
json_is_true
json_loadb
json_loadf
json_load_file
json_loads
json_null
json_object
json_object_clear
json_object_del
json_object_get
json_object_iter
json_object_iter_at
json_object_iter_key
json_object_iter_next
json_object_iter_set
json_object_iter_set_new
json_object_iter_value
json_object_key_to_iter
json_object_seed
json_object_set
json_object_set_new
json_object_size
json_object_update
json_object_update_existing
json_object_update_missing
json_pack
json_pack_ex
json_real
json_real_set
json_real_value
json_set_alloc_funcs
json_sprintf
json_string
json_stringn
json_stringn_nocheck
json_string_set
json_string_setn
json_string_setn_nocheck
json_string_value
json_string_length
json_true
json_type
json_unpack
json_unpack_ex
json_vpack_ex
json_vunpack_ex
json_vsprintf
")
endif()

source_group("Library Sources" FILES ${JANSSON_SRC})
source_group("Library Private Headers" FILES ${JANSSON_HDR_PRIVATE})
source_group("Library Public Headers" FILES ${JANSSON_HDR_PUBLIC})

# Build shared library (DLL)
if(JANSSON_BUILD_SHARED_LIBS)
    add_library(jansson SHARED
        ${JANSSON_SRC}
        ${JANSSON_HDR_PRIVATE}
        ${JANSSON_HDR_PUBLIC}
    )
    
    if(WIN32)
        target_sources(jansson PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/jansson.def)
    endif()
    
    # Set properties for DLL
    set_target_properties(jansson PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        OUTPUT_NAME jansson
        DEFINE_SYMBOL DLL_EXPORT
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
    
    # Include directories for the target
    target_include_directories(jansson
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_BINARY_DIR}/private_include
    )
    
    # Link libraries
    target_link_libraries(jansson PRIVATE Threads::Threads)
endif()

# Build static library
if(JANSSON_BUILD_STATIC_LIBS)
    add_library(jansson_static STATIC
        ${JANSSON_SRC}
        ${JANSSON_HDR_PRIVATE}
        ${JANSSON_HDR_PUBLIC}
    )
    
    set_target_properties(jansson_static PROPERTIES
        VERSION ${PROJECT_VERSION}
        OUTPUT_NAME jansson
        POSITION_INDEPENDENT_CODE ON
    )
    
    target_include_directories(jansson_static
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_BINARY_DIR}/private_include
    )
    
    target_link_libraries(jansson_static PRIVATE Threads::Threads)
endif()

# Find threads
find_package(Threads REQUIRED)

# Install targets
include(GNUInstallDirs)

if(JANSSON_BUILD_SHARED_LIBS)
    install(TARGETS jansson
        EXPORT jansson-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

if(JANSSON_BUILD_STATIC_LIBS)
    install(TARGETS jansson_static
        EXPORT jansson-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

# Install headers
install(FILES ${JANSSON_HDR_PUBLIC}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install pkg-config file
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/jansson.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

# Export targets
install(EXPORT jansson-targets
    FILE jansson-targets.cmake
    NAMESPACE jansson::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jansson
)

# Create config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/jansson-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/jansson-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/jansson-config.cmake
    @ONLY
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/jansson-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/jansson-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jansson
)

# Build tests
if(JANSSON_BUILD_TESTS AND NOT JANSSON_WITHOUT_TESTS)
    enable_testing()
    
    # GoogleTest
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    
    FetchContent_MakeAvailable(googletest)
    
    add_subdirectory(tests)
endif()

# Build examples
if(JANSSON_EXAMPLES)
    add_subdirectory(examples)
endif()
