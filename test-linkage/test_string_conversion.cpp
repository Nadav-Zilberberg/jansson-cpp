#include <gtest/gtest.h>

extern "C" {
#include "jansson.h"
}

// Test string to JSON conversion
TEST(StringConversionTest, StringToJson) {
    const char* str = "\"hello world\"";
    json_error_t error;
    json_t* result = json_loads(str, 0, &error);
    
    ASSERT_NE(result, nullptr);
    EXPECT_TRUE(json_is_string(result));
    EXPECT_STREQ(json_string_value(result), "hello world");
    
    json_decref(result);
}

// Test JSON to string conversion
TEST(StringConversionTest, JsonToString) {
    json_t* str = json_string("test string");
    ASSERT_NE(str, nullptr);
    
    char* result = json_dumps(str, 0);
    ASSERT_NE(result, nullptr);
    EXPECT_STREQ(result, "\"test string\"");
    
    free(result);
    json_decref(str);
}

// Test null JSON object
TEST(BasicJsonTest, NullObject) {
    json_t* null_obj = json_null();
    ASSERT_NE(null_obj, nullptr);
    EXPECT_TRUE(json_is_null(null_obj));
    json_decref(null_obj);
}

// Test boolean JSON object
TEST(BasicJsonTest, BooleanObject) {
    json_t* true_obj = json_true();
    json_t* false_obj = json_false();
    
    ASSERT_NE(true_obj, nullptr);
    ASSERT_NE(false_obj, nullptr);
    EXPECT_TRUE(json_is_true(true_obj));
    EXPECT_TRUE(json_is_false(false_obj));
    
    json_decref(true_obj);
    json_decref(false_obj);
}

// Test integer JSON object
TEST(BasicJsonTest, IntegerObject) {
    json_t* int_obj = json_integer(42);
    ASSERT_NE(int_obj, nullptr);
    EXPECT_TRUE(json_is_integer(int_obj));
    EXPECT_EQ(json_integer_value(int_obj), 42);
    json_decref(int_obj);
}

// Test real number JSON object
TEST(BasicJsonTest, RealObject) {
    json_t* real_obj = json_real(3.14);
    ASSERT_NE(real_obj, nullptr);
    EXPECT_TRUE(json_is_real(real_obj));
    EXPECT_DOUBLE_EQ(json_real_value(real_obj), 3.14);
    json_decref(real_obj);
}
