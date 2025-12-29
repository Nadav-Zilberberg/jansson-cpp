#include <gtest/gtest.h>

extern "C" {
#include "jansson.h"
}

// Test JSON parsing with invalid JSON (should handle errors gracefully)
TEST(JsonExceptionTest, InvalidJsonParsing) {
    const char* invalid_json = "{ invalid json }";
    json_error_t error;
    
    json_t* result = json_loads(invalid_json, 0, &error);
    EXPECT_EQ(result, nullptr);
    
    // The error structure should contain information about the parsing error
    EXPECT_GT(error.line, 0);
    EXPECT_GT(error.column, 0);
    EXPECT_NE(error.text[0], '\0');
}

// Test JSON parsing with valid but complex JSON
TEST(JsonLiteralTest, ComplexJsonParsing) {
    const char* complex_json = R"({
        "users": [
            {
                "id": 1,
                "name": "Alice",
                "email": "alice@example.com",
                "active": true,
                "balance": 123.45
            },
            {
                "id": 2,
                "name": "Bob",
                "email": "bob@example.com",
                "active": false,
                "balance": 0.0
            }
        ],
        "total_users": 2,
        "timestamp": "2023-12-29T10:30:00Z"
    })";
    
    json_error_t error;
    json_t* result = json_loads(complex_json, 0, &error);
    ASSERT_NE(result, nullptr);
    
    // Verify structure
    EXPECT_TRUE(json_is_object(result));
    
    json_t* users = json_object_get(result, "users");
    ASSERT_NE(users, nullptr);
    EXPECT_TRUE(json_is_array(users));
    EXPECT_EQ(json_array_size(users), 2);
    
    json_t* total_users = json_object_get(result, "total_users");
    ASSERT_NE(total_users, nullptr);
    EXPECT_TRUE(json_is_integer(total_users));
    EXPECT_EQ(json_integer_value(total_users), 2);
    
    // Print the parsed JSON
    char* json_str = json_dumps(result, 0);
    ASSERT_NE(json_str, nullptr);
    printf("Parsed complex JSON:\n%s\n", json_str);
    
    free(json_str);
    json_decref(result);
}

// Test JSON array literal parsing
TEST(JsonLiteralTest, ArrayLiteralParsing) {
    const char* array_json = "[1, 2, 3, 4, 5]";
    json_error_t error;
    
    json_t* result = json_loads(array_json, 0, &error);
    ASSERT_NE(result, nullptr);
    EXPECT_TRUE(json_is_array(result));
    EXPECT_EQ(json_array_size(result), 5);
    
    // Verify array elements
    for (size_t i = 0; i < json_array_size(result); ++i) {
        json_t* elem = json_array_get(result, i);
        ASSERT_NE(elem, nullptr);
        EXPECT_TRUE(json_is_integer(elem));
        EXPECT_EQ(json_integer_value(elem), static_cast<int>(i + 1));
    }
    
    json_decref(result);
}

// Test JSON string literal with escape sequences
TEST(JsonLiteralTest, StringEscaping) {
    const char* escaped_json = R"("Line 1\nLine 2\tTabbed\"Quoted\"")";
    json_error_t error;
    
    json_t* result = json_loads(escaped_json, 0, &error);
    ASSERT_NE(result, nullptr);
    EXPECT_TRUE(json_is_string(result));
    
    const char* str_value = json_string_value(result);
    ASSERT_NE(str_value, nullptr);
    EXPECT_STREQ(str_value, "Line 1\nLine 2\tTabbed\"Quoted\"");
    
    // Test round-trip: dump back to string
    char* dumped = json_dumps(result, 0);
    ASSERT_NE(dumped, nullptr);
    EXPECT_STREQ(dumped, escaped_json);
    
    free(dumped);
    json_decref(result);
}
