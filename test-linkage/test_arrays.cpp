#include "jansson-cpp.hpp"
#include <iostream>
#include <cassert>

using namespace jasson;

void test_json_array_creation() {
    std::cout << "Testing JSON Array Creation..." << std::endl;
    
    auto array = std::make_shared<JsonArray>();
    assert(array->is_array());
    assert(!array->is_object());
    assert(!array->is_string());
    assert(array->size() == 0);
    
    std::cout << "✓ JSON Array creation test passed" << std::endl;
}

void test_json_array_append() {
    std::cout << "Testing JSON Array Append..." << std::endl;
    
    auto array = std::make_shared<JsonArray>();
    
    // Append different types
    array->append(static_cast<json_int_t>(42));
    array->append(3.14);
    array->append(std::string("hello"));  // Explicit string
    array->append(true);
    array->append_null();
    
    assert(array->size() == 5);
    
    // Check first element (integer)
    auto elem0 = array->at(0);
    assert(elem0->is_integer());
    assert(elem0->as_integer()->value() == 42);
    
    // Check second element (real)
    auto elem1 = array->at(1);
    assert(elem1->is_real());
    assert(elem1->as_real()->value() == 3.14);
    
    // Check third element (string)
    auto elem2 = array->at(2);
    assert(elem2->is_string());
    assert(elem2->as_string()->value() == "hello");
    
    // Check fourth element (boolean)
    auto elem3 = array->at(3);
    assert(elem3->is_boolean());
    assert(elem3->as_boolean()->value() == true);
    
    // Check fifth element (null)
    auto elem4 = array->at(4);
    assert(elem4->is_null());
    
    std::cout << "✓ JSON Array append test passed" << std::endl;
}

void test_json_array_nested() {
    std::cout << "Testing JSON Array Nested..." << std::endl;
    
    auto outer_array = std::make_shared<JsonArray>();
    auto inner_array = std::make_shared<JsonArray>();
    auto inner_object = std::make_shared<JsonObject>();
    
    // Populate inner array
    inner_array->append(static_cast<json_int_t>(1));
    inner_array->append(static_cast<json_int_t>(2));
    inner_array->append(static_cast<json_int_t>(3));
    
    // Populate inner object
    inner_object->set("name", std::string("test"));
    inner_object->set("value", static_cast<json_int_t>(42));
    
    // Add to outer array
    outer_array->append(inner_array);
    outer_array->append(inner_object);
    outer_array->append(std::string("simple string"));
    
    assert(outer_array->size() == 3);
    
    // Check first element (inner array)
    auto elem0 = outer_array->at(0);
    assert(elem0->is_array());
    assert(elem0->as_array()->size() == 3);
    assert(elem0->as_array()->at(0)->as_integer()->value() == 1);
    
    // Check second element (inner object)
    auto elem1 = outer_array->at(1);
    assert(elem1->is_object());
    assert(elem1->as_object()->has("name"));
    assert(elem1->as_object()->has("value"));
    
    // Check third element (string)
    auto elem2 = outer_array->at(2);
    assert(elem2->is_string());
    assert(elem2->as_string()->value() == "simple string");
    
    std::cout << "✓ JSON Array nested test passed" << std::endl;
}

void test_json_array_bounds() {
    std::cout << "Testing JSON Array Bounds..." << std::endl;
    
    auto array = std::make_shared<JsonArray>();
    array->append(std::string("test"));
    
    // Valid access
    auto elem = array->at(0);
    assert(elem->is_string());
    
    // Invalid access should throw
    bool exception_thrown = false;
    try {
        array->at(1); // Out of bounds
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    
    std::cout << "✓ JSON Array bounds test passed" << std::endl;
}

void test_json_array_serialization() {
    std::cout << "Testing JSON Array Serialization..." << std::endl;
    
    auto array = std::make_shared<JsonArray>();
    array->append(static_cast<json_int_t>(42));
    array->append(std::string("hello"));
    array->append(true);
    array->append_null();
    
    std::string compact = array->dump();
    std::cout << "Compact array: " << compact << std::endl;
    assert(compact == "[42,\"hello\",true,null]");
    
    std::string pretty = array->dump(2);
    std::cout << "Pretty array: " << pretty << std::endl;
    assert(pretty.find("\n") != std::string::npos);
    assert(pretty.find("  ") != std::string::npos);
    
    std::cout << "✓ JSON Array serialization test passed" << std::endl;
}

int main() {
    std::cout << "=== Running JSON Array Tests ===" << std::endl;
    
    try {
        test_json_array_creation();
        test_json_array_append();
        test_json_array_nested();
        test_json_array_bounds();
        test_json_array_serialization();
        
        std::cout << "\n🎉 All JSON array tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "❌ Test failed with unknown exception" << std::endl;
        return 1;
    }
}
