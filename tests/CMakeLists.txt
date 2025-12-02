# Tests CMakeLists.txt
cmake_minimum_required(VERSION 3.28)
# GoogleTest setup
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# Find all test files
file(GLOB TEST_SOURCES "*.cpp")

# Create test executable
add_executable(jansson_tests ${TEST_SOURCES})

# Link against the main library and GoogleTest
target_link_libraries(jansson_tests PRIVATE --coverage
    GTest::gtest
    GTest::gtest_main
)

# Add include directories
target_include_directories(jansson_tests
    PRIVATE ${CMAKE_SOURCE_DIR}/include
)

# Set C++ standard
set_target_properties(jansson_tests PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(jansson_tests)

target_compile_options(jansson_tests PRIVATE --coverage -O0 -g)