#include <gtest/gtest.h>
#include <jansson.h>
#include <string>
#include <memory>

class JsonBuilderTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize any test setup if needed
    }

    void TearDown() override {
        // Cleanup any test resources if needed
    }
};

TEST_F(JsonBuilderTest, BuildSimpleObject) {
    // Build a simple JSON object programmatically
    json_t* obj = json_object();
    ASSERT_NE(obj, nullptr);
    
    json_t* name_str = json_string("John Doe");
    json_t* age_int = json_integer(30);
    json_t* active_bool = json_true();
    json_t* null_val = json_null();
    
    // Add string field
    int result = json_object_set(obj, "name", name_str);
    EXPECT_EQ(result, 0);
    
    // Add integer field
    result = json_object_set(obj, "age", age_int);
    EXPECT_EQ(result, 0);
    
    // Add boolean field
    result = json_object_set(obj, "active", active_bool);
    EXPECT_EQ(result, 0);
    
    // Add null field
    result = json_object_set(obj, "middle_name", null_val);
    EXPECT_EQ(result, 0);
    
    // Print the built object
    char* json_str = json_dumps(obj, 0);
    ASSERT_NE(json_str, nullptr);
    printf("Built JSON object: %s\n", json_str);
    
    // Verify the object was built correctly
    json_t* name = json_object_get(obj, "name");
    ASSERT_NE(name, nullptr);
    EXPECT_TRUE(json_is_string(name));
    EXPECT_STREQ(json_string_value(name), "John Doe");
    
    json_t* age = json_object_get(obj, "age");
    ASSERT_NE(age, nullptr);
    EXPECT_TRUE(json_is_integer(age));
    EXPECT_EQ(json_integer_value(age), 30);
    
    free(json_str);
    json_decref(obj);
    json_decref(name_str);
    json_decref(age_int);
    json_decref(active_bool);
    json_decref(null_val);
}

TEST_F(JsonBuilderTest, BuildArray) {
    // Build a JSON array programmatically
    json_t* arr = json_array();
    ASSERT_NE(arr, nullptr);
    
    json_t* int1 = json_integer(1);
    json_t* int2 = json_integer(2);
    json_t* str = json_string("hello");
    json_t* real_val = json_real(3.14);
    json_t* bool_val = json_true();
    json_t* null_val = json_null();
    
    // Add various types to the array
    int result = json_array_append(arr, int1);
    EXPECT_EQ(result, 0);
    
    result = json_array_append(arr, int2);
    EXPECT_EQ(result, 0);
    
    result = json_array_append(arr, str);
    EXPECT_EQ(result, 0);
    
    result = json_array_append(arr, real_val);
    EXPECT_EQ(result, 0);
    
    result = json_array_append(arr, bool_val);
    EXPECT_EQ(result, 0);
    
    result = json_array_append(arr, null_val);
    EXPECT_EQ(result, 0);
    
    // Print the built array
    char* json_str = json_dumps(arr, 0);
    ASSERT_NE(json_str, nullptr);
    printf("Built JSON array: %s\n", json_str);
    
    // Verify the array was built correctly
    EXPECT_EQ(json_array_size(arr), 6);
    
    json_t* elem = json_array_get(arr, 0);
    ASSERT_NE(elem, nullptr);
    EXPECT_TRUE(json_is_integer(elem));
    EXPECT_EQ(json_integer_value(elem), 1);
    
    elem = json_array_get(arr, 2);
    ASSERT_NE(elem, nullptr);
    EXPECT_TRUE(json_is_string(elem));
    EXPECT_STREQ(json_string_value(elem), "hello");
    
    free(json_str);
    json_decref(arr);
    json_decref(int1);
    json_decref(int2);
    json_decref(str);
    json_decref(real_val);
    json_decref(bool_val);
    json_decref(null_val);
}

