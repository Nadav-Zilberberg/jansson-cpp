#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <jansson.h>

// Simple test framework
#define ASSERT_TRUE(expr) do { if (!(expr)) { fprintf(stderr, "FAIL: " #expr " at %s:%d\n", __FILE__, __LINE__); return 0; } } while(0)
#define ASSERT_FALSE(expr) do { if (expr) { fprintf(stderr, "FAIL: " #expr " at %s:%d\n", __FILE__, __LINE__); return 0; } } while(0)
#define ASSERT_EQ(a, b) do { if ((a) != (b)) { fprintf(stderr, "FAIL: " #a " != " #b " at %s:%d\n", __FILE__, __LINE__); return 0; } } while(0)
#define ASSERT_NULL(ptr) ASSERT_TRUE((ptr) == NULL)
#define ASSERT_NOT_NULL(ptr) ASSERT_TRUE((ptr) != NULL)

// Test function type
typedef int (*test_func)();

// Test structure
typedef struct {
    const char* name;
    test_func func;
} TestCase;

// Test basic JSON value creation
int test_basic_json_creation() {
    printf("  Testing basic JSON creation...\n");
    
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
    
    printf("  PASSED\n");
    return 1;
}

// Test JSON array operations
int test_json_array_operations() {
    printf("  Testing JSON array operations...\n");
    
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
    
    printf("  PASSED\n");
    return 1;
}

// Test JSON object operations
int test_json_object_operations() {
    printf("  Testing JSON object operations...\n");
    
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
    
    printf("  PASSED\n");
    return 1;
}

// Test JSON serialization and deserialization
int test_json_serialization() {
    printf("  Testing JSON serialization...\n");
    
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
    ASSERT_TRUE(strcmp(json_str, "{\"data\":[\"test\",42],\"name\":\"example\"}") == 0 ||
                strcmp(json_str, "{\"name\":\"example\",\"data\":[\"test\",42]}") == 0);
    
    printf("    Serialized JSON: %s\n", json_str);
    
    // Deserialize back
    json_error_t error;
    char* cmplx_json_str = "{\"meta\":{\"requestId\":\"req-8f3a9c21\",\"timestamp\":\"2025-01-15T12:34:56.789Z\",\"success\":true,\"version\":3,\"tags\":[\"test\",\"json\",\"complex\",\"edge-case\"]},\"user\":{\"id\":987654321012345700,\"username\":\"test_user_01\",\"email\":\"user@example.com\",\"roles\":[\"admin\",\"editor\",\"viewer\"],\"active\":true,\"lastLogin\":null},\"data\":{\"metrics\":{\"count\":123456,\"ratio\":0.987654321,\"maxValue\":10000,\"minValue\":-10000},\"matrix\":[[1,2,3],[4,5,6],[7,8,9]]},\"errors\":[],\"debug\":{\"processingTimeMs\":12.345,\"flags\":{\"experimental\":false,\"trace\":true}}}";
    // char* cmplx_json_str = "{\"data\":[\"test\",42],\"name\":\"example\"}";
    json_t* parsed = json_loads(cmplx_json_str, 0, &error);
    ASSERT_NOT_NULL(parsed);
    ASSERT_TRUE(json_is_object(parsed));
    
    json_t* name = json_object_get(parsed, "meta");
    ASSERT_NOT_NULL(name);
    ASSERT_TRUE(json_is_object(name));
    
    json_t* data = json_object_get(parsed, "data");
    ASSERT_NOT_NULL(data);
    ASSERT_TRUE(json_is_object(data));

    json_t* user = json_object_get(parsed, "user");
    ASSERT_NOT_NULL(user);
    ASSERT_TRUE(json_is_object(user));
    json_t* username = json_object_get(user, "username");
    ASSERT_NOT_NULL(username);
    ASSERT_TRUE(json_is_string(username));
    ASSERT_EQ(strcmp(json_string_value(username), "test_user_01"), 0);
    
    free(json_str);
    json_decref(root);
    json_decref(parsed);
    
    printf("  Deserialization test passed\n");
    printf("  PASSED\n");
    return 1;
}

// Test JSON equality
int test_json_equality() {
    printf("  Testing JSON equality...\n");
    
    json_t* obj1 = json_object();
    json_t* obj2 = json_object();
    
    json_object_set(obj1, "key", json_string("value"));
    json_object_set(obj2, "key", json_string("value"));
    
    ASSERT_TRUE(json_equal(obj1, obj2));
    
    json_object_set(obj2, "extra", json_integer(123));
    ASSERT_FALSE(json_equal(obj1, obj2));
    
    json_decref(obj1);
    json_decref(obj2);
    
    printf("  PASSED\n");
    return 1;
}

int main() {
    printf("Running Jansson C Linkage Tests...\n");
    printf("=================================\n");
    
    TestCase tests[] = {
        {"basic_json_creation", test_basic_json_creation},
        {"json_array_operations", test_json_array_operations},
        {"json_object_operations", test_json_object_operations},
        {"json_serialization", test_json_serialization},
        {"json_equality", test_json_equality},
        {NULL, NULL}
    };
    
    int passed = 0;
    int failed = 0;
    int i = 0;
    
    while (tests[i].name != NULL) {
        printf("Running test: %s\n", tests[i].name);
        if (tests[i].func()) {
            passed++;
        } else {
            failed++;
        }
        i++;
    }
    
    printf("=================================\n");
    printf("Test Results: %d passed, %d failed\n", passed, failed);
    
    return failed > 0 ? 1 : 0;
}
