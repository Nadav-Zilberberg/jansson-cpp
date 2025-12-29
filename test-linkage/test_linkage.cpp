#include <iostream>
#include <cstring>

extern "C" {
#include "jansson.h"
}

// Simple test framework for standalone testing
#ifdef USE_GTEST
#include <gtest/gtest.h>
#define TEST_BEGIN(name) TEST(LinkageTest, name) {
#define TEST_END }
#define ASSERT_TRUE(cond) EXPECT_TRUE(cond)
#define ASSERT_FALSE(cond) EXPECT_FALSE(cond)
#define ASSERT_EQ(a, b) EXPECT_EQ(a, b)
#define ASSERT_NE(a, b) EXPECT_NE(a, b)
#define ASSERT_STREQ(a, b) EXPECT_STREQ(a, b)
#define ASSERT_NULL(ptr) EXPECT_EQ(ptr, nullptr)
#define ASSERT_NOT_NULL(ptr) EXPECT_NE(ptr, nullptr)
#else
#define TEST_BEGIN(name) bool test_##name() { std::cout << "Testing " #name "... ";
#define TEST_END return true; }
#define ASSERT_TRUE(cond) if (!(cond)) { std::cout << "FAILED: " #cond << std::endl; return false; }
#define ASSERT_FALSE(cond) if (cond) { std::cout << "FAILED: !" #cond << std::endl; return false; }
#define ASSERT_EQ(a, b) if ((a) != (b)) { std::cout << "FAILED: " #a " != " #b << std::endl; return false; }
#define ASSERT_NE(a, b) if ((a) == (b)) { std::cout << "FAILED: " #a " == " #b << std::endl; return false; }
#define ASSERT_STREQ(a, b) if (std::strcmp((a), (b)) != 0) { std::cout << "FAILED: " #a " != " #b << std::endl; return false; }
#define ASSERT_NULL(ptr) if ((ptr) != nullptr) { std::cout << "FAILED: " #ptr " != nullptr" << std::endl; return false; }
#define ASSERT_NOT_NULL(ptr) if ((ptr) == nullptr) { std::cout << "FAILED: " #ptr " == nullptr" << std::endl; return false; }
#endif

// Test basic JSON types
TEST_BEGIN(basic_types)
    // Test null
    json_t* null_obj = json_null();
    ASSERT_NOT_NULL(null_obj);
    ASSERT_TRUE(json_is_null(null_obj));
    json_decref(null_obj);
    
    // Test boolean
    json_t* true_obj = json_true();
    json_t* false_obj = json_false();
    ASSERT_NOT_NULL(true_obj);
    ASSERT_NOT_NULL(false_obj);
    ASSERT_TRUE(json_is_true(true_obj));
    ASSERT_TRUE(json_is_false(false_obj));
    json_decref(true_obj);
    json_decref(false_obj);
    
    // Test integer
    json_t* int_obj = json_integer(42);
    ASSERT_NOT_NULL(int_obj);
    ASSERT_TRUE(json_is_integer(int_obj));
    ASSERT_EQ(json_integer_value(int_obj), 42);
    json_decref(int_obj);
    
    // Test real
    json_t* real_obj = json_real(3.14159);
    ASSERT_NOT_NULL(real_obj);
    ASSERT_TRUE(json_is_real(real_obj));
    ASSERT_EQ(json_real_value(real_obj), 3.14159);
    json_decref(real_obj);
    
    // Test string
    json_t* str_obj = json_string("Hello, World!");
    ASSERT_NOT_NULL(str_obj);
    ASSERT_TRUE(json_is_string(str_obj));
    ASSERT_STREQ(json_string_value(str_obj), "Hello, World!");
    json_decref(str_obj);
TEST_END

