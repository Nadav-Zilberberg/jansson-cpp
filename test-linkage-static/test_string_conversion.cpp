#include <gtest/gtest.h>
#include <jansson.h>
#include <string>
#include <memory>

class StringConversionTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize any test setup if needed
    }

    void TearDown() override {
        // Cleanup any test resources if needed
    }
};

TEST_F(StringConversionTest, ParseSimpleObject) {
    const char* json_str = "{\"name\":\"John\",\"age\":30}";
    json_error_t error;
    
    json_t* json = json_loads(json_str, 0, &error);
    ASSERT_NE(json, nullptr) << "Failed to parse JSON: " << error.text;
    
    // Verify it's an object
    EXPECT_TRUE(json_is_object(json));
    
    // Get and verify the name field
    json_t* name = json_object_get(json, "name");
    ASSERT_NE(name, nullptr);
    EXPECT_TRUE(json_is_string(name));
    EXPECT_STREQ(json_string_value(name), "John");
    
    // Get and verify the age field
    json_t* age = json_object_get(json, "age");
    ASSERT_NE(age, nullptr);
    EXPECT_TRUE(json_is_integer(age));
    EXPECT_EQ(json_integer_value(age), 30);
    
    json_decref(json);
}

TEST_F(StringConversionTest, ParseSimpleArray) {
    const char* json_str = "[1, 2, 3, \"hello\", true, null]";
    json_error_t error;
    
    json_t* json = json_loads(json_str, 0, &error);
    ASSERT_NE(json, nullptr) << "Failed to parse JSON: " << error.text;
    
    // Verify it's an array
    EXPECT_TRUE(json_is_array(json));
    EXPECT_EQ(json_array_size(json), 6);
    
    // Check first three elements are integers
    for (int i = 0; i < 3; i++) {
        json_t* elem = json_array_get(json, i);
        ASSERT_NE(elem, nullptr);
        EXPECT_TRUE(json_is_integer(elem));
        EXPECT_EQ(json_integer_value(elem), i + 1);
    }
    
    // Check string element
    json_t* str_elem = json_array_get(json, 3);
    ASSERT_NE(str_elem, nullptr);
    EXPECT_TRUE(json_is_string(str_elem));
    EXPECT_STREQ(json_string_value(str_elem), "hello");
    
    // Check boolean element
    json_t* bool_elem = json_array_get(json, 4);
    ASSERT_NE(bool_elem, nullptr);
    EXPECT_TRUE(json_is_true(bool_elem));
    
    // Check null element
    json_t* null_elem = json_array_get(json, 5);
    ASSERT_NE(null_elem, nullptr);
    EXPECT_TRUE(json_is_null(null_elem));
    
    json_decref(json);
}

TEST_F(StringConversionTest, ParseNestedStructure) {
    const char* json_str = "{\"users\":[{\"id\":1,\"name\":\"Alice\"},{\"id\":2,\"name\":\"Bob\"}],\"count\":2}";
    json_error_t error;
    
    json_t* json = json_loads(json_str, 0, &error);
    ASSERT_NE(json, nullptr) << "Failed to parse JSON: " << error.text;
    
    // Get users array
    json_t* users = json_object_get(json, "users");
    ASSERT_NE(users, nullptr);
    EXPECT_TRUE(json_is_array(users));
    EXPECT_EQ(json_array_size(users), 2);
    
    // Check first user
    json_t* user1 = json_array_get(users, 0);
    ASSERT_NE(user1, nullptr);
    EXPECT_TRUE(json_is_object(user1));
    
    json_t* user1_id = json_object_get(user1, "id");
    ASSERT_NE(user1_id, nullptr);
    EXPECT_TRUE(json_is_integer(user1_id));
    EXPECT_EQ(json_integer_value(user1_id), 1);
    
    json_t* user1_name = json_object_get(user1, "name");
    ASSERT_NE(user1_name, nullptr);
    EXPECT_TRUE(json_is_string(user1_name));
    EXPECT_STREQ(json_string_value(user1_name), "Alice");
    
    // Check count field
    json_t* count = json_object_get(json, "count");
    ASSERT_NE(count, nullptr);
    EXPECT_TRUE(json_is_integer(count));
    EXPECT_EQ(json_integer_value(count), 2);
    
    json_decref(json);
}

TEST_F(StringConversionTest, ParseInvalidJson) {
    const char* invalid_json = "{invalid json}";
    json_error_t error;
    
    json_t* json = json_loads(invalid_json, 0, &error);
    EXPECT_EQ(json, nullptr);
    
    // Error should be populated
    EXPECT_GT(strlen(error.text), 0);
}

TEST_F(StringConversionTest, ParseAllDataTypes) {
    const char* json_str = "{\"string\":\"test\",\"integer\":42,\"real\":3.14,\"boolean\":true,\"null_value\":null}";
    json_error_t error;
    
    json_t* json = json_loads(json_str, 0, &error);
    ASSERT_NE(json, nullptr) << "Failed to parse JSON: " << error.text;
    
    // String
    json_t* str_val = json_object_get(json, "string");
    ASSERT_NE(str_val, nullptr);
    EXPECT_TRUE(json_is_string(str_val));
    EXPECT_STREQ(json_string_value(str_val), "test");
    
    // Integer
    json_t* int_val = json_object_get(json, "integer");
    ASSERT_NE(int_val, nullptr);
    EXPECT_TRUE(json_is_integer(int_val));
    EXPECT_EQ(json_integer_value(int_val), 42);
    
    // Real
    json_t* real_val = json_object_get(json, "real");
    ASSERT_NE(real_val, nullptr);
    EXPECT_TRUE(json_is_real(real_val));
    EXPECT_DOUBLE_EQ(json_real_value(real_val), 3.14);
    
    // Boolean
    json_t* bool_val = json_object_get(json, "boolean");
    ASSERT_NE(bool_val, nullptr);
    EXPECT_TRUE(json_is_true(bool_val));
    
    // Null
    json_t* null_val = json_object_get(json, "null_value");
    ASSERT_NE(null_val, nullptr);
    EXPECT_TRUE(json_is_null(null_val));
    
    json_decref(json);
}