TEST_F(JsonBuilderTest, BuildNestedStructure) {
    // Build a complex nested JSON structure
    json_t* root = json_object();
    ASSERT_NE(root, nullptr);
    
    // Create a nested object
    json_t* address = json_object();
    json_t* street = json_string("123 Main St");
    json_t* city = json_string("Anytown");
    json_t* zip = json_string("12345");
    json_object_set(address, "street", street);
    json_object_set(address, "city", city);
    json_object_set(address, "zip", zip);
    json_object_set(root, "address", address);
    
    // Create a nested array
    json_t* hobbies = json_array();
    json_t* hobby1 = json_string("reading");
    json_t* hobby2 = json_string("swimming");
    json_t* hobby3 = json_string("coding");
    json_array_append(hobbies, hobby1);
    json_array_append(hobbies, hobby2);
    json_array_append(hobbies, hobby3);
    json_object_set(root, "hobbies", hobbies);
    
    // Add other fields
    json_t* name = json_string("Alice");
    json_t* age = json_integer(25);
    json_object_set(root, "name", name);
    json_object_set(root, "age", age);
    
    // Print the built structure
    char* json_str = json_dumps(root, 0);
    ASSERT_NE(json_str, nullptr);
    printf("Built nested JSON structure:\n%s\n", json_str);
    
    // Verify the structure was built correctly
    json_t* name_check = json_object_get(root, "name");
    ASSERT_NE(name_check, nullptr);
    EXPECT_STREQ(json_string_value(name_check), "Alice");
    
    json_t* addr = json_object_get(root, "address");
    ASSERT_NE(addr, nullptr);
    EXPECT_TRUE(json_is_object(addr));
    
    json_t* city_check = json_object_get(addr, "city");
    ASSERT_NE(city_check, nullptr);
    EXPECT_STREQ(json_string_value(city_check), "Anytown");
    
    json_t* hobs = json_object_get(root, "hobbies");
    ASSERT_NE(hobs, nullptr);
    EXPECT_EQ(json_array_size(hobs), 3);
    
    free(json_str);
    json_decref(root);
    json_decref(address);
    json_decref(street);
    json_decref(city);
    json_decref(zip);
    json_decref(hobbies);
    json_decref(hobby1);
    json_decref(hobby2);
    json_decref(hobby3);
    json_decref(name);
    json_decref(age);
}

TEST_F(JsonBuilderTest, ModifyExistingObject) {
    // Create an object and modify it
    json_t* obj = json_object();
    ASSERT_NE(obj, nullptr);
    
    json_t* counter1 = json_integer(0);
    json_t* name1 = json_string("initial");
    
    // Set initial values
    json_object_set(obj, "counter", counter1);
    json_object_set(obj, "name", name1);
    
    // Create new values for modification
    json_t* counter2 = json_integer(5);
    json_t* name2 = json_string("updated");
    json_t* new_field = json_string("new_value");
    
    // Modify existing fields
    json_object_set(obj, "counter", counter2);
    json_object_set(obj, "name", name2);
    
    // Add new field
    json_object_set(obj, "new_field", new_field);
    
    // Print the modified object
    char* json_str = json_dumps(obj, 0);
    ASSERT_NE(json_str, nullptr);
    printf("Modified JSON object: %s\n", json_str);
    
    // Verify modifications
    json_t* counter = json_object_get(obj, "counter");
    ASSERT_NE(counter, nullptr);
    EXPECT_EQ(json_integer_value(counter), 5);
    
    json_t* name = json_object_get(obj, "name");
    ASSERT_NE(name, nullptr);
    EXPECT_STREQ(json_string_value(name), "updated");
    
    json_t* new_field_check = json_object_get(obj, "new_field");
    ASSERT_NE(new_field_check, nullptr);
    EXPECT_STREQ(json_string_value(new_field_check), "new_value");
    
    free(json_str);
    json_decref(obj);
    json_decref(counter1);
    json_decref(name1);
    json_decref(counter2);
    json_decref(name2);
    json_decref(new_field);
}

TEST_F(JsonBuilderTest, BuildWithRealNumbers) {
    // Test building JSON with real numbers
    json_t* obj = json_object();
    ASSERT_NE(obj, nullptr);
    
    json_t* pi = json_real(3.14159);
    json_t* e = json_real(2.71828);
    json_t* negative = json_real(-1.5);
    json_t* zero = json_real(0.0);
    
    json_object_set(obj, "pi", pi);
    json_object_set(obj, "e", e);
    json_object_set(obj, "negative", negative);
    json_object_set(obj, "zero", zero);
    
    // Print the object
    char* json_str = json_dumps(obj, 0);
    ASSERT_NE(json_str, nullptr);
    printf("JSON with real numbers: %s\n", json_str);
    
    // Verify the values
    json_t* pi_check = json_object_get(obj, "pi");
    ASSERT_NE(pi_check, nullptr);
    EXPECT_TRUE(json_is_real(pi_check));
    EXPECT_DOUBLE_EQ(json_real_value(pi_check), 3.14159);
    
    json_t* negative_check = json_object_get(obj, "negative");
    ASSERT_NE(negative_check, nullptr);
    EXPECT_DOUBLE_EQ(json_real_value(negative_check), -1.5);
    
    free(json_str);
    json_decref(obj);
    json_decref(pi);
    json_decref(e);
    json_decref(negative);
    json_decref(zero);
}