// Test JSON object operations
TEST_BEGIN(object_operations)
    json_t* obj = json_object();
    ASSERT_NOT_NULL(obj);
    ASSERT_TRUE(json_is_object(obj));
    
    // Add properties
    ASSERT_EQ(json_object_set(obj, "name", json_string("test")), 0);
    ASSERT_EQ(json_object_set(obj, "value", json_integer(123)), 0);
    ASSERT_EQ(json_object_set(obj, "active", json_true()), 0);
    
    // Verify properties
    json_t* name = json_object_get(obj, "name");
    ASSERT_NOT_NULL(name);
    ASSERT_TRUE(json_is_string(name));
    ASSERT_STREQ(json_string_value(name), "test");
    
    json_t* value = json_object_get(obj, "value");
    ASSERT_NOT_NULL(value);
    ASSERT_TRUE(json_is_integer(value));
    ASSERT_EQ(json_integer_value(value), 123);
    
    json_t* active = json_object_get(obj, "active");
    ASSERT_NOT_NULL(active);
    ASSERT_TRUE(json_is_true(active));
    
    json_decref(obj);
TEST_END

// Test JSON array operations
TEST_BEGIN(array_operations)
    json_t* arr = json_array();
    ASSERT_NOT_NULL(arr);
    ASSERT_TRUE(json_is_array(arr));
    ASSERT_EQ(json_array_size(arr), 0);
    
    // Add elements
    ASSERT_EQ(json_array_append(arr, json_string("first")), 0);
    ASSERT_EQ(json_array_append(arr, json_integer(42)), 0);
    ASSERT_EQ(json_array_append(arr, json_true()), 0);
    
    // Verify size
    ASSERT_EQ(json_array_size(arr), 3);
    
    // Verify elements
    json_t* elem0 = json_array_get(arr, 0);
    ASSERT_NOT_NULL(elem0);
    ASSERT_TRUE(json_is_string(elem0));
    ASSERT_STREQ(json_string_value(elem0), "first");
    
    json_t* elem1 = json_array_get(arr, 1);
    ASSERT_NOT_NULL(elem1);
    ASSERT_TRUE(json_is_integer(elem1));
    ASSERT_EQ(json_integer_value(elem1), 42);
    
    json_t* elem2 = json_array_get(arr, 2);
    ASSERT_NOT_NULL(elem2);
    ASSERT_TRUE(json_is_true(elem2));
    
    json_decref(arr);
TEST_END

// Test JSON parsing and serialization
TEST_BEGIN(parsing_and_serialization)
    const char* json_str = "{\"name\":\"test\",\"value\":123,\"active\":true}";
    json_error_t error;
    
    json_t* parsed = json_loads(json_str, 0, &error);
    ASSERT_NOT_NULL(parsed);
    ASSERT_TRUE(json_is_object(parsed));
    
    // Verify the parsed content by checking individual properties
    json_t* name = json_object_get(parsed, "name");
    ASSERT_NOT_NULL(name);
    ASSERT_TRUE(json_is_string(name));
    ASSERT_STREQ(json_string_value(name), "test");
    
    json_t* value = json_object_get(parsed, "value");
    ASSERT_NOT_NULL(value);
    ASSERT_TRUE(json_is_integer(value));
    ASSERT_EQ(json_integer_value(value), 123);
    
    json_t* active = json_object_get(parsed, "active");
    ASSERT_NOT_NULL(active);
    ASSERT_TRUE(json_is_true(active));
    
    // Serialize back to string (order may vary, so just verify it works)
    char* serialized = json_dumps(parsed, 0);
    ASSERT_NOT_NULL(serialized);
    
    // Verify the serialized string is valid JSON by parsing it again
    json_t* reparsed = json_loads(serialized, 0, &error);
    ASSERT_NOT_NULL(reparsed);
    ASSERT_TRUE(json_is_object(reparsed));
    
    // Verify the reparsed content matches
    json_t* reparsed_name = json_object_get(reparsed, "name");
    ASSERT_NOT_NULL(reparsed_name);
    ASSERT_TRUE(json_is_string(reparsed_name));
    ASSERT_STREQ(json_string_value(reparsed_name), "test");
    
    free(serialized);
    json_decref(parsed);
    json_decref(reparsed);
TEST_END

