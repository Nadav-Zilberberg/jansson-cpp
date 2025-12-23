#include "jansson-cpp.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

using namespace jasson;

void test_json_null() {
    std::cout << "Testing JSON Null..." << std::endl;
    
    auto null_val = std::make_shared<JsonNull>();
    assert(null_val != nullptr);
    assert(null_val->is_null());
    assert(!null_val->is_string());
    assert(!null_val->is_number());
    assert(!null_val->is_boolean());
    assert(!null_val->is_array());
    assert(!null_val->is_object());
    
    assert(null_val->to_string() == "null");
    assert(null_val->dump() == "null");
    
    std::cout << "✓ JSON Null test passed" << std::endl;
}

void test_json_boolean() {
    std::cout << "Testing JSON Boolean..." << std::endl;
    
    auto true_val = std::make_shared<JsonBoolean>(true);
    auto false_val = std::make_shared<JsonBoolean>(false);
    
    assert(true_val->is_boolean());
    assert(true_val->value() == true);
    assert(true_val->to_string() == "true");
    assert(true_val->dump() == "true");
    
    assert(false_val->is_boolean());
    assert(false_val->value() == false);
    assert(false_val->to_string() == "false");
    assert(false_val->dump() == "false");
    
    std::cout << "✓ JSON Boolean test passed" << std::endl;
}

void test_json_integer() {
    std::cout << "Testing JSON Integer..." << std::endl;
    
    auto int_val = std::make_shared<JsonInteger>(42);
    assert(int_val->is_integer());
    assert(int_val->is_number());
    assert(!int_val->is_real());
    assert(int_val->value() == 42);
    assert(int_val->to_string() == "42");
    assert(int_val->dump() == "42");
    
    auto negative_val = std::make_shared<JsonInteger>(-123);
    assert(negative_val->value() == -123);
    assert(negative_val->dump() == "-123");
    
    std::cout << "✓ JSON Integer test passed" << std::endl;
}

void test_json_real() {
    std::cout << "Testing JSON Real..." << std::endl;
    
    auto real_val = std::make_shared<JsonReal>(3.14159);
    assert(real_val->is_real());
    assert(real_val->is_number());
    assert(!real_val->is_integer());
    assert(std::abs(real_val->value() - 3.14159) < 0.00001);
    
    auto negative_real = std::make_shared<JsonReal>(-2.718);
    assert(std::abs(negative_real->value() - (-2.718)) < 0.001);
    
    std::cout << "✓ JSON Real test passed" << std::endl;
}

void test_json_string() {
    std::cout << "Testing JSON String..." << std::endl;
    
    auto str_val = std::make_shared<JsonString>("Hello, World!");
    assert(str_val->is_string());
    assert(!str_val->is_number());
    assert(str_val->value() == "Hello, World!");
    assert(str_val->to_string() == "Hello, World!");
    assert(str_val->dump() == "\"Hello, World!\"");
    
    auto empty_str = std::make_shared<JsonString>("");
    assert(empty_str->value() == "");
    assert(empty_str->dump() == "\"\"");
    
    auto special_str = std::make_shared<JsonString>("test\nwith\ttabs");
    std::string dumped = special_str->dump();
    assert(dumped.find("\\n") != std::string::npos);
    assert(dumped.find("\\t") != std::string::npos);
    
    std::cout << "✓ JSON String test passed" << std::endl;
}

void test_factory_functions() {
    std::cout << "Testing Factory Functions..." << std::endl;
    
    auto null_val = make_null();
    assert(null_val->is_null());
    
    auto bool_val = make_bool(true);
    assert(bool_val->is_boolean());
    assert(bool_val->as_boolean()->value() == true);
    
    auto int_val = make_int(static_cast<json_int_t>(123));
    assert(int_val->is_integer());
    assert(int_val->as_integer()->value() == 123);
    
    auto real_val = make_real(45.67);
    assert(real_val->is_real());
    assert(std::abs(real_val->as_real()->value() - 45.67) < 0.01);
    
    auto str_val = make_string("factory test");
    assert(str_val->is_string());
    assert(str_val->as_string()->value() == "factory test");
    
    auto array_val = make_array();
    assert(array_val->is_array());
    
    auto object_val = make_object();
    assert(object_val->is_object());
    
    std::cout << "✓ Factory Functions test passed" << std::endl;
}

int main() {
    std::cout << "=== Running Basic JSON Types Tests ===" << std::endl;
    
    try {
        test_json_null();
        test_json_boolean();
        test_json_integer();
        test_json_real();
        test_json_string();
        test_factory_functions();
        
        std::cout << "\n🎉 All basic JSON types tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "❌ Test failed with unknown exception" << std::endl;
        return 1;
    }
}
