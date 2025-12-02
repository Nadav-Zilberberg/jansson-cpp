# Tests CMakeLists.txt

# Find all test files
file(GLOB TEST_SOURCES "*.cpp")

# Create test executable
add_executable(jansson_tests ${TEST_SOURCES})

# Link against the main library and GoogleTest
target_link_libraries(jansson_tests
    jansson::jansson
    GTest::gtest
    GTest::gtest_main
)

# Add include directories
target_include_directories(jansson_tests
    PRIVATE ${CMAKE_SOURCE_DIR}/include
)

# Set C++ standard
set_target_properties(jansson_tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(jansson_tests)
