#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "jansson.h"

void test_parse_errors() {
    printf("=== Testing Parse Errors ===\n");
    
    json_error_t error;
    
    // Test completely invalid JSON
    json_t* result = json_loads("this is not json", 0, &error);
    assert(result == NULL);
    assert(error.code != 0);
    printf("✓ Invalid JSON rejected: %s\n", error.text);
    
    // Test unclosed array
    result = json_loads("[1, 2, 3", 0, &error);
    assert(result == NULL);
    printf("✓ Unclosed array rejected: %s\n", error.text);
    
    // Test unclosed object
    result = json_loads("{\"key\": \"value\"", 0, &error);
    assert(result == NULL);
    printf("✓ Unclosed object rejected: %s\n", error.text);
    
    // Test invalid number - this might actually parse as a valid number
    // Let's test something that's definitely invalid
    result = json_loads("{\"key\": undefined}", 0, &error);
    assert(result == NULL);
    printf("✓ Invalid value rejected: %s\n", error.text);
    
    // Test invalid string escape
    result = json_loads("\"invalid \\x escape\"", 0, &error);
    assert(result == NULL);
    printf("✓ Invalid string escape rejected: %s\n", error.text);
    
    printf("✓ Parse error tests passed!\n\n");
}

void test_memory_errors() {
    printf("=== Testing Memory-related Errors ===\n");
    
    // Test operations on NULL pointers
    json_t* result = json_object_get(NULL, "key");
    assert(result == NULL);
    printf("✓ json_object_get with NULL object returns NULL\n");
    
    result = json_array_get(NULL, 0);
    assert(result == NULL);
    printf("✓ json_array_get with NULL array returns NULL\n");
    
    int int_result = json_object_set(NULL, "key", json_string("value"));
    assert(int_result != 0);
    printf("✓ json_object_set with NULL object fails\n");
    
    int_result = json_array_append(NULL, json_string("value"));
    assert(int_result != 0);
    printf("✓ json_array_append with NULL array fails\n");
    
    printf("✓ Memory error tests passed!\n\n");
}

void test_type_errors() {
    printf("=== Testing Type-related Errors ===\n");
    
    // Create different types
    json_t* str = json_string("test");
    json_t* num = json_integer(42);
    json_t* arr = json_array();
    json_t* obj = json_object();
    
    // Test operations on wrong types
    const char* str_val = json_string_value(num);
    assert(str_val == NULL);
    printf("✓ json_string_value on non-string returns NULL\n");
    
    json_int_t int_val = json_integer_value(str);
    assert(int_val == 0);  // Default value for non-integer
    printf("✓ json_integer_value on non-integer returns 0\n");
    
    json_t* elem = json_array_get(obj, 0);
    assert(elem == NULL);
    printf("✓ json_array_get on non-array returns NULL\n");
    
    json_t* prop = json_object_get(arr, "key");
    assert(prop == NULL);
    printf("✓ json_object_get on non-object returns NULL\n");
    
    // Test setting wrong types
    int result = json_string_set(arr, "new value");
    assert(result != 0);
    printf("✓ json_string_set on non-string fails\n");
    
    result = json_integer_set(obj, 100);
    assert(result != 0);
    printf("✓ json_integer_set on non-integer fails\n");
    
    // Cleanup
    json_decref(str);
    json_decref(num);
    json_decref(arr);
    json_decref(obj);
    
    printf("✓ Type error tests passed!\n\n");
}

void test_bounds_errors() {
    printf("=== Testing Bounds Errors ===\n");
    
    // Create array with one element
    json_t* arr = json_array();
    json_array_append(arr, json_string("only"));
    
    // Test out of bounds access
    json_t* result = json_array_get(arr, 0);
    assert(result != NULL);
    printf("✓ Valid array access works\n");
    
    result = json_array_get(arr, 1);
    assert(result == NULL);
    printf("✓ Out of bounds array access returns NULL\n");
    
    result = json_array_get(arr, 100);
    assert(result == NULL);
    printf("✓ Far out of bounds array access returns NULL\n");
    
    // Test negative index (should fail)
    result = json_array_get(arr, -1);
    assert(result == NULL);
    printf("✓ Negative array index returns NULL\n");
    
    // Test setting out of bounds
    int set_result = json_array_set(arr, 1, json_string("new"));
    assert(set_result != 0);
    printf("✓ Out of bounds array set fails\n");
    
    // Test removing out of bounds
    int del_result = json_array_remove(arr, 1);
    assert(del_result != 0);
    printf("✓ Out of bounds array remove fails\n");
    
    // Cleanup
    json_decref(arr);
    
    printf("✓ Bounds error tests passed!\n\n");
}

void test_duplicate_key_errors() {
    printf("=== Testing Duplicate Key Errors ===\n");
    
    json_t* obj = json_object();
    
    // Set initial value
    int result = json_object_set(obj, "key", json_string("first"));
    assert(result == 0);
    printf("✓ Initial key set successful\n");
    
    // Try to set same key again (should succeed and overwrite)
    result = json_object_set(obj, "key", json_string("second"));
    assert(result == 0);
    
    json_t* value = json_object_get(obj, "key");
    assert(strcmp(json_string_value(value), "second") == 0);
    printf("✓ Duplicate key overwrites previous value\n");
    
    // Test with set_new (should also work)
    result = json_object_set_new(obj, "key", json_string("third"));
    assert(result == 0);
    
    value = json_object_get(obj, "key");
    assert(strcmp(json_string_value(value), "third") == 0);
    printf("✓ set_new also overwrites duplicate key\n");
    
    // Cleanup
    json_decref(obj);
    
    printf("✓ Duplicate key tests passed!\n\n");
}

int main() {
    printf("Starting C Linkage Error Handling Tests\n");
    printf("=======================================\n\n");
    
    test_parse_errors();
    test_memory_errors();
    test_type_errors();
    test_bounds_errors();
    test_duplicate_key_errors();
    
    printf("All error handling tests passed successfully! ✓\n");
    return 0;
}
