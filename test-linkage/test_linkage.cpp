#include <iostream>
#include <cassert>
#include <cstring>
#include <vector>
#include <jansson.h>

// Simple test framework
#define ASSERT_TRUE(expr) do { if (!(expr)) { std::cerr << "FAIL: " << #expr << " at " << __FILE__ << ":" << __LINE__ << std::endl; return false; } } while(0)
#define ASSERT_FALSE(expr) do { if (expr) { std::cerr << "FAIL: " << #expr << " at " << __FILE__ << ":" << __LINE__ << std::endl; return false; } } while(0)
#define ASSERT_EQ(a, b) do { if ((a) != (b)) { std::cerr << "FAIL: " << #a << " != " << #b << " at " << __FILE__ << ":" << __LINE__ << std::endl; return false; } } while(0)
#define ASSERT_NULL(ptr) ASSERT_TRUE((ptr) == nullptr)
#define ASSERT_NOT_NULL(ptr) ASSERT_TRUE((ptr) != nullptr)

struct TestCase {
    const char* name;
    bool (*func)();
};

std::vector<TestCase> tests;

// Forward declarations
void register_tests();

// Test basic JSON value creation
bool test_basic_json_creation() {
    json_t* null_val = json_null();
    ASSERT_NOT_NULL(null_val);
    ASSERT_TRUE(json_is_null(null_val));
    json_decref(null_val);
    
    json_t* true_val = json_true();
    ASSERT_NOT_NULL(true_val);
    ASSERT_TRUE(json_is_true(true_val));
    json_decref(true_val);
    
    json_t* false_val = json_false();
    ASSERT_NOT_NULL(false_val);
    ASSERT_TRUE(json_is_false(false_val));
    json_decref(false_val);
    
    json_t* int_val = json_integer(42);
    ASSERT_NOT_NULL(int_val);
    ASSERT_TRUE(json_is_integer(int_val));
    ASSERT_EQ(json_integer_value(int_val), 42);
    json_decref(int_val);
    
    json_t* real_val = json_real(3.14);
    ASSERT_NOT_NULL(real_val);
    ASSERT_TRUE(json_is_real(real_val));
    ASSERT_EQ(json_real_value(real_val), 3.14);
    json_decref(real_val);
    
    json_t* str_val = json_string("test");
    ASSERT_NOT_NULL(str_val);
    ASSERT_TRUE(json_is_string(str_val));
    ASSERT_EQ(strcmp(json_string_value(str_val), "test"), 0);
    json_decref(str_val);
    
    return true;
}

// Test JSON array operations
bool test_json_array_operations() {
    json_t* arr = json_array();
    ASSERT_NOT_NULL(arr);
    ASSERT_TRUE(json_is_array(arr));
    ASSERT_EQ(json_array_size(arr), 0);
    
    json_t* int_val = json_integer(42);
    json_t* str_val = json_string("test");
    
    int result = json_array_append(arr, int_val);
    ASSERT_EQ(result, 0);
    ASSERT_EQ(json_array_size(arr), 1);
    
    result = json_array_append(arr, str_val);
    ASSERT_EQ(result, 0);
    ASSERT_EQ(json_array_size(arr), 2);
    
    json_t* retrieved = json_array_get(arr, 0);
    ASSERT_NOT_NULL(retrieved);
    ASSERT_TRUE(json_is_integer(retrieved));
    ASSERT_EQ(json_integer_value(retrieved), 42);
    
    retrieved = json_array_get(arr, 1);
    ASSERT_NOT_NULL(retrieved);
    ASSERT_TRUE(json_is_string(retrieved));
    ASSERT_EQ(strcmp(json_string_value(retrieved), "test"), 0);
    
    json_decref(arr);  // This also decrements references to contained elements
    json_decref(int_val);
    json_decref(str_val);
    
    return true;
}

