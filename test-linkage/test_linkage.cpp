#include "jansson_cpp.hpp"
#include <gtest/gtest.h>
#include <iostream>

using namespace jansson_cpp;

class LinkageTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::cout << "Setting up test..." << std::endl;
    }
    
    void TearDown() override {
        std::cout << "Tearing down test..." << std::endl;
    }
};

TEST_F(LinkageTest, ObjectCreation) {
    JsonValue obj = make_object();
    EXPECT_TRUE(obj.is_object());
    EXPECT_TRUE(obj);
    EXPECT_EQ(obj.type(), JSON_OBJECT);
}

TEST_F(LinkageTest, ArrayCreation) {
    JsonValue arr = make_array();
    EXPECT_TRUE(arr.is_array());
    EXPECT_TRUE(arr);
    EXPECT_EQ(arr.type(), JSON_ARRAY);
}

TEST_F(LinkageTest, StringCreation) {
    JsonValue str = make_string("test string");
    EXPECT_TRUE(str.is_string());
    EXPECT_TRUE(str);
    EXPECT_EQ(str.type(), JSON_STRING);
}

TEST_F(LinkageTest, IntegerCreation) {
    JsonValue num = make_integer(42);
    EXPECT_TRUE(num.is_integer());
    EXPECT_TRUE(num);
    EXPECT_EQ(num.type(), JSON_INTEGER);
}

TEST_F(LinkageTest, RealCreation) {
    JsonValue real = make_real(3.14159);
    EXPECT_TRUE(real.is_real());
    EXPECT_TRUE(real);
    EXPECT_EQ(real.type(), JSON_REAL);
}

TEST_F(LinkageTest, BooleanCreation) {
    JsonValue bool_true = make_true();
    EXPECT_TRUE(bool_true.is_boolean());
    EXPECT_TRUE(bool_true);
    EXPECT_EQ(bool_true.type(), JSON_TRUE);
    
    JsonValue bool_false = make_false();
    EXPECT_TRUE(bool_false.is_boolean());
    EXPECT_TRUE(bool_false);
    EXPECT_EQ(bool_false.type(), JSON_FALSE);
}

TEST_F(LinkageTest, NullCreation) {
    JsonValue null_val = make_null();
    EXPECT_TRUE(null_val.is_null());
    EXPECT_TRUE(null_val);
    EXPECT_EQ(null_val.type(), JSON_NULL);
}

TEST_F(LinkageTest, ReferenceCounting) {
    JsonValue obj1 = make_object();
    JsonValue obj2 = obj1;  // Copy constructor
    
    EXPECT_TRUE(obj1.is_object());
    EXPECT_TRUE(obj2.is_object());
    
    JsonValue obj3;
    obj3 = obj1;  // Assignment operator
    EXPECT_TRUE(obj3.is_object());
}

TEST_F(LinkageTest, MoveSemantics) {
    JsonValue obj1 = make_object();
    JsonValue obj2 = std::move(obj1);  // Move constructor
    
    EXPECT_FALSE(obj1);  // obj1 should be empty after move
    EXPECT_TRUE(obj2.is_object());
    
    JsonValue obj3;
    obj3 = std::move(obj2);  // Move assignment
    EXPECT_FALSE(obj2);  // obj2 should be empty after move
    EXPECT_TRUE(obj3.is_object());
}

TEST_F(LinkageTest, TypeChecking) {
    JsonValue obj = make_object();
    JsonValue arr = make_array();
    JsonValue str = make_string("test");
    JsonValue num = make_integer(42);
    JsonValue real = make_real(3.14);
    JsonValue bool_true = make_true();
    JsonValue bool_false = make_false();
    JsonValue null_val = make_null();
    
    EXPECT_TRUE(obj.is_object());
    EXPECT_FALSE(obj.is_array());
    EXPECT_FALSE(obj.is_string());
    EXPECT_FALSE(obj.is_integer());
    EXPECT_FALSE(obj.is_real());
    EXPECT_FALSE(obj.is_boolean());
    EXPECT_FALSE(obj.is_null());
    
    EXPECT_TRUE(arr.is_array());
    EXPECT_FALSE(arr.is_object());
    
    EXPECT_TRUE(str.is_string());
    EXPECT_FALSE(str.is_object());
    
    EXPECT_TRUE(num.is_integer());
    EXPECT_TRUE(num.is_number());
    EXPECT_FALSE(num.is_real());
    
    EXPECT_TRUE(real.is_real());
    EXPECT_TRUE(real.is_number());
    EXPECT_FALSE(real.is_integer());
    
    EXPECT_TRUE(bool_true.is_boolean());
    EXPECT_TRUE(bool_false.is_boolean());
    EXPECT_FALSE(bool_true.is_null());
    EXPECT_FALSE(bool_false.is_null());
    
    EXPECT_TRUE(null_val.is_null());
    EXPECT_FALSE(null_val.is_object());
}

int main(int argc, char **argv) {
    std::cout << "Running linkage tests with shared library..." << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