// Test complex JSON structure
TEST_BEGIN(complex_structure)
    json_t* root = json_object();
    json_t* person = json_object();
    json_t* address = json_object();
    json_t* hobbies = json_array();
    
    ASSERT_NOT_NULL(root);
    ASSERT_NOT_NULL(person);
    ASSERT_NOT_NULL(address);
    ASSERT_NOT_NULL(hobbies);
    
    // Build address object
    ASSERT_EQ(json_object_set(address, "street", json_string("123 Main St")), 0);
    ASSERT_EQ(json_object_set(address, "city", json_string("Anytown")), 0);
    ASSERT_EQ(json_object_set(address, "zip", json_string("12345")), 0);
    
    // Build hobbies array
    ASSERT_EQ(json_array_append(hobbies, json_string("reading")), 0);
    ASSERT_EQ(json_array_append(hobbies, json_string("coding")), 0);
    ASSERT_EQ(json_array_append(hobbies, json_string("gaming")), 0);
    
    // Build person object
    ASSERT_EQ(json_object_set(person, "name", json_string("John Doe")), 0);
    ASSERT_EQ(json_object_set(person, "age", json_integer(30)), 0);
    ASSERT_EQ(json_object_set(person, "address", address), 0);
    ASSERT_EQ(json_object_set(person, "hobbies", hobbies), 0);
    
    // Add person to root
    ASSERT_EQ(json_object_set(root, "person", person), 0);
    ASSERT_EQ(json_object_set(root, "version", json_string("1.0")), 0);
    
    // Verify structure
    json_t* retrieved_person = json_object_get(root, "person");
    ASSERT_NOT_NULL(retrieved_person);
    ASSERT_TRUE(json_is_object(retrieved_person));
    
    json_t* retrieved_address = json_object_get(retrieved_person, "address");
    ASSERT_NOT_NULL(retrieved_address);
    ASSERT_TRUE(json_is_object(retrieved_address));
    
    json_t* retrieved_hobbies = json_object_get(retrieved_person, "hobbies");
    ASSERT_NOT_NULL(retrieved_hobbies);
    ASSERT_TRUE(json_is_array(retrieved_hobbies));
    ASSERT_EQ(json_array_size(retrieved_hobbies), 3);
    
    // Print the JSON for verification
    char* json_str = json_dumps(root, 0);
    ASSERT_NOT_NULL(json_str);
    printf("Complex JSON structure:\n%s\n", json_str);
    
    free(json_str);
    json_decref(root);
TEST_END

#ifndef USE_GTEST
// Main function for standalone testing
int main() {
    std::cout << "Running Jansson linkage tests..." << std::endl;
    
    bool all_passed = true;
    
    std::cout << "Testing basic_types... ";
    if (test_basic_types()) {
        std::cout << "PASSED" << std::endl;
    } else {
        std::cout << "FAILED" << std::endl;
        all_passed = false;
    }
    
    std::cout << "Testing object_operations... ";
    if (test_object_operations()) {
        std::cout << "PASSED" << std::endl;
    } else {
        std::cout << "FAILED" << std::endl;
        all_passed = false;
    }
    
    std::cout << "Testing array_operations... ";
    if (test_array_operations()) {
        std::cout << "PASSED" << std::endl;
    } else {
        std::cout << "FAILED" << std::endl;
        all_passed = false;
    }
    
    std::cout << "Testing parsing_and_serialization... ";
    if (test_parsing_and_serialization()) {
        std::cout << "PASSED" << std::endl;
    } else {
        std::cout << "FAILED" << std::endl;
        all_passed = false;
    }
    
    std::cout << "Testing complex_structure... ";
    if (test_complex_structure()) {
        std::cout << "PASSED" << std::endl;
    } else {
        std::cout << "FAILED" << std::endl;
        all_passed = false;
    }
    
    if (all_passed) {
        std::cout << "\nAll tests PASSED!" << std::endl;
        return 0;
    } else {
        std::cout << "\nSome tests FAILED!" << std::endl;
        return 1;
    }
}
#endif
