#include <gtest/gtest.h>

extern "C" {
#include "jansson.h"
}

// Test JSON object creation and manipulation
TEST(JsonBuilderTest, ObjectCreation) {
    json_t* obj = json_object();
    ASSERT_NE(obj, nullptr);
    EXPECT_TRUE(json_is_object(obj));
    
    // Add string property
    json_t* str = json_string("test value");
    int result = json_object_set(obj, "key", str);
    EXPECT_EQ(result, 0);
    
    // Verify the property exists
    json_t* retrieved = json_object_get(obj, "key");
    ASSERT_NE(retrieved, nullptr);
    EXPECT_TRUE(json_is_string(retrieved));
    EXPECT_STREQ(json_string_value(retrieved), "test value");
    
    json_decref(obj);  // This also decrements the string we added
}

// Test JSON array creation and manipulation
TEST(JsonBuilderTest, ArrayCreation) {
    json_t* arr = json_array();
    ASSERT_NE(arr, nullptr);
    EXPECT_TRUE(json_is_array(arr));
    
    // Add elements
    json_t* str1 = json_string("first");
    json_t* str2 = json_string("second");
    
    EXPECT_EQ(json_array_append(arr, str1), 0);
    EXPECT_EQ(json_array_append(arr, str2), 0);
    
    // Verify array size
    EXPECT_EQ(json_array_size(arr), 2);
    
    // Verify elements
    json_t* elem0 = json_array_get(arr, 0);
    json_t* elem1 = json_array_get(arr, 1);
    
    ASSERT_NE(elem0, nullptr);
    ASSERT_NE(elem1, nullptr);
    EXPECT_STREQ(json_string_value(elem0), "first");
    EXPECT_STREQ(json_string_value(elem1), "second");
    
    json_decref(arr);  // This also decrements the elements we added
}

// Test complex JSON structure
TEST(JsonBuilderTest, ComplexStructure) {
    json_t* root = json_object();
    json_t* person = json_object();
    json_t* hobbies = json_array();
    
    ASSERT_NE(root, nullptr);
    ASSERT_NE(person, nullptr);
    ASSERT_NE(hobbies, nullptr);
    
    // Build person object
    json_object_set(person, "name", json_string("John Doe"));
    json_object_set(person, "age", json_integer(30));
    json_object_set(person, "active", json_true());
    
    // Build hobbies array
    json_array_append(hobbies, json_string("reading"));
    json_array_append(hobbies, json_string("coding"));
    json_array_append(hobbies, json_string("gaming"));
    
    // Add person and hobbies to root
    json_object_set(root, "person", person);
    json_object_set(root, "hobbies", hobbies);
    
    // Verify structure
    json_t* retrieved_person = json_object_get(root, "person");
    ASSERT_NE(retrieved_person, nullptr);
    EXPECT_TRUE(json_is_object(retrieved_person));
    
    json_t* retrieved_hobbies = json_object_get(root, "hobbies");
    ASSERT_NE(retrieved_hobbies, nullptr);
    EXPECT_TRUE(json_is_array(retrieved_hobbies));
    EXPECT_EQ(json_array_size(retrieved_hobbies), 3);
    
    // Print the JSON for verification
    char* json_str = json_dumps(root, 0);
    ASSERT_NE(json_str, nullptr);
    printf("Complex JSON structure:\n%s\n", json_str);
    
    free(json_str);
    json_decref(root);
}
