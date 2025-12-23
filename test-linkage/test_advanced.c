#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "jansson.h"

void test_reference_counting() {
    printf("=== Testing Reference Counting ===\n");
    
    // Create a value
    json_t* obj = json_object();
    assert(obj->refcount == 1);
    printf("✓ Initial refcount is 1\n");
    
    // Increase reference
    json_incref(obj);
    assert(obj->refcount == 2);
    printf("✓ Refcount increased to 2\n");
    
    // Decrease reference
    json_decref(obj);
    assert(obj->refcount == 1);
    printf("✓ Refcount decreased back to 1\n");
    
    // Final decref should free the object
    json_decref(obj);
    printf("✓ Object freed after final decref\n");
    
    printf("✓ Reference counting tests passed!\n\n");
}

void test_deep_copy() {
    printf("=== Testing Deep Copy ===\n");
    
    // Create a complex nested structure
    json_t* original = json_object();
    json_t* nested_arr = json_array();
    json_t* nested_obj = json_object();
    
    json_object_set(nested_obj, "key", json_string("value"));
    json_array_append(nested_arr, nested_obj);
    json_array_append(nested_arr, json_integer(42));
    json_object_set(original, "data", nested_arr);
    
    // Create deep copy
    json_t* copy = json_deep_copy(original);
    assert(copy != NULL);
    assert(copy != original);
    printf("✓ Deep copy created\n");
    
    // Verify equality but different objects
    assert(json_equal(original, copy));
    printf("✓ Copy is equal to original\n");
    
    // Modify copy and verify original is unchanged
    json_t* copy_data = json_object_get(copy, "data");
    json_array_append(copy_data, json_string("new"));
    
    json_t* original_data = json_object_get(original, "data");
    assert(json_array_size(original_data) == 2);
    assert(json_array_size(copy_data) == 3);
    printf("✓ Modifying copy doesn't affect original\n");
    
    // Cleanup
    json_decref(original);
    json_decref(copy);
    
    printf("✓ Deep copy tests passed!\n\n");
}

void test_iteration() {
    printf("=== Testing Iteration ===\n");
    
    // Create object with multiple properties
    json_t* obj = json_object();
    json_object_set(obj, "name", json_string("test"));
    json_object_set(obj, "value", json_integer(100));
    json_object_set(obj, "active", json_true());
    
    // Test object size
    assert(json_object_size(obj) == 3);
    printf("✓ Object has 3 properties\n");
    
    // Test individual property access
    json_t* name_prop = json_object_get(obj, "name");
    json_t* value_prop = json_object_get(obj, "value");
    json_t* active_prop = json_object_get(obj, "active");
    
    assert(name_prop != NULL);
    assert(value_prop != NULL);
    assert(active_prop != NULL);
    assert(json_is_string(name_prop));
    assert(json_is_integer(value_prop));
    assert(json_is_true(active_prop));
    assert(strcmp(json_string_value(name_prop), "test") == 0);
    assert(json_integer_value(value_prop) == 100);
    printf("✓ All properties accessible individually\n");
    
    // Test array iteration
    json_t* arr = json_array();
    json_array_append(arr, json_integer(10));
    json_array_append(arr, json_integer(20));
    json_array_append(arr, json_integer(30));
    
    printf("  Array elements: ");
    for (size_t i = 0; i < json_array_size(arr); i++) {
        json_t* elem = json_array_get(arr, i);
        if (i > 0) printf(", ");
        printf("%lld", (long long)json_integer_value(elem));
    }
    printf("\n");
    
    assert(json_array_size(arr) == 3);
    printf("✓ Array iteration works\n");
    
    // Cleanup
    json_decref(obj);
    json_decref(arr);
    
    printf("✓ Iteration tests passed!\n\n");
}

void test_error_handling() {
    printf("=== Testing Error Handling ===\n");
    
    // Test invalid JSON parsing
    json_error_t error;
    json_t* invalid = json_loads("{ invalid json }", 0, &error);
    assert(invalid == NULL);
    assert(error.code != 0);
    printf("✓ Invalid JSON rejected, error code: %d\n", error.code);
    printf("  Error text: %s\n", error.text);
    
    // Test unclosed array
    invalid = json_loads("[1, 2, 3", 0, &error);
    assert(invalid == NULL);
    printf("✓ Unclosed array rejected: %s\n", error.text);
    
    // Test unclosed object
    invalid = json_loads("{\"key\": \"value\"", 0, &error);
    assert(invalid == NULL);
    printf("✓ Unclosed object rejected: %s\n", error.text);
    
    // Test invalid number
    invalid = json_loads("{\"key\": 123.45.67}", 0, &error);
    assert(invalid == NULL);
    printf("✓ Invalid number rejected: %s\n", error.text);
    
    // Test invalid string escape
    invalid = json_loads("\"invalid \\x escape\"", 0, &error);
    assert(invalid == NULL);
    printf("✓ Invalid string escape rejected: %s\n", error.text);
    
    // Test array bounds
    json_t* arr = json_array();
    json_array_append(arr, json_string("only"));
    
    json_t* out_of_bounds = json_array_get(arr, 10);
    assert(out_of_bounds == NULL);
    printf("✓ Out of bounds array access returns NULL\n");
    
    // Test non-existent object key
    json_t* obj = json_object();
    json_object_set(obj, "existing", json_string("value"));
    
    json_t* missing = json_object_get(obj, "missing");
    assert(missing == NULL);
    printf("✓ Non-existent object key returns NULL\n");
    
    // Cleanup
    json_decref(arr);
    json_decref(obj);
    
    printf("✓ Error handling tests passed!\n\n");
}

int main() {
    printf("Starting Advanced C Linkage Tests\n");
    printf("=================================\n\n");
    
    test_reference_counting();
    test_deep_copy();
    test_iteration();
    test_error_handling();
    
    printf("All advanced tests passed successfully! ✓\n");
    return 0;
}
