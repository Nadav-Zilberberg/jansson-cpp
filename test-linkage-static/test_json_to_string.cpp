#include <gtest/gtest.h>
#include <jansson.h>
#include <string>
#include <memory>

class JsonToStringTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize any test setup if needed
    }

    void TearDown() override {
        // Cleanup any test resources if needed
    }
};

TEST_F(JsonToStringTest, SerializeSimpleObject) {
    // Create a simple JSON object
    json_t* obj = json_object();
    ASSERT_NE(obj, nullptr);
    
    json_t* name_str = json_string("John");
    json_t* age_int = json_integer(30);
    json_t* active_bool = json_true();
    
    json_object_set(obj, "name", name_str);
    json_object_set(obj, "age", age_int);
    json_object_set(obj, "active", active_bool);
    
    // Serialize to string
    char* json_str = json_dumps(obj, 0);
    ASSERT_NE(json_str, nullptr);
    
    // Print the JSON object
    printf("Serialized JSON object: %s\n", json_str);
    
    // Verify the string contains expected content
    std::string result(json_str);
    EXPECT_NE(result.find("\"name\":\"John\""), std::string::npos);
    EXPECT_NE(result.find("\"age\":30"), std::string::npos);
    EXPECT_NE(result.find("\"active\":true"), std::string::npos);
    
    free(json_str);
    json_decref(obj);
    json_decref(name_str);
    json_decref(age_int);
    json_decref(active_bool);
}

TEST_F(JsonToStringTest, SerializeSimpleArray) {
    // Create a JSON array
    json_t* arr = json_array();
    ASSERT_NE(arr, nullptr);
    
    json_t* int1 = json_integer(1);
    json_t* int2 = json_integer(2);
    json_t* str = json_string("hello");
    json_t* real_val = json_real(3.14);
    json_t* bool_val = json_true();
    json_t* null_val = json_null();
    
    json_array_append(arr, int1);
    json_array_append(arr, int2);
    json_array_append(arr, str);
    json_array_append(arr, real_val);
    json_array_append(arr, bool_val);
    json_array_append(arr, null_val);
    
    // Serialize to string
    char* json_str = json_dumps(arr, 0);
    ASSERT_NE(json_str, nullptr);
    
    // Print the JSON array
    printf("Serialized JSON array: %s\n", json_str);
    
    // Verify the string contains expected content
    std::string result(json_str);
    EXPECT_NE(result.find("1"), std::string::npos);
    EXPECT_NE(result.find("2"), std::string::npos);
    EXPECT_NE(result.find("\"hello\""), std::string::npos);
    EXPECT_NE(result.find("3.14"), std::string::npos);
    EXPECT_NE(result.find("true"), std::string::npos);
    EXPECT_NE(result.find("null"), std::string::npos);
    
    free(json_str);
    json_decref(arr);
    json_decref(int1);
    json_decref(int2);
    json_decref(str);
    json_decref(real_val);
    json_decref(bool_val);
    json_decref(null_val);
}

TEST_F(JsonToStringTest, SerializeNestedStructure) {
    // Create a nested JSON structure
    json_t* root = json_object();
    ASSERT_NE(root, nullptr);
    
    json_t* users = json_array();
    json_t* user1 = json_object();
    json_t* user1_id = json_integer(1);
    json_t* user1_name = json_string("Alice");
    json_object_set(user1, "id", user1_id);
    json_object_set(user1, "name", user1_name);
    json_array_append(users, user1);
    
    json_t* user2 = json_object();
    json_t* user2_id = json_integer(2);
    json_t* user2_name = json_string("Bob");
    json_object_set(user2, "id", user2_id);
    json_object_set(user2, "name", user2_name);
    json_array_append(users, user2);
    
    json_t* count = json_integer(2);
    json_object_set(root, "users", users);
    json_object_set(root, "count", count);
    
    // Serialize to string with pretty formatting (flags = 0 for now)
    char* json_str = json_dumps(root, 0);
    ASSERT_NE(json_str, nullptr);
    
    // Print the JSON object
    printf("Serialized nested JSON structure:\n%s\n", json_str);
    
    // Verify the string contains expected content
    std::string result(json_str);
    EXPECT_NE(result.find("\"users\""), std::string::npos);
    EXPECT_NE(result.find("\"count\":2"), std::string::npos);
    EXPECT_NE(result.find("\"id\":1"), std::string::npos);
    EXPECT_NE(result.find("\"name\":\"Alice\""), std::string::npos);
    
    free(json_str);
    json_decref(root);
    json_decref(users);
    json_decref(user1);
    json_decref(user1_id);
    json_decref(user1_name);
    json_decref(user2);
    json_decref(user2_id);
    json_decref(user2_name);
    json_decref(count);
}

TEST_F(JsonToStringTest, SerializeAllDataTypes) {
    // Create JSON object with all data types
    json_t* obj = json_object();
    ASSERT_NE(obj, nullptr);
    
    json_t* str_val = json_string("test");
    json_t* int_val = json_integer(42);
    json_t* real_val = json_real(3.14159);
    json_t* bool_true = json_true();
    json_t* bool_false = json_false();
    json_t* null_val = json_null();
    
    json_object_set(obj, "string", str_val);
    json_object_set(obj, "integer", int_val);
    json_object_set(obj, "real", real_val);
    json_object_set(obj, "boolean_true", bool_true);
    json_object_set(obj, "boolean_false", bool_false);
    json_object_set(obj, "null_value", null_val);
    
    // Serialize to string
    char* json_str = json_dumps(obj, 0);
    ASSERT_NE(json_str, nullptr);
    
    // Print the JSON object
    printf("Serialized JSON with all data types: %s\n", json_str);
    
    // Verify the string contains expected content
    std::string result(json_str);
    EXPECT_NE(result.find("\"string\":\"test\""), std::string::npos);
    EXPECT_NE(result.find("\"integer\":42"), std::string::npos);
    EXPECT_NE(result.find("\"real\":3.14159"), std::string::npos);
    EXPECT_NE(result.find("\"boolean_true\":true"), std::string::npos);
    EXPECT_NE(result.find("\"boolean_false\":false"), std::string::npos);
    EXPECT_NE(result.find("\"null_value\":null"), std::string::npos);
    
    free(json_str);
    json_decref(obj);
    json_decref(str_val);
    json_decref(int_val);
    json_decref(real_val);
    json_decref(bool_true);
    json_decref(bool_false);
    json_decref(null_val);
}

TEST_F(JsonToStringTest, SerializeEmptyStructures) {
    // Test empty object
    json_t* empty_obj = json_object();
    ASSERT_NE(empty_obj, nullptr);
    
    char* obj_str = json_dumps(empty_obj, 0);
    ASSERT_NE(obj_str, nullptr);
    
    printf("Empty object: %s\n", obj_str);
    EXPECT_STREQ(obj_str, "{}");
    
    free(obj_str);
    json_decref(empty_obj);
    
    // Test empty array
    json_t* empty_arr = json_array();
    ASSERT_NE(empty_arr, nullptr);
    
    char* arr_str = json_dumps(empty_arr, 0);
    ASSERT_NE(arr_str, nullptr);
    
    printf("Empty array: %s\n", arr_str);
    EXPECT_STREQ(arr_str, "[]");
    
    free(arr_str);
    json_decref(empty_arr);
}
