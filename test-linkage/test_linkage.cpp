#include "jansson.h"
#include <iostream>
#include <cassert>
#include <cstring>
#include <memory>

// Test function declarations
void test_c_interface();
void test_cpp_types();
void test_exceptions();
void test_json_builder();
void test_literals();
void test_raii();
void test_smart_pointers();

int main() {
    std::cout << "Running comprehensive JSON library tests..." << std::endl;
    
    try {
        test_c_interface();
        std::cout << "✓ C interface tests passed" << std::endl;
        
        test_cpp_types();
        std::cout << "✓ C++ types tests passed" << std::endl;
        
        test_exceptions();
        std::cout << "✓ Exception tests passed" << std::endl;
        
        test_json_builder();
        std::cout << "✓ JSON builder tests passed" << std::endl;
        
        test_literals();
        std::cout << "✓ Literal tests passed" << std::endl;
        
        test_raii();
        std::cout << "✓ RAII tests passed" << std::endl;
        
        test_smart_pointers();
        std::cout << "✓ Smart pointer tests passed" << std::endl;
        
        std::cout << "\n🎉 All tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}

void test_c_interface() {
    // Test basic C API functionality
    json_t* obj = json_object();
    assert(obj != nullptr);
    
    json_t* str = json_string("test");
    assert(str != nullptr);
    
    int result = json_object_set(obj, "key", str);
    assert(result == 0);
    
    json_t* retrieved = json_object_get(obj, "key");
    assert(retrieved != nullptr);
    assert(strcmp(json_string_value(retrieved), "test") == 0);
    
    json_decref(obj);
}

void test_cpp_types() {
    using namespace jansson;
    
    // Test string type
    auto str = make_string("Hello, World!");
    assert(str->is_string());
    assert(!str->is_number());
    assert(str->as_string() != nullptr);
    assert(str->as_string()->value() == "Hello, World!");
    
    // Test integer type
    auto int_val = make_integer(42);
    assert(int_val->is_integer());
    assert(int_val->is_number());
    assert(int_val->as_integer() != nullptr);
    assert(int_val->as_integer()->value() == 42);
    
    // Test real type
    auto real_val = make_real(3.14159);
    assert(real_val->is_real());
    assert(real_val->is_number());
    assert(real_val->as_real() != nullptr);
    assert(real_val->as_real()->value() == 3.14159);
    
    // Test boolean type
    auto bool_true = make_boolean(true);
    auto bool_false = make_boolean(false);
    assert(bool_true->is_boolean());
    assert(bool_false->is_boolean());
    assert(bool_true->as_boolean()->value() == true);
    assert(bool_false->as_boolean()->value() == false);
    
    // Test null type
    auto null_val = make_null();
    assert(null_val->is_null());
    assert(!null_val->is_string());
    
    // Test array type
    auto array = make_array();
    assert(array->is_array());
    assert(array->size() == 0);
    assert(array->empty());
    
    array->append(make_integer(1));
    array->append(make_integer(2));
    array->append(make_integer(3));
    assert(array->size() == 3);
    assert(!array->empty());
    assert(array->at(0)->as_integer()->value() == 1);
    assert(array->at(1)->as_integer()->value() == 2);
    assert(array->at(2)->as_integer()->value() == 3);
    
    // Test object type
    auto obj = make_object();
    assert(obj->is_object());
    assert(obj->size() == 0);
    assert(obj->empty());
    
    obj->set("name", make_string("John"));
    obj->set("age", make_integer(30));
    obj->set("active", make_boolean(true));
    assert(obj->size() == 3);
    assert(!obj->empty());
    assert(obj->has_key("name"));
    assert(obj->has_key("age"));
    assert(obj->has_key("active"));
    assert(!obj->has_key("nonexistent"));
    
    assert(obj->get("name")->as_string()->value() == "John");
    assert(obj->get("age")->as_integer()->value() == 30);
    assert(obj->get("active")->as_boolean()->value() == true);
}

void test_exceptions() {
    using namespace jansson;
    
    // Test array out of range exception
    auto array = make_array();
    bool caught_exception = false;
    try {
        auto val = array->at(0); // Should throw
    } catch (const JsonOutOfRangeException& e) {
        caught_exception = true;
        std::string msg = e.what();
        assert(msg.find("Array index out of range") != std::string::npos);
    }
    assert(caught_exception);
    
    // Test object key not found exception
    auto obj = make_object();
    caught_exception = false;
    try {
        auto val = obj->get("nonexistent"); // Should throw
    } catch (const JsonOutOfRangeException& e) {
        caught_exception = true;
        std::string msg = e.what();
        assert(msg.find("Key not found") != std::string::npos);
    }
    assert(caught_exception);
    
    // Test type exception in builder
    caught_exception = false;
    try {
        JsonBuilder builder = JsonBuilder::string("test");
        builder.append(make_integer(1)); // Should throw - can't append to string
    } catch (const JsonTypeException& e) {
        caught_exception = true;
        std::string msg = e.what();
        assert(msg.find("Cannot append to non-array value") != std::string::npos);
    }
    assert(caught_exception);
    
    // Test type exception in builder for object operations
    caught_exception = false;
    try {
        JsonBuilder builder = JsonBuilder::integer(42);
        builder.set("key", make_string("value")); // Should throw - can't set property on non-object
    } catch (const JsonTypeException& e) {
        caught_exception = true;
        std::string msg = e.what();
        assert(msg.find("Cannot set property on non-object value") != std::string::npos);
    }
    assert(caught_exception);
}

void test_json_builder() {
    using namespace jansson;
    
    // Test building a simple object
    auto obj = JsonBuilder::object()
        .set("name", JsonBuilder::string("Alice"))
        .set("age", JsonBuilder::integer(25))
        .set("active", JsonBuilder::boolean(true))
        .set("score", JsonBuilder::real(95.5))
        .build();
    
    assert(obj->is_object());
    assert(obj->as_object()->get("name")->as_string()->value() == "Alice");
    assert(obj->as_object()->get("age")->as_integer()->value() == 25);
    assert(obj->as_object()->get("active")->as_boolean()->value() == true);
    assert(obj->as_object()->get("score")->as_real()->value() == 95.5);
    
    // Test building a nested structure
    auto nested = JsonBuilder::object()
        .set("user", JsonBuilder::object()
            .set("id", JsonBuilder::integer(123))
            .set("name", JsonBuilder::string("Bob"))
            .build())
        .set("items", JsonBuilder::array()
            .append(JsonBuilder::string("item1"))
            .append(JsonBuilder::string("item2"))
            .append(JsonBuilder::integer(42))
            .build())
        .set("metadata", JsonBuilder::object()
            .set("version", JsonBuilder::string("1.0"))
            .set("timestamp", JsonBuilder::integer(1234567890))
            .build())
        .build();
    
    assert(nested->is_object());
    assert(nested->as_object()->has_key("user"));
    assert(nested->as_object()->has_key("items"));
    assert(nested->as_object()->has_key("metadata"));
    
    auto user = nested->as_object()->get("user");
    assert(user->is_object());
    assert(user->as_object()->get("id")->as_integer()->value() == 123);
    assert(user->as_object()->get("name")->as_string()->value() == "Bob");
    
    auto items = nested->as_object()->get("items");
    assert(items->is_array());
    assert(items->as_array()->size() == 3);
    assert(items->as_array()->at(0)->as_string()->value() == "item1");
    assert(items->as_array()->at(1)->as_string()->value() == "item2");
    assert(items->as_array()->at(2)->as_integer()->value() == 42);
    
    // Test serialization
    std::string json_str = nested->to_string();
    assert(!json_str.empty());
    assert(json_str.find("\"user\"") != std::string::npos);
    assert(json_str.find("\"items\"") != std::string::npos);
    assert(json_str.find("\"metadata\"") != std::string::npos);
}

void test_literals() {
    using namespace jansson::literals;
    
    // Test integer literal
    auto int_lit = 42_json_int;
    assert(int_lit->is_integer());
    assert(int_lit->as_integer()->value() == 42);
    
    // Test real literal
    auto real_lit = 3.14159_json_real;
    assert(real_lit->is_real());
    assert(real_lit->as_real()->value() == 3.14159);
    
    // Note: String literal parsing would require a full JSON parser implementation
    // This is a simplified example
}

void test_raii() {
    using namespace jansson;
    
    // Test that objects are properly cleaned up when going out of scope
    {
        auto obj = make_object();
        obj->set("test", make_string("value"));
        // obj should be automatically cleaned up when it goes out of scope
    }
    
    {
        auto array = make_array();
        for (int i = 0; i < 100; ++i) {
            array->append(make_integer(i));
        }
        // array and all its elements should be automatically cleaned up
    }
    
    // Test nested structures with RAII
    {
        auto root = make_object();
        auto users = make_array();
        
        for (int i = 0; i < 10; ++i) {
            auto user = make_object();
            user->set("id", make_integer(i));
            user->set("name", make_string("User" + std::to_string(i)));
            users->append(user);
        }
        
        root->set("users", users);
        root->set("count", make_integer(10));
        
        // All objects should be properly cleaned up when going out of scope
    }
}

void test_smart_pointers() {
    using namespace jansson;
    
    // Test shared_ptr semantics
    auto obj1 = make_object();
    obj1->set("shared", make_boolean(true));
    
    {
        auto obj2 = obj1; // Shared ownership
        assert(obj2->as_object()->get("shared")->as_boolean()->value() == true);
        obj2->set("modified", make_boolean(true));
        
        // Both obj1 and obj2 point to the same object
        assert(obj1->as_object()->has_key("modified"));
    }
    
    // obj1 still valid after obj2 went out of scope
    assert(obj1->as_object()->has_key("shared"));
    assert(obj1->as_object()->has_key("modified"));
    
    // Test cloning
    auto original = make_object();
    original->set("name", make_string("Original"));
    original->set("value", make_integer(100));
    
    auto clone_obj = original->clone();
    assert(clone_obj->is_object());
    assert(clone_obj->as_object()->get("name")->as_string()->value() == "Original");
    assert(clone_obj->as_object()->get("value")->as_integer()->value() == 100);
    
    // Modify clone to ensure it's independent
    clone_obj->as_object()->set("name", make_string("Clone"));
    assert(original->as_object()->get("name")->as_string()->value() == "Original");
    assert(clone_obj->as_object()->get("name")->as_string()->value() == "Clone");
}
