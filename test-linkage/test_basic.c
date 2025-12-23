#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "jansson.h"

void test_basic_creation() {
    printf("=== Testing Basic JSON Creation ===\n");
    
    // Test null creation
    json_t* null_val = json_null();
    assert(null_val != NULL);
    assert(json_is_null(null_val));
    printf("✓ Null value created successfully\n");
    
    // Test boolean creation
    json_t* true_val = json_true();
    json_t* false_val = json_false();
    assert(true_val != NULL);
    assert(false_val != NULL);
    assert(json_is_true(true_val));
    assert(json_is_false(false_val));
    assert(json_is_boolean(true_val));
    assert(json_is_boolean(false_val));
    printf("✓ Boolean values created successfully\n");
    
    // Test integer creation
    json_t* int_val = json_integer(42);
    assert(int_val != NULL);
    assert(json_is_integer(int_val));
    assert(json_integer_value(int_val) == 42);
    printf("✓ Integer value created successfully\n");
    
    // Test real creation
    json_t* real_val = json_real(3.14);
    assert(real_val != NULL);
    assert(json_is_real(real_val));
    assert(json_real_value(real_val) == 3.14);
    printf("✓ Real value created successfully\n");
    
    // Test string creation
    json_t* str_val = json_string("test");
    assert(str_val != NULL);
    assert(json_is_string(str_val));
    assert(strcmp(json_string_value(str_val), "test") == 0);
    printf("✓ String value created successfully\n");
    
    // Cleanup
    json_decref(null_val);
    json_decref(true_val);
    json_decref(false_val);
    json_decref(int_val);
    json_decref(real_val);
    json_decref(str_val);
    
    printf("✓ All basic creation tests passed!\n\n");
}

void test_array_operations() {
    printf("=== Testing Array Operations ===\n");
    
    // Create array
    json_t* arr = json_array();
    assert(arr != NULL);
    assert(json_is_array(arr));
    assert(json_array_size(arr) == 0);
    printf("✓ Empty array created\n");
    
    // Add elements
    json_t* int_val = json_integer(42);
    json_t* str_val = json_string("test");
    
    assert(json_array_append(arr, int_val) == 0);
    assert(json_array_append(arr, str_val) == 0);
    assert(json_array_size(arr) == 2);
    printf("✓ Elements appended to array\n");
    
    // Check elements
    json_t* elem0 = json_array_get(arr, 0);
    json_t* elem1 = json_array_get(arr, 1);
    assert(elem0 != NULL);
    assert(elem1 != NULL);
    assert(json_is_integer(elem0));
    assert(json_is_string(elem1));
    assert(json_integer_value(elem0) == 42);
    assert(strcmp(json_string_value(elem1), "test") == 0);
    printf("✓ Array elements retrieved correctly\n");
    
    // Cleanup
    json_decref(arr);  // This will also decref all elements
    
    printf("✓ All array tests passed!\n\n");
}

void test_object_operations() {
    printf("=== Testing Object Operations ===\n");
    
    // Create object
    json_t* obj = json_object();
    assert(obj != NULL);
    assert(json_is_object(obj));
    assert(json_object_size(obj) == 0);
    printf("✓ Empty object created\n");
    
    // Add properties
    json_t* name_val = json_string("John");
    json_t* age_val = json_integer(30);
    
    assert(json_object_set(obj, "name", name_val) == 0);
    assert(json_object_set(obj, "age", age_val) == 0);
    assert(json_object_size(obj) == 2);
    printf("✓ Properties added to object\n");
    
    // Get properties
    json_t* name_prop = json_object_get(obj, "name");
    json_t* age_prop = json_object_get(obj, "age");
    assert(name_prop != NULL);
    assert(age_prop != NULL);
    assert(json_is_string(name_prop));
    assert(json_is_integer(age_prop));
    assert(strcmp(json_string_value(name_prop), "John") == 0);
    assert(json_integer_value(age_prop) == 30);
    printf("✓ Object properties retrieved correctly\n");
    
    // Cleanup
    json_decref(obj);  // This will also decref all properties
    
    printf("✓ All object tests passed!\n\n");
}

void test_serialization() {
    printf("=== Testing Serialization ===\n");
    
    // Create a complex JSON structure
    json_t* root = json_object();
    json_t* arr = json_array();
    
    json_array_append(arr, json_integer(1));
    json_array_append(arr, json_integer(2));
    json_array_append(arr, json_integer(3));
    
    json_object_set(root, "numbers", arr);
    json_object_set(root, "name", json_string("test"));
    json_object_set(root, "active", json_true());
    
    // Serialize to string
    char* json_str = json_dumps(root, 0);
    assert(json_str != NULL);
    printf("✓ JSON serialized: %s\n", json_str);
    
    // Parse it back
    json_error_t error;
    json_t* parsed = json_loads(json_str, 0, &error);
    assert(parsed != NULL);
    assert(json_is_object(parsed));
    printf("✓ JSON parsed back successfully\n");
    
    // Verify the parsed content
    json_t* parsed_name = json_object_get(parsed, "name");
    assert(parsed_name != NULL);
    assert(strcmp(json_string_value(parsed_name), "test") == 0);
    printf("✓ Parsed content verified\n");
    
    // Cleanup
    free(json_str);
    json_decref(root);
    json_decref(parsed);
    
    printf("✓ All serialization tests passed!\n\n");
}

int main() {
    printf("Starting C Linkage Tests for Jansson Library\n");
    printf("============================================\n\n");
    
    test_basic_creation();
    test_array_operations();
    test_object_operations();
    test_serialization();
    
    printf("All tests passed successfully! ✓\n");
    return 0;
}
