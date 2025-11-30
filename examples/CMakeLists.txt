# Examples CMakeLists.txt
# This file builds all example programs for the JSON library

cmake_minimum_required(VERSION 3.10)

# Find the JSON library
find_package(jansson REQUIRED)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Example executables
set(EXAMPLE_PROGRAMS
    example
    simple_parse
    json_array_example
    json_object_example
    error_handling_example
    type_conversion_example
    performance_example
    advanced_features_example
)

# Create each example executable
foreach(example ${EXAMPLE_PROGRAMS})
    add_executable(${example} ${example}.cpp)
    target_link_libraries(${example} PRIVATE jansson)
    target_include_directories(${example} PRIVATE ${CMAKE_SOURCE_DIR}/src)
endforeach()

# Install examples (optional)
install(TARGETS ${EXAMPLE_PROGRAMS}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/examples
)

# Create a custom target to build all examples
add_custom_target(build_examples ALL DEPENDS ${EXAMPLE_PROGRAMS})