// Test JSON object operations
bool test_json_object_operations() {
    json_t* obj = json_object();
    ASSERT_NOT_NULL(obj);
    ASSERT_TRUE(json_is_object(obj));
    ASSERT_EQ(json_object_size(obj), 0);
    
    json_t* int_val = json_integer(123);
    int result = json_object_set(obj, "number", int_val);
    ASSERT_EQ(result, 0);
    ASSERT_EQ(json_object_size(obj), 1);
    
    json_t* str_val = json_string("hello");
    result = json_object_set(obj, "message", str_val);
    ASSERT_EQ(result, 0);
    ASSERT_EQ(json_object_size(obj), 2);
    
    json_t* retrieved = json_object_get(obj, "number");
    ASSERT_NOT_NULL(retrieved);
    ASSERT_TRUE(json_is_integer(retrieved));
    ASSERT_EQ(json_integer_value(retrieved), 123);
    
    retrieved = json_object_get(obj, "message");
    ASSERT_NOT_NULL(retrieved);
    ASSERT_TRUE(json_is_string(retrieved));
    ASSERT_EQ(strcmp(json_string_value(retrieved), "hello"), 0);
    
    json_decref(obj);
    json_decref(int_val);
    json_decref(str_val);
    
    return true;
}

// Test JSON serialization and deserialization
bool test_json_serialization() {
    // Create a complex JSON structure
    json_t* root = json_object();
    json_t* arr = json_array();
    json_t* str = json_string("test");
    json_t* num = json_integer(42);
    
    json_array_append(arr, str);
    json_array_append(arr, num);
    json_object_set(root, "data", arr);
    json_object_set(root, "name", json_string("example"));
    
    // Serialize to string
    char* json_str = json_dumps(root, 0);  // Use 0 instead of JSON_COMPACT
    ASSERT_NOT_NULL(json_str);
    ASSERT_TRUE(strlen(json_str) > 0);
    
    std::cout << "Serialized JSON: " << json_str << std::endl;
    
    // Deserialize back
    json_error_t error;
    json_t* parsed = json_loads(json_str, 0, &error);
    ASSERT_NOT_NULL(parsed);
    ASSERT_TRUE(json_is_object(parsed));
    
    json_t* name = json_object_get(parsed, "name");
    ASSERT_NOT_NULL(name);
    ASSERT_TRUE(json_is_string(name));
    ASSERT_EQ(strcmp(json_string_value(name), "example"), 0);
    
    json_t* data = json_object_get(parsed, "data");
    ASSERT_NOT_NULL(data);
    ASSERT_TRUE(json_is_array(data));
    ASSERT_EQ(json_array_size(data), 2);
    
    free(json_str);
    json_decref(root);
    json_decref(parsed);
    
    return true;
}

// Test JSON equality
bool test_json_equality() {
    json_t* obj1 = json_object();
    json_t* obj2 = json_object();
    
    json_object_set(obj1, "key", json_string("value"));
    json_object_set(obj2, "key", json_string("value"));
    
    ASSERT_TRUE(json_equal(obj1, obj2));
    
    json_object_set(obj2, "extra", json_integer(123));
    ASSERT_FALSE(json_equal(obj1, obj2));
    
    json_decref(obj1);
    json_decref(obj2);
    
    return true;
}

void register_tests() {
    tests.push_back({"basic_json_creation", test_basic_json_creation});
    tests.push_back({"json_array_operations", test_json_array_operations});
    tests.push_back({"json_object_operations", test_json_object_operations});
    tests.push_back({"json_serialization", test_json_serialization});
    tests.push_back({"json_equality", test_json_equality});
}

int main() {
    std::cout << "Running Jansson Linkage Tests..." << std::endl;
    std::cout << "=================================" << std::endl;
    
    register_tests();
    
    int passed = 0;
    int failed = 0;
    
    for (const auto& test : tests) {
        std::cout << "Running test: " << test.name << "... ";
        if (test.func()) {
            std::cout << "PASSED" << std::endl;
            passed++;
        } else {
            std::cout << "FAILED" << std::endl;
            failed++;
        }
    }
    
    std::cout << "=================================" << std::endl;
    std::cout << "Test Results: " << passed << " passed, " << failed << " failed" << std::endl;
    
    return failed > 0 ? 1 : 0;
}
