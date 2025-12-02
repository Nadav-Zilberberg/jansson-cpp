#include "test_base.hpp"
#include <gtest/gtest.h>

using namespace jasson;

class SimpleJsonTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
    
    void TearDown() override {
        // Cleanup code if needed
    }
};

TEST_F(SimpleJsonTest, BasicJsonValueCreation) {
    // Test basic JSON value creation
    auto null_val = std::make_shared<JsonNull>();
    EXPECT_TRUE(null_val != nullptr);
    
    auto bool_val = std::make_shared<JsonBoolean>(true);
    EXPECT_TRUE(bool_val != nullptr);
    EXPECT_TRUE(bool_val->is_boolean());
    
    auto int_val = std::make_shared<JsonInteger>(42);
    EXPECT_TRUE(int_val != nullptr);
    EXPECT_TRUE(int_val->is_integer());
    
    auto real_val = std::make_shared<JsonReal>(3.14);
    EXPECT_TRUE(real_val != nullptr);
    EXPECT_TRUE(real_val->is_real());
    
    auto str_val = std::make_shared<JsonString>("test");
    EXPECT_TRUE(str_val != nullptr);
    EXPECT_TRUE(str_val->is_string());
}

TEST_F(SimpleJsonTest, JsonArrayCreation) {
    // Test array creation
    auto arr = std::make_shared<JsonArray>();
    EXPECT_TRUE(arr != nullptr);
    EXPECT_TRUE(arr->is_array());
    EXPECT_EQ(arr->size(), 0);
    
    auto int_val = std::make_shared<JsonInteger>(42);
    auto str_val = std::make_shared<JsonString>("test");
    
    arr->append(int_val);
    arr->append(str_val);
    
    EXPECT_EQ(arr->size(), 2);
}

TEST_F(SimpleJsonTest, JsonObjectCreation) {
    // Test object creation
    auto obj = std::make_shared<JsonObject>();
    EXPECT_TRUE(obj != nullptr);
    EXPECT_TRUE(obj->is_object());
    EXPECT_EQ(obj->size(), 0);
    
    auto str_val = std::make_shared<JsonString>("test");
    auto int_val = std::make_shared<JsonInteger>(42);
    auto bool_val = std::make_shared<JsonBoolean>(true);
    
    obj->set("name", str_val);
    obj->set("age", int_val);
    obj->set("active", bool_val);
    
    EXPECT_EQ(obj->size(), 3);
}

TEST_F(SimpleJsonTest, JsonSerialization) {
    // Test serialization
    auto arr = std::make_shared<JsonArray>();
    auto int_val = std::make_shared<JsonInteger>(42);
    auto str_val = std::make_shared<JsonString>("test");
    
    arr->append(int_val);
    arr->append(str_val);
    
    auto obj = std::make_shared<JsonObject>();
    obj->set("name", str_val);
    obj->set("age", int_val);
    
    std::string arr_str = arr->to_string();
    std::string obj_str = obj->to_string();
    
    EXPECT_FALSE(arr_str.empty());
    EXPECT_FALSE(obj_str.empty());
    
    // Basic validation of serialized format - check that it contains the expected elements
    EXPECT_EQ(arr_str, "[42,\"test\"]");
    
    // For object, just check that it contains the expected keys and values
    // (order is not guaranteed in JSON objects)
    EXPECT_TRUE(obj_str.find("\"name\":\"test\"") != std::string::npos);
    EXPECT_TRUE(obj_str.find("\"age\":42") != std::string::npos);
}
